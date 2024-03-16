#include <unistd.h>
#include "net/weakup_fd_event.h"
#include "common/log.h"

namespace rocket {
    WeakUpFdEvent::WeakUpFdEvent(int fd) : FdEvent(fd){
        init();
    }


    WeakUpFdEvent::~WeakUpFdEvent(){

    }

    //用于设置weakup fd的回调函数
    void WeakUpFdEvent::init() {
        auto read_callback = [this]() {
            char buf[8];
            while(read(getFd(),buf,8) != -1 && errno != EAGAIN) {

            }
            DEBUGLOG("read weakup finished.");
        };

        listen(EventType::IN_EVENT,read_callback);
    }

    //weakup唤醒，其实就是写数据
    void WeakUpFdEvent::weakUp() {
        char buf[8] = {'a'};
        auto fd = getFd();
        int ret = write(fd,buf,8);
        if(ret != 8) {
            ERRORLOG("write to weakup fd less than 8 bytes,fd[%d]",fd);
        }
    }
}