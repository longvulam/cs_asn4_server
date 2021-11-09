
#pragma once

#include "Thread.hpp"
#include "Socket.hpp"

class ServerThread : public Thread {
public:

    explicit ServerThread(Socket *clientSocket);

    void run() override;

private:
    Socket *msgsock;
};


