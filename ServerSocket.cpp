#include "ServerSocket.hpp"
#include <sys/socket.h>
#include <resolv.h>
#include <cstdio>

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

Socket *ServerSocket::Accept() const {
    sockaddr_in remoteAddr{};
    int addrLen = sizeof(remoteAddr);
    int cSock = accept(sock, (struct sockaddr *) &remoteAddr, &addrLen);
    auto *cs = new Socket(cSock);
    return cs;
}

ServerSocket::~ServerSocket() = default;
