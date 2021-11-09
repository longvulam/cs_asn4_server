
#pragma once

#include "Thread.hpp"
#include "ClientSocket.hpp"

class ServerThread final : public Thread {
public:

    explicit ServerThread(ClientSocket *clientSocket);

    void run() override;

private:
    ClientSocket *msgsock;

    void writeResponseAndCloseSocket(string &output) const;
};


