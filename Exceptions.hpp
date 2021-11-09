
#pragma once

#include <string>
#include <utility>

using namespace std;

class invalid_http_request_format : public exception {
private:
    string target;
public:
    explicit invalid_http_request_format(string target) : target(move(target)) {
    }

    const char *what() const noexcept override {
        return target.c_str();
    };
};
