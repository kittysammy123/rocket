#include <memory>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common/log.h"
#include "common/config.h"
#include "net/eventloop.h"
#include "net/fd_event.h"


int main() {
    
    rocket::Config::SetGlobalConfig("../conf/rocket.xml");
    rocket::Logger::InitGlobalLogger();

    auto eventloop_ptr = std::make_shared<rocket::EventLoop>();

    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd == -1) {
        ERRORLOG("listenfd == -1.");
        exit(EXIT_FAILURE);
    }

    sockaddr_in addr;
    addr.sin_port = htons(12345);
    addr.sin_family = AF_INET;
    inet_aton("127.0.0.1",&addr.sin_addr);

    //bind
    int ret = bind(listenfd,reinterpret_cast<sockaddr*>(&addr),sizeof(addr));
    if(ret != 0) {
        ERRORLOG("bind error.");
        exit(EXIT_FAILURE);
    }

    ret = listen(listenfd,100);
    if(ret != 0) {
        ERRORLOG("listen error");
        exit(EXIT_FAILURE);
    }

    rocket::FdEvent event(listenfd);
    event.listen(rocket::FdEvent::EventType::IN_EVENT,[listenfd](){
        sockaddr_in peer_addr;
        socklen_t addr_len = sizeof(peer_addr);
        memset(&peer_addr,0,sizeof(peer_addr));
        int clientfd = accept(listenfd,reinterpret_cast<sockaddr*>(&peer_addr),&addr_len);
        DEBUGLOG("success get clientfd=%d [%s:%d].",clientfd, inet_ntoa(peer_addr.sin_addr),ntohs(peer_addr.sin_port));
    });

    eventloop_ptr->addEpollEvent(&event);
    eventloop_ptr->loop();

    return 0;
}
