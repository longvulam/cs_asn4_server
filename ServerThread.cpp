#include <cstring>
#include <sys/unistd.h>
#include <sys/dir.h>
#include <pthread.h>
#include <exception>
#include <string>
#include "ServerThread.hpp"
#include "HttpResponse.hpp"
#include "UploadServlet.hpp"

using namespace std;
+
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
        readLength = read(clientsock, inBuffer, readBufferLength);
        if (readLength > 0) {
            input += inBuffer;
        }
    } while (readLength == readBufferLength);

    HttpRequest request(input);
    HttpResponse response(outBuffer);

    UploadServlet servlet;

    // TODO: for developing uncomment the one you're working with
//    servlet.doPost(request, response);
//    servlet.doGet(request, response);



    // WRITING PART
    // TODO: this part needs to go to response write
    //doGet?
//    char buf2 = '\0';
//    char responseHTML[1024] = "<!DOCTYPE html>\r\n"
//    "<html>\r\n<head>\r\n<title>File Upload Form</title>\r\n</head>\r\n"
//    "<body>\r\n<h1>Upload file</h1>\r\n"
//    "<form method=\"POST\" action=\"**REDIRECT**\"enctype=\"multipart/form-data\">\r\n"
//                                                          "<input type=\"file\" name=\"fileName\"/><br/><br/>\r\n"
//                                                          "Caption: <input type=\"text\" name=\"caption\"<br/><br/>\r\n<br />\n"
//                                                          "Date: <input type=\"date\" name=\"date\"<br/><br/>\r\n<br/>\n"
//                                                          "<input type=\"submit\" value=\"Submit\"/>\r\n</form>\r\n</body>\r\n</html>\r\n";
//    response.setContentType("text/html");
//    strcpy(&buf2, responseHTML);

    // write buffer to the end.
    if ((readLength = write(clientsock, outBuffer, 1024)) < 0) {
        perror("writing socket");
    }

    return nullptr;
}
