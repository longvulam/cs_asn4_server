#include <iomanip>
#include "HttpResponse.hpp"

HttpResponse::HttpResponse(string &init) : outputStream(&init) {
    writeHeader();
}

void HttpResponse::write(const string &content) {
    body += content;
    *outputStream += content;
    setContentLength();
}

void HttpResponse::writeHeader() {

    time_t t = time(nullptr);   // get time now
    tm *now = localtime(&t);
    stringstream buffer;
    buffer << put_time(now, "%a, %d %b %Y %H:%M:%S GMT"); // Mon, 07 Nov 2021 12:60:60 GMT
    string date = buffer.str();

    string responseStr = "HTTP/1.1 200 ok" + LF;
    responseStr += "Date: " + date + LF;
    responseStr += "Server: Custom C++ server" + LF;
    responseStr += "Connection: close" + LF;
    contentTypeHeader = "Content-Type: text/plain";
    responseStr += contentTypeHeader + LF;
    contentLengthHeader = "Content-Length: 0";
    responseStr += contentLengthHeader + LF + LF;
    *outputStream += responseStr;
}

void HttpResponse::setContentLength() {
    string newContentLengthHeader = "Content-Length: " + to_string(body.length());
    size_t indexRes =  outputStream->find(contentLengthHeader);
    if(indexRes != string::npos){
        outputStream->replace(indexRes, contentLengthHeader.length(), newContentLengthHeader);
        contentLengthHeader = newContentLengthHeader;
    }
}

void HttpResponse::setContentType(const string &contentType) {
    string newContentTypeHeader = "Content-Type: " + contentType;
    size_t indexRes =  outputStream->find(contentTypeHeader);
    if(indexRes != string::npos){
        outputStream->replace(indexRes, contentTypeHeader.length(), newContentTypeHeader);
        contentTypeHeader = newContentTypeHeader;
    }
}
