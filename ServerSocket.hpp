#pragma once

#include "ClientSocket.hpp"

class ServerSocket {
public:
    explicit ServerSocket(int port);

    ClientSocket *Accept() const;

    ~ServerSocket();

private:
    int sock;
};

