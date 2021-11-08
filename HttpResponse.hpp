
#pragma once

#include <string>
#include <sstream>

using namespace std;

static const string LINE_FEED = "\r\n";


class HttpResponse {
public:

    explicit HttpResponse(string &init);

    HttpResponse(const HttpResponse &response) = default;

    void write(const string &content);

private:

    size_t endOfHeader;
    string *outputStream;
    string contentLengthHeader;
    string body;

    void writeHeader();

    void setContentLength();

};
