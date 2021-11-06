#pragma once

#include "Servlet.hpp"

class UploadServlet : public Servlet {

public:

    void doGet(HttpRequest request, HttpResponse response) override;

    void doPost(HttpRequest request, HttpResponse response) override;
};