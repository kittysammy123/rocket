#include <chrono>
#include "net/timer_event.h"
#include "common/log.h"

namespace rocket {
    TimerEvent::TimerEvent(int interval,std::function<void()> cb,bool is_repeated = false) {
        
        m_arrive_time = std::chrono::duration_cast<std::chrono::milliseconds> 
                            (std::chrono::system_clock::now().time_since_epoch()).count() + interval;
        
        m_interval = interval;
        m_task = cb;
        m_is_repeated = is_repeated;

        DEBUGLOG("success create time event,will excute at [%lld].",m_arrive_time);
    }
}