#pragma once

#include "Servlet.hpp"

class UploadServlet : public Servlet {

private:
    std::vector<std::string> getRecords(char *path);

public:

    void doGet(HttpRequest request, HttpResponse response) override;

    void doPost(HttpRequest request, HttpResponse response) override;
};