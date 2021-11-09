#pragma once

#include "ServletBase.hpp"

class UploadServlet : public ServletBase {
public:

    void doGet(HttpRequest request, HttpResponse response) override;

    void doPost(HttpRequest request, HttpResponse response) override;
};