#include <pthread.h>
#include <unistd.h>
#include <unistd.h>
#include "common/log.h"
#include "common/config.h"

void *func(void *) {
    DEBUGLOG("this is a thread for");
    return NULL;
}

int main() {
    
    printf("dir=%s",get_current_dir_name());
    rocket::Config::SetGlobalConfig("../conf/rocket.xml");

    DEBUGLOG("test log %s","11");
    pthread_t thread;

    pthread_create(&thread,NULL,func,NULL);
    sleep(1);
    pthread_join(thread,NULL);
    return 0;
}

