#include <sys/timerfd.h>
#include <functional>
#include "net/timer.h"
#include "common/log.h"

namespace rocket {

    Timer::Timer() : FdEvent(){
        
        int timer_fd = timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK | TFD_CLOEXEC);

        DEBUGLOG("timer fd=%d.",timer_fd);
        
        setFd(timer_fd);
        //fd可读事件监听
        listen(FdEvent::EventType::IN_EVENT,std::bind(&Timer::onTime,this));

    }
}