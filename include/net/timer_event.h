#ifndef NET_TIMER_EVENT_H
#define NET_TIMER_EVENT_H

#include <memory>
#include <functional>

namespace rocket {
    class TimerEvent {
    public:
        using ptr = std::shared_ptr<TimerEvent>;

        TimerEvent(int interval,std::function<void()> cb,bool is_repeated = false);

        ~TimerEvent();
    private:
        int64_t m_arrive_time;
        int64_t m_interval;
        bool m_is_repeated {false};
        bool m_is_cancled {false};

        std::function<void()> m_task;
    };
}


#endif

