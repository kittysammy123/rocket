#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include <pthread.h>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include "common/mutex.h"

namespace rocket {
    //前项申明
    class FdEvent;
    class WeakUpFdEvent;


    class EventLoop {
    public:
        EventLoop();
        ~EventLoop();

        void loop();

        void wakeUp();

        void stop();

        //带weak_up是为了紧急的task需要尽快执行
        void addTask(std::function<void()> task,bool is_weak_up = false);
    
        void addEpollEvent(FdEvent* event);

        void deleteEpollEvent(FdEvent* event);
    private:
        void dealWeakUp();
        void initWeakUpFdEvent();

        bool isInLoopThread();
    private:
        pid_t m_thread_id{0};

        int m_epoll_fd {0};

        //int m_weakup_fd {0};

        std::shared_ptr<WeakUpFdEvent> m_weakup_fd_event;

        bool m_stop_flag {false};

        std::set<int> m_listen_fds;

        std::queue<std::function<void()>> m_pending_tasks;

        Mutex m_mutex;
    };
}




#endif