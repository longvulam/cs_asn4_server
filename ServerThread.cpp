#include <cstring>
#include <sys/unistd.h>
#include <sys/dir.h>
#include <pthread.h>

#include "ServerThread.hpp"

ServerThread::ServerThread(int initMsgsock): msgsock(initMsgsock) {
}

void *ServerThread::run(void *arg) {
    DIR *dirp;
    struct direct *d;
    char buf1[102400];
    char buf2[102400];

    int clientsock = static_cast<int>(reinterpret_cast<intptr_t>(arg));

    int rval;
    /* pthread_detach(pthread_self());*/
    while((rval = read(clientsock, buf1, 80)) >= 0){
        printf("%s\n", buf1);
    }

    buf2[0] = '\0';
    strcpy(buf2, "SOME RESPONSE");

    closedir(dirp);
    close(clientsock);
    return nullptr;
}

void ServerThread::start() {
    void *socketAsPointer = (void *) msgsock;
    pthread_create(&tid, nullptr, run, socketAsPointer);
}
