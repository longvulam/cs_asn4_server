#include <cstring>
#include <sys/unistd.h>
#include <string>
#include <iomanip>
#include <filesystem>
#include <iostream>
#include "ServerThread.hpp"
#include "HttpResponse.hpp"
#include "UploadServlet.hpp"
#include "Exceptions.hpp"

using namespace std;

ServerThread::ServerThread(Socket *clientSocket) : Thread(this) {
    this->msgsock = clientSocket;
}

void ServerThread::run() {

    // READING PART
    HttpRequest request(msgsock);
    string output;
    HttpResponse response(output);

    try {
        request.readHeaders();
    } catch (invalid_http_request_format &ex) {
        cerr << ex.what() << endl;
        string errMsg{"Wrong HTTP Request format" + LF + LF};
        writeResponse(errMsg);
        return;
    }

    UploadServlet servlet;
    if (request.getMethod() == "GET") {
        servlet.doGet(request, response);
    }

    if (request.getMethod() == "POST") {
        servlet.doPost(request, response);
    }

    writeResponse(output);
}

void ServerThread::writeResponse(string &output) const {
    output += "\r\n";
    msgsock->sendResponse(output.c_str());
    delete msgsock;
}
