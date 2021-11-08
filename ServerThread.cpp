#include <cstring>
#include <sys/unistd.h>
#include <sys/dir.h>
#include <pthread.h>
#include <exception>
#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include "ServerThread.hpp"
#include "HttpResponse.hpp"
#include "UploadServlet.hpp"

using namespace std;

ServerThread::ServerThread(int initMsgsock) : msgsock(initMsgsock) {
}

void ServerThread::start() {
    pthread_create(&tid, nullptr, run, (void *) msgsock);
}

void *ServerThread::run(void *arg) {
//    DIR *dirp;
    struct direct *d;
    int readBufferLength = 1024;
    char inBuffer[readBufferLength];
    char outBuffer[1024];

    int clientsock = static_cast<int>(reinterpret_cast<intptr_t>(arg));

    // READING PART
    size_t readLength;
    string input;
    do {
        inBuffer[0] = '\0';
        readLength = read(clientsock, inBuffer, readBufferLength);
        if (readLength > 0) {

            for (int i = 0; i < readLength; ++i) {
                input += inBuffer[i];
            }
        }
    } while (readLength == readBufferLength);

    HttpRequest request(input);
    string output;
    HttpResponse response(output);

    UploadServlet servlet;
    if (request.getMethod() == "GET") {
        servlet.doGet(request, response);
    }

    if (request.getMethod() == "POST") {
        servlet.doPost(request, response);
    }

//    char buf2 = '\0';

    string LF = "\r\n";
    time_t timetoday;
    time(&timetoday);
    string string1 = asctime(localtime(&timetoday));

    string content =
            "<!DOCTYPE html>\r\n"
            "<html>\r\n<head>\r\n<title>File Upload Form</title>\r\n</head>\r\n"
            "<body>\r\n<h1>Upload file</h1>\r\n"
            "<form method=\"POST\" action=\"**REDIRECT**\"enctype=\"multipart/form-data\">\r\n"
            "<input type=\"file\" name=\"fileName\"/><br/><br/>\r\n"
            "Caption: <input type=\"text\" name=\"caption\"<br/><br/>\r\n<br />\n"
            "Date: <input type=\"date\" name=\"date\"<br/><br/>\r\n<br/>\n"
            "<input type=\"submit\" value=\"Submit\"/>\r\n</form>\r\n";

    namespace fs = std::filesystem;
    string path = IMAGES_FOLDER;
    for(const auto& entry : fs::directory_iterator(path)) {
        content += entry.path();
        content += "<br/>";
    }

    content += "</body>\r\n</html>\r\n";

    size_t contentLength = content.size() * sizeof(char);

//    auto now = chrono::system_clock::now();
//    responseStr += "Date: " + ctime(&now);

    string responseStr = "HTTP/1.1 200 ok" + LF;
    responseStr += "Date: Sun, 7 Nov 2021 15:11:00 GMT" + LF;
    responseStr += "Server: Custom C++ server" + LF;
    responseStr += "Connection: close" + LF;
    responseStr += "Content-Length: " + to_string(contentLength) + LF;
    responseStr += "Content-Type: text/html" + LF;
    responseStr += LF;
    responseStr += content;

    cout << responseStr << endl;
    const char *asCharArray = responseStr.c_str();
    strcpy(outBuffer, asCharArray);

    // write buffer to the end.
    if ((readLength = write(clientsock, outBuffer, 1024)) < 0) {
        perror("writing socket");
    }

    return nullptr;
}
