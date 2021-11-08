#include <cstring>
#include <sys/unistd.h>
#include <sys/dir.h>
#include <pthread.h>
#include <string>
#include <iomanip>
#include <filesystem>
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
    string output;
    HttpResponse response(output);

    UploadServlet servlet;
    if (request.getMethod() == "GET") {
        servlet.doGet(request, response);
    }

    if (request.getMethod() == "POST") {
        servlet.doPost(request, response);
    }

    char outBuffer[1024];
    strcpy(outBuffer, output.c_str());

    // write buffer to the end.
    if ((readLength = write(clientsock, outBuffer, 1024)) < 0) {
        perror("writing socket");
    }

    return nullptr;
}
