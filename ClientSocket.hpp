#pragma once

#include <string>

using namespace std;

class ClientSocket {
public:
    explicit ClientSocket(int sock);

    string getRequest() const;

    void sendResponse(const char *res) const;

    ~ClientSocket();

private:
    int sock;
};

