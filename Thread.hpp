#pragma once

class Thread {
public:
    explicit Thread(Thread *childThread);

    ~Thread();

    virtual void run() = 0;

    virtual void start();

private:
    void *state;
    Thread *childThread;
};
