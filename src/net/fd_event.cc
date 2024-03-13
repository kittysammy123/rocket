#include "net/fd_event.h"


namespace rocket {
    FdEvent::FdEvent(int fd) {
        m_event.data.fd = fd;
    }

    FdEvent::~FdEvent() {

    }

    void FdEvent::handler(EventType type) {
        
        switch(type) {
        case EventType::IN_EVENT:
            m_read_callback();
            break;
        case EventType::OUT_EVENT:
            m_write_callback();
            break;
        default:
        };

    }

    void FdEvent::listen(EventType type,std::function<void()> callback) {
        switch (type)
        {
        case EventType::IN_EVENT:
            m_read_callback = callback;
            m_event.events |= EPOLLIN;
            break;
        case EventType::OUT_EVENT:
            m_write_callback = callback;
            m_event.events |= EPOLLOUT;
            break;
        default:
        }
        m_event.data.ptr = this;
    }
}