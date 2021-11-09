#pragma once

#include <string>

using namespace std;

class Socket {
public:
    explicit Socket(int sock);

    string getRequest() const;

    void sendResponse(const char *res) const;

    ~Socket();

private:
    int sock;
};

