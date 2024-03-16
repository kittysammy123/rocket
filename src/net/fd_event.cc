#include "net/fd_event.h"


namespace rocket {
    FdEvent::FdEvent(int fd) {
        m_fd = fd;
    }

    FdEvent::FdEvent() {

    }

    FdEvent::~FdEvent() {

    }

    void FdEvent::setFd(int fd) {
        m_fd = fd;
    }
    std::function<void()> FdEvent::getHandler(EventType type) {
        
        switch(type) {
        case EventType::IN_EVENT:
            return m_read_callback;
        case EventType::OUT_EVENT:
            return m_write_callback;
        default:
            return nullptr;
        }

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
            break;
        }
        m_event.data.ptr = this;
    }
}