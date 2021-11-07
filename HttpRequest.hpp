
#pragma once
#include <string>
#include <vector>
#include <map>


using namespace std;

class HttpRequest {
private:
    string method;
    string path;
    string protocolVersion;
    string boundary;

    map<string, string> params{};
    map<string, string> headers{};

    static string urlDecode(string &SRC);
public:

    explicit HttpRequest(const string& inputBuffer);

    void readHeaders(const string &inputBuffer);

    void readParams(const string &pathPart);

    void readBody(const string& bodyStr);
};


