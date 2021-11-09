#include "Socket.hpp"
#include <unistd.h>
#include <cstdio>
#include <cstring>


Socket::Socket(int sock) {
    this->sock = sock;
}

string Socket::getRequest() const {
    const int readBufferLength = 1024;
    char readBuffer[readBufferLength];

    // READING PART
    size_t readLength = readBufferLength;
    string rawClientRequest;
    while (readLength == readBufferLength) {
        readBuffer[0] = '\0';
        if ((readLength = read(sock, readBuffer, readBufferLength)) < 0) {
            perror("reading socket");
        } else {
            for (size_t i = 0; i < readLength; ++i) {
                rawClientRequest += readBuffer[i];
            }
        }
    }
    return rawClientRequest;
}

void Socket::sendResponse(const char *res) const {
    int rval;
    if ((rval = write(sock, res, strlen(res))) < 0) {
        perror("writing socket");
    } else {
        printf("%s\n", res);
    }
}

Socket::~Socket() {
    close(sock);
}
