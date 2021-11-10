#include "Thread.hpp"
#include "ServerThread.hpp"
#include <pthread.h>
#include <cstdlib>
#include <cstring>

void *startMethodInThread(void *arg) {
    auto thread = (ServerThread *) arg;
    if (thread == nullptr) {
        return nullptr;
    }
    ServerThread serverThread = *thread;
    serverThread.run();
    pthread_exit(nullptr);
}

Thread::Thread(Thread *childThread) {
    this->state = malloc(sizeof(pthread_t));
    this->childThread = childThread;
}

void Thread::start() {
    pthread_t tid;
    pthread_create(&tid, nullptr, startMethodInThread, (void *) this->childThread);
//    memcpy(this->state, (const void *) &tid, sizeof(pthread_t));
}

Thread::~Thread() {
    free(this->state);
}