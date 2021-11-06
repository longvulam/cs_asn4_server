
#pragma once


#include <cstdio>

class ServerThread {
private:
    int msgsock;
    pthread_t tid{};
    static void* run(void *arg);

public:
    ServerThread() = default;
    explicit ServerThread(int initMsgsock);
    void start();
};


