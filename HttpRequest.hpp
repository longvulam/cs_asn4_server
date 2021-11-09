#pragma once

#include <string>
#include <vector>
#include <map>
#include "FilePart.hpp"
#include "Socket.hpp"

using namespace std;

const static string browserUserAgents[] = {
        "Mozilla",
        "Chrome"
        "Firefox",
        "Safari",
};

class HttpRequest {
private:

    string method;
    string path;
    string protocolVersion;
    string boundary;
    string rawClientRequest;

    map<string, string> params{};
    map<string, string> bodyParams{};
    map<string, string> headers{};
    map<string, FilePart> fileParts{};

    static string urlDecode(string &SRC);

    static string toLower(const string &input);

    static string trim(const string &valuePart);

    static string replaceAll(const string &value, char c);

public:

    explicit HttpRequest(Socket *inputBuffer);

    void readHeaders();

    void readParams(const string &pathPart);

    void readBody(stringstream &strStream, string &line);

    bool getIsMultipart();

    void readFile(stringstream &strStream, string &line, const string &fileName);

    FilePart *getFilePart(const string &key) const;

    const string &getMethod() const;

    const string &getProtocolVersion() const;

    string getBodyParam(const string &key);

    bool isBrowserRequest();


    map<string, string> getParams();

    map<string, string> getHeaders();


    string getParam(const string &key);
};


