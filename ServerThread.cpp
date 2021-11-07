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
    char inBuffer[readBufferLength];
    char outBuffer[1024];

    int clientsock = static_cast<int>(reinterpret_cast<intptr_t>(arg));

    // READING PART
    size_t readLength;
    string input;
    do {
        inBuffer[0] = '\0';
        readLength = read(clientsock, inBuffer, readBufferLength);
        if (readLength > 0) {

            for (int i = 0; i < readLength; ++i) {
                input += inBuffer[i];
            }
        }
    } while (readLength == readBufferLength);

    HttpRequest request(input);
    HttpResponse response(outBuffer);

    UploadServlet servlet;

    // TODO: for developing uncomment the one you're working with
//    servlet.doPost(request, response);
//    servlet.doGet(request, response);



    // WRITING PART
    // TODO: this part needs to go to response write
//    buf2[0] = '\0';
//    string responseHTML = "<div>";
//    strcpy(buf2, "SOME RESPONSE\r\n");

    // write buffer to the end.
    if ((readLength = write(clientsock, outBuffer, 1024)) < 0) {
        perror("writing socket");
    }

    return nullptr;
}
