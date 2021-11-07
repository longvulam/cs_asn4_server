
#include <sstream>
#include <iostream>
#include <cstring>
#include "HttpRequest.hpp"

static const string BOUNDARY_KEY = "boundary=";
static constexpr char PARAM_PATH_SEPARATOR = '?';
static constexpr char PARAM_VALUE_SEPARATOR = '=';
static constexpr char PARAMS_SEPARATOR = '&';
static constexpr char HEADER_SEPARATOR = ' ';

HttpRequest::HttpRequest(const string &inputBuffer) {
    readHeaders(inputBuffer);
}

void HttpRequest::readHeaders(const string &inputBuffer) {
    stringstream strStream{inputBuffer};
    string line;
    getline(strStream, line);

    char *asCharArr = const_cast<char *>(line.c_str());
    char *token = strtok(asCharArr, " ");
    if (token != nullptr) {
        method = token;
        token = strtok(nullptr, " ");
    }

    if (token != nullptr) {
        path = token;

        // GET PARAM PART OF THE URL PATH
        stringstream pathStream{path};
        string pathPart;
        getline(pathStream, pathPart, PARAM_PATH_SEPARATOR);
        getline(pathStream, pathPart, PARAM_PATH_SEPARATOR);

        // READING PARAMS
        readParams(pathPart);

        token = strtok(nullptr, " ");
    }

    if (token != nullptr) {
        protocolVersion = token;
    }

    // READING HEADERS
    while (getline(strStream, line)) {
        auto charIndex = line.find(boundary);
        if (!boundary.empty() && charIndex != string::npos) {
            break;
        }

        charIndex = line.find(BOUNDARY_KEY);
        if (charIndex != string::npos) {
            boundary = line.substr(charIndex + BOUNDARY_KEY.length());
        }

        size_t pos = line.find(':');
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        headers.insert(make_pair(key, value));
    }

    bool isMultipart;
    auto resIt = headers.find("Content-Type");
    if(resIt != headers.end()){
        isMultipart = resIt->second.find("multipart/form-data") != string::npos;
    }

    auto boundaryEnd = boundary + "--";
    // READING BODY
    while (getline(strStream, line)) {
        cout << line << endl;

        auto charIndex = line.find(boundaryEnd);
        if(charIndex != string::npos){
            // END OF BODY
            break;
        }
    }

//    for (const auto &param: params) {
//        cout << param.first << " - " << param.second << endl;
//    }
//
//    for (const auto &header: headers) {
//        cout << header.first << " - " << header.second << endl;
//    }
}

void HttpRequest::readParams(const string &pathPart) {
    stringstream paramsStream{pathPart};
    string paramStr;
    while (getline(paramsStream, paramStr, PARAMS_SEPARATOR)) {

        stringstream paramPartsStream{paramStr};
        string paramPart;
        getline(paramPartsStream, paramPart, PARAM_VALUE_SEPARATOR);

        string paramKey = paramPart;
        getline(paramPartsStream, paramPart, PARAM_VALUE_SEPARATOR);

        string paramValue = urlDecode(paramPart);
        params.insert(make_pair(paramKey, paramValue));
    }
}

void HttpRequest::readBody(const string &bodyStr) {
    stringstream bodyStream{bodyStr};
    string line;
    while (getline(bodyStream, line)) {
        cout << line << endl;
    }
}

string HttpRequest::urlDecode(string &SRC) {
    string ret;
    char ch;
    int ii;
    for (size_t i = 0; i < SRC.length(); i++) {
        if (int(SRC[i]) == '%') {
            sscanf(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        } else {
            ret += SRC[i];
        }
    }
    return (ret);
}

map<string, string> HttpRequest::getParams() {
    return params;
}

map<string, string> HttpRequest::getHeaders() {
    return headers;
}
