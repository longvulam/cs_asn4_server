

#pragma once

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"


class ServletBase {
public:
    virtual void doGet(HttpRequest request, HttpResponse response) = 0;

    virtual void doPost(HttpRequest request, HttpResponse response) = 0;

    ~ServletBase()= default;
};

