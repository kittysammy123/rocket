#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <string.h>
#include "net/eventloop.h"
#include "common/util.h"
#include "common/log.h"

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

        m_pid = getThreadId();
        

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

        INFOLOG("success create event loop in thread %d",m_pid);
        
        t_current_eventloop = this;
    }

    EventLoop::~EventLoop() {

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

        }
    }

    void EventLoop::wakeUp() {

    }

    void EventLoop::stop() {
        m_stop_flag = true;
    }
}