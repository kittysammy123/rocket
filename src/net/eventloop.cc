#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <string.h>
#include "net/eventloop.h"
#include "common/util.h"
#include "common/log.h"
#include "net/fd_event.h"

namespace rocket {

    static thread_local EventLoop* t_current_eventloop = nullptr;
    static int g_epoll_max_events = 10;
    static int g_epoll_max_timeout = 10000;

    EventLoop::EventLoop() {
        //判断当前线程是否创建过eventloop
        if(t_current_eventloop != nullptr) {
            ERRORLOG("failed to create eventloop,this thread has created eventloop.");
            exit(EXIT_FAILURE);
        }

        m_thread_id = getThreadId();
        

        m_epoll_fd = epoll_create(1);
        if(m_epoll_fd == -1) {
            ERRORLOG("epoll_create failed:%s",strerror(errno));
            exit(EXIT_FAILURE);
        }

        m_weakup_fd = eventfd(0,EFD_NONBLOCK);
        if(m_weakup_fd == -1) {
            ERRORLOG("create eventfd failed:%s",strerror(errno));
            exit(EXIT_FAILURE);
        }

        //注册weakup_fd的可读事件
        epoll_event event;
        event.events = EPOLLIN;

        int rt = epoll_ctl(m_epoll_fd,EPOLL_CTL_ADD,m_weakup_fd,&event);
        if(rt == -1) {
            ERRORLOG("epoll_ctl add weakup_fd failed:%s",strerror(errno));
            exit(EXIT_FAILURE);
        }

        INFOLOG("success create event loop in thread %d",m_thread_id);
        
        t_current_eventloop = this;
    }

    EventLoop::~EventLoop() {

    }

    bool EventLoop::isInLoopThread() {
        return getThreadId() == m_thread_id;
    }

    void EventLoop::addEpollEvent(FdEvent* event) {
        if(isInLoopThread()) {
            
            int op = EPOLL_CTL_ADD;
            auto fd_event = event->getEpollEvent();
            int fd = event->getFd();
            //判断是否在监听队列里面
            auto it = m_listen_fds.find(fd);
            if(it != m_listen_fds.end()) {
                op = EPOLL_CTL_MOD;
            }

            int ret = epoll_ctl(m_epoll_fd,op,fd,&fd_event);
            if(ret == -1) {
                ERRORLOG("epoll_ctl fd=%d op=%d failed:%s.",fd,op,strerror(errno));
                return;
            }

            INFOLOG("epoll_ctl add fd=%d op=%d success.",fd,op);
        }
        else
        {
            //添加任务
            auto cb = [this,event]() {
                this->addEpollEvent(event);
            };
            addTask(cb,true);
        }

    }

    void EventLoop::deleteEpollEvent(FdEvent* event) {
        if(isInLoopThread()) {
            int fd = event->getFd();

            auto it = m_listen_fds.find(fd);
            if(it == m_listen_fds.end()) {
                return;
            }

            int op = EPOLL_CTL_DEL;
            auto fd_event = event->getEpollEvent();
            
            int ret = epoll_ctl(m_epoll_fd,op,fd,&fd_event);
            if(ret == -1) {
                ERRORLOG("epoll_ctl fd=%d op=%d failed:%s.",fd,op,strerror(errno));
                return;
            }
            INFOLOG("epoll_ctl add fd=%d op=%d success.",fd,op);
        }
        else
        {
            //添加任务
            auto cb = [this,event]() {
                this->deleteEpollEvent(event);
            };
            addTask(cb,true);
        }
    }

    void EventLoop::addTask(std::function<void()> task,bool is_weak_up) {
        {
            ScopeMutex<Mutex> lock(m_mutex);
            m_pending_tasks.push(task);
        }

        if(is_weak_up) {
            wakeUp();
        }
    }

    void EventLoop::loop() {
        while(!m_stop_flag) {
            std::queue<std::function<void()>> tmp_tasks;

            //tasks  取出来
            {
                ScopeMutex<Mutex> lock(m_mutex);
                m_pending_tasks.swap(tmp_tasks);
            }

            while(!tmp_tasks.empty()) {
                tmp_tasks.front()();
                tmp_tasks.pop();
            }

            epoll_event events[g_epoll_max_events];
            int timeout = g_epoll_max_timeout;

            int rt = epoll_wait(m_epoll_fd,events,g_epoll_max_events,timeout);

            if(rt < 0) {
                ERRORLOG("epoll_wait error:%s",strerror(errno));
            }
            else
            {
                for(int i = 0; i < rt; i++) {
                    auto trigger_event = events[i];
                    //获取到对应的FdEvent
                    auto fd_event_ptr = static_cast<FdEvent*>(trigger_event.data.ptr);
                    if(fd_event_ptr == nullptr) {
                        continue;
                    }
                    //可读事件，则添加可读任务回调
                    if(trigger_event.events | EPOLLIN) {
                        addTask(fd_event_ptr->getHandler(FdEvent::EventType::IN_EVENT));
                    }

                    if(trigger_event.events | EPOLLOUT) {
                        addTask(fd_event_ptr->getHandler(FdEvent::EventType::OUT_EVENT));
                    }
                }
            }
        }
    }

    void EventLoop::wakeUp() {

    }

    void EventLoop::stop() {
        m_stop_flag = true;
    }
}