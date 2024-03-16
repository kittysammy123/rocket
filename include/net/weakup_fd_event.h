#ifndef COMMON_WEAKUP_FD_EVENT_H
#define COMMON_WEAKUP_FD_EVENT_H

#include "net/fd_event.h"


namespace rocket {
    class WeakUpFdEvent : public FdEvent {
    public:
        WeakUpFdEvent(int fd);
        ~WeakUpFdEvent();

        void weakUp();
    private:
        void init();
    };
}

#endif
