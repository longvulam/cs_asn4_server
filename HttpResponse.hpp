
#pragma once

#include <string>
#include <sstream>

using namespace std;

static const string LF {"\r\n"};

class HttpResponse {
public:

    explicit HttpResponse(string &init);

    HttpResponse(const HttpResponse &response) = default;

    void write(const string &content);

    void setContentType(const string &contentType);

private:

    string *outputStream;
    string contentLengthHeader;
    string contentTypeHeader;
    string body;

    void writeHeader();

    void setContentLength();
};

