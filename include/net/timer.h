#ifndef NET_TIMER_H
#define NET_TIMER_H

//用于管理定时事件
#include <map>
#include "net/fd_event.h"
#include "net/timer_event.h"

namespace rocket {

    class Timer : FdEvent {
    public:
        Timer();
        ~Timer();

        void addTimeEvent(TimerEvent::ptr event);

        void deleteTimeEvent(TimerEvent::ptr event);

        void onTime();

    private:
        std::multimap<int64_t,TimerEvent::ptr> m_pending_tasks;  
    };
}


#endif