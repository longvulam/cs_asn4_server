#include "ServerSocket.hpp"
#include <sys/socket.h>
#include <resolv.h>
#include <cstdio>
#include <iostream>
#include <sys/unistd.h>

ServerSocket::ServerSocket(int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("opening stream socket");
    }

    struct sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *) &server, sizeof server) < 0) {
        perror("binding stream socket");
    }
    listen(sock, 5);
}

ClientSocket *ServerSocket::Accept() const {
    sockaddr_in remoteAddr{};
    int addrLen = sizeof(remoteAddr);
    int cSock = accept(sock, (struct sockaddr *) &remoteAddr, &addrLen);

//    cout << "Current Socket: " << cSock << endl;
//    sleep(5);

    auto *cs = new ClientSocket(cSock);
    return cs;
}

ServerSocket::~ServerSocket() = default;
