
#pragma once

#include <string>
#include <vector>
#include <map>
#include "FilePart.hpp"

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

    map<string, string> params{};
    map<string, string> bodyParams{};
    map<string, string> headers{};
    map<string, FilePart> fileParts{};

    static string urlDecode(string &SRC);

    void readHeaders(const string &inputBuffer);

    string toLower(const string &input);

public:

    explicit HttpRequest(const string &inputBuffer);

    void readParams(const string &pathPart);

    void readBody(stringstream &strStream, string &line);

    bool getIsMultipart();

    void readFile(stringstream &strStream, string &line, const string &fileName);

    static string trim(const string &valuePart);

    FilePart * getFilePart(const string& key) const;

    static string replaceAll(const string &value, char c);

    map<string, string> getParams();

    map<string, string> getHeaders();

    const string &getMethod() const;

    string getParam(const string &key);

    string getBodyParam(const string &key);

    bool isBrowserRequest();

};


