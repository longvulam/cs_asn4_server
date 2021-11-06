#include <cstring>
#include <sys/unistd.h>
#include <sys/dir.h>
#include <pthread.h>
#include <exception>
#include <string>
#include "ServerThread.hpp"
#include "HttpResponse.hpp"
#include "UploadServlet.hpp"

using namespace std;

ServerThread::ServerThread(int initMsgsock) : msgsock(initMsgsock) {
}

void ServerThread::start() {
    pthread_create(&tid, nullptr, run, (void *) msgsock);
}

void *ServerThread::run(void *arg) {
//    DIR *dirp;
    struct direct *d;
    int readBufferLength = 1024;
    char buf1[readBufferLength];
    char buf2[1024];

    int clientsock = static_cast<int>(reinterpret_cast<intptr_t>(arg));

    // TODO: basic structure
//    HttpRequest request (buf2);
//    HttpResponse response (buf2);
//
//    UploadServlet servlet{};
//
//    servlet.doPost(request, response);

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
//    string responseHTML = "<div>";
//    strcpy(buf2, "SOME RESPONSE\r\n");
    if ((readVal = write(clientsock, buf2, 1024)) < 0) {
        perror("writing socket");
    }

//    closedir(dirp);
    return nullptr;
}
