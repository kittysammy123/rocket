#ifndef NET_FD_EVENT_H
#define NET_FD_EVENT_H

#include <functional>
#include <sys/epoll.h>

namespace rocket {
    class FdEvent {
    public:

        enum class EventType : std::size_t {
            IN_EVENT = EPOLLIN,
            OUT_EVENT = EPOLLOUT,
        };

        FdEvent(int fd);

        ~FdEvent();

        void handler(EventType type);

        void listen(EventType type,std::function<void()> callback);

        int getFd() {
            return m_event.data.fd;
        }

        epoll_event getEpollEvent() {
            return m_event;
        }
    private:
        epoll_event m_event;

        std::function<void()> m_read_callback;
        
        std::function<void()> m_write_callback;
    };
}

#endif