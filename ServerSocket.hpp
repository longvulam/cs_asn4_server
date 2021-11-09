#pragma once

#include "Socket.hpp"

class ServerSocket {
public:
    explicit ServerSocket(int port);

    Socket *Accept() const;

    ~ServerSocket();

private:
    int sock;
};

