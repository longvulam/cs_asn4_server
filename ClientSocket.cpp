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

    if ((readLength = read(sock, readBuffer, readBufferLength)) < 0) {
        perror("reading socket");
    }

    for (size_t i = 0; i < readLength; ++i) {
        rawClientRequest += readBuffer[i];
    }


    string bodyIndicator = "boundary=";
    size_t boundaryKeyIndex = rawClientRequest.find(bodyIndicator);
    if (boundaryKeyIndex == string::npos) {
        cout << "NO BODY" << endl;
        cout << rawClientRequest << endl;
        return rawClientRequest;
    }

    string temp = rawClientRequest.substr(boundaryKeyIndex);
    size_t lineEndIndex = temp.find("\r\n");
    string boundaryKey = rawClientRequest.substr(boundaryKeyIndex + bodyIndicator.length(), lineEndIndex - bodyIndicator.length());
    string boundaryKeyEnd = "--" + boundaryKey + "--";

    string currentBufferContent;
    while (currentBufferContent.find(boundaryKeyEnd) == string::npos) {
        readBuffer[0] = '\0';
        if ((readLength = read(sock, readBuffer, readBufferLength)) < 0) {
            perror("reading socket");
        } else {
            for (size_t i = 0; i < readLength; ++i) {
                currentBufferContent += readBuffer[i];
            }
            rawClientRequest += currentBufferContent;
        }
    }
    cout << "HAS BODY" << endl;
    cout << rawClientRequest << endl;
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
