#include "ClientSocket.hpp"
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>


ClientSocket::ClientSocket(int sock) {
    this->sock = sock;
}

string ClientSocket::getRequest() const {
    const int readBufferLength = 1024;
    char readBuffer[readBufferLength];

    // READING PART
    size_t readLength;
    string rawClientRequest;
    bool firstRead = true;
    while (readLength == readBufferLength || firstRead ) {
        readBuffer[0] = '\0';
        if ((readLength = read(sock, readBuffer, readBufferLength)) < 0) {
            perror("reading socket");
        } else {
            for (size_t i = 0; i < readLength; ++i) {
                rawClientRequest += readBuffer[i];
            }
        }
        firstRead = false;
    }
//    cout << rawClientRequest << endl;
    return rawClientRequest;
}

void ClientSocket::sendResponse(const char *res) const {
    int rval;
    if ((rval = write(sock, res, strlen(res))) < 0) {
        perror("writing socket");
    } else {
        printf("%s\n", res);
    }
}

ClientSocket::~ClientSocket() {
    close(sock);
}
