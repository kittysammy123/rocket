#include <pthread.h>
#include <unistd.h>
#include <unistd.h>
#include "common/log.h"
#include "common/config.h"

void *func(void *data) {
    DEBUGLOG("this is a thread for :%d",*(int*)data);
    //INFOLOG("this is info:%d",*(int*)data);
    return NULL;
}

int main() {
    
    printf("dir=%s\n",get_current_dir_name());
    rocket::Config::SetGlobalConfig("../conf/rocket.xml");
    rocket::Logger::InitGlobalLogger();

    //DEBUGLOG("test log %s","11");
    pthread_t thread;
    for(int i=0;i<10;i++) {
        auto arg = new int(i);
        pthread_create(&thread,NULL,func,arg);
    }
    
    sleep(1);
    pthread_join(thread,NULL);
    return 0;
}

