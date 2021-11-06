#include <cstring>
#include <sys/unistd.h>
#include <sys/dir.h>
#include <pthread.h>
#include <cerrno>
#include "ServerThread.hpp"

ServerThread::ServerThread(int initMsgsock) : msgsock(initMsgsock) {
}

void ServerThread::start() {
    void *socketAsPointer = (void *) msgsock;
    pthread_create(&tid, nullptr, run, socketAsPointer);
}

void *ServerThread::run(void *arg) {
    DIR *dirp;
    struct direct *d;
    int readBufferLength = 1024;
    char buf1[readBufferLength];
    char buf2[102400];

    int clientsock = static_cast<int>(reinterpret_cast<intptr_t>(arg));

    size_t readVal;
    do {
        readVal = read(clientsock, buf1, readBufferLength);
        if (readVal > 0) {
            printf("%s\n", buf1);
        }
    } while (readVal == readBufferLength);

//    while (((rval = read(clientsock, buf1, readBufferLength)) == 0)) {
//        if (errno != EINTR) {
//            printf("%s\n", buf1);
//        }
//    }


    buf2[0] = '\0';
    strcpy(buf2, "SOME RESPONSE");

    do {
        readVal = write(clientsock, buf2, 1024);
        if (readVal < 0) {
            perror("writing socket");
        }
    } while (readVal == 1024);

//    if ((rval = write(clientsock, buf2, 1024)) < 0) {
//        perror("writing socket");
//    }

    closedir(dirp);
    close(clientsock);
    return nullptr;
}
