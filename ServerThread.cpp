#include <cstring>
#include <sys/unistd.h>
#include <string>
#include <iomanip>
#include <filesystem>
#include "ServerThread.hpp"
#include "HttpResponse.hpp"
#include "UploadServlet.hpp"

using namespace std;

ServerThread::ServerThread(Socket *clientSocket) : Thread(this) {
    this->msgsock = clientSocket;
}

void ServerThread::run() {

    // READING PART
    HttpRequest request(msgsock);
    string output;
    HttpResponse response(output);

    UploadServlet servlet;
    if (request.getMethod() == "GET") {
        servlet.doGet(request, response);
    }

    if (request.getMethod() == "POST") {
        servlet.doPost(request, response);
    }

    output += "\r\n";
    msgsock->sendResponse(output.c_str());
    delete msgsock;
}
