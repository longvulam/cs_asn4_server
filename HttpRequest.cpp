#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "HttpRequest.hpp"
#include "Exceptions.hpp"

static const char BOUNDARY_KEY[] = "boundary=";
static const char *CONTENT_TYPE_KEY = "Content-Type";
static const char *HEADER_SEPARATOR = " ";
static constexpr char CONTENT_DISPOSITION_SEPARATOR = ';';
static constexpr char HEADER_VALUE_SEPARATOR = ':';
static constexpr char PARAM_PATH_SEPARATOR = '?';
static constexpr char PARAM_VALUE_SEPARATOR = '=';
static constexpr char PARAMS_SEPARATOR = '&';

HttpRequest::HttpRequest(ClientSocket *clientsock) {
    rawClientRequest = clientsock->getRequest();
}

void HttpRequest::readHeaders() {
    stringstream strStream{rawClientRequest};
    string line;
    getline(strStream, line);

    char *asCharArr = const_cast<char *>(line.c_str());
    char *token = strtok(asCharArr, HEADER_SEPARATOR);
    if (token != nullptr) {
        method = token;
        token = strtok(nullptr, HEADER_SEPARATOR);
    }

    if (token != nullptr) {
        path = token;
        if (!path.starts_with("/")) {
            throw invalid_http_request_format("request path");
        }

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
            boundary = line.substr(charIndex + strlen(BOUNDARY_KEY));
        }

        size_t pos = line.find(':');
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        headers.insert(make_pair(toLower(key), value));
    }

    // READING BODY
    readBody(strStream, line);

    // FOR DEBUGGING
//        for (const auto &param: params) {
//            cout << param.first << " - " << param.second << endl;
//        }

//        for (const auto &header: headers) {
//            cout << header.first << " - " << header.second << endl;
//        }
}

void HttpRequest::readBody(stringstream &strStream, string &line) {

    auto boundaryEnd = "--" + replaceAll(boundary, '\r') + "--";
    // READ ALL BODY PARTS
    while (replaceAll(line, '\r') != boundaryEnd && getline(strStream, line)) {

        string fileName;
        FilePart filePart;

        // Content disposition
        if (!line.starts_with("Content-Disposition")) {
            cerr << "Invalid format, should be Content-Disposition, but found:" + line << endl;
        }

        bool isFile = line.find("filename") != string::npos;

        // GET DATA AND STORE IT
        stringstream ss{line};
        string contentDispValue;
        getline(ss, contentDispValue, HEADER_VALUE_SEPARATOR);
        getline(ss, contentDispValue, HEADER_VALUE_SEPARATOR);

        ss.clear();
        ss.str(contentDispValue);


        string valuePart;
        pair<string, string> paramPair;
        while (getline(ss, valuePart, CONTENT_DISPOSITION_SEPARATOR)) {
            string trimmed = trim(valuePart);
            if (trimmed == "form-data") {
                continue;
            }

            stringstream valueReaderStream{trimmed};
            string temp;
            getline(valueReaderStream, temp, '=');
            string key = temp;
            getline(valueReaderStream, temp, '=');
            string value = temp;

            string trimmedKey = trim(key);
            trimmedKey = replaceAll(trimmedKey, '\r');
            trimmedKey = replaceAll(trimmedKey, '\"');

            string trimmedValue = replaceAll(value, '\r');
            trimmedValue = replaceAll(trimmedValue, '\"');
            if (trimmedKey == "filename") {
                fileName = trimmedValue;
                filePart.setFileName(fileName);
            } else if (trimmedKey == "name") {
                if (isFile) {
                    filePart.setKey(trimmedValue);
                } else {
                    paramPair.first = trimmedValue;
                }
            }
        }

        // Content Type
        getline(strStream, line);
        if (line.find(CONTENT_TYPE_KEY) != string::npos) {
            string contentTypeVal = line.substr(strlen(CONTENT_TYPE_KEY) + 2);
            string replacedTypeVal = replaceAll(contentTypeVal, '\r');
            if (!replacedTypeVal.starts_with(IMAGES_FOLDER)) {
                continue;
            }
            readFile(strStream, line, fileName);

            ss.clear();
            ss.str(fileName);
            string ext;
            getline(ss, ext, '.');
            getline(ss, ext, '.');
            filePart.setFileType(ext);
            fileParts.insert(make_pair(filePart.getKey(), filePart));
        } else {
            // value of the body param
            getline(strStream, line);

            const string &trimmedValue = replaceAll(line, '\r');
            paramPair.second = trimmedValue;
            bodyParams.insert(paramPair);

            // next line is a boundary
            getline(strStream, line);
        }
    }
}

string HttpRequest::replaceAll(const string &value, char c) {
    string copy = value;
    auto iterator = remove(copy.begin(), copy.end(), c);
    copy.erase(iterator, copy.end());
    return copy;
}

string HttpRequest::trim(const string &valuePart) {
    string res = valuePart;
    res.erase(res.find_last_not_of(' ') + 1);
    res.erase(0, res.find_first_not_of(' '));
    return res;
}

void HttpRequest::readFile(stringstream &strStream, string &line, const string &fileName) {
    // THROW AWAY ROW
    getline(strStream, line);

    bool isMultipart = getIsMultipart();
    fstream outputFile{TEMP_FOLDER + fileName, ios_base::out};
    if (isMultipart && outputFile.is_open()) {
        while (getline(strStream, line) && line != ("--" + boundary)) {
            line += "\n";
            outputFile.write(line.c_str(), sizeof(char) * line.length());
        };
    }
    outputFile.close();
}


bool HttpRequest::getIsMultipart() {
    bool isMultipart;
    auto resIt = headers.find("content-type");
    if (resIt != headers.end()) {
        unsigned long searchRes = resIt->second.find("multipart/form-data");
        isMultipart = searchRes != string::npos;
    }
    return isMultipart;
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
        params.insert(make_pair(toLower(paramKey), paramValue));
    }
}

string HttpRequest::urlDecode(string &SRC) {
    string ret;
    char ch;
    unsigned int ii;
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

FilePart *HttpRequest::getFilePart(const string &key) const {
    auto resIt = fileParts.find(key);
    if (resIt == fileParts.end()) {
        cerr << "Invalid key at getFilePart: " + key;
        return nullptr;
    }
    return new FilePart{resIt->second};
}


string HttpRequest::getParam(const string &key) {
    auto resIt = params.find(toLower(key));
    if (resIt == params.end()) {
        return "";
    }

    return resIt->second;
}

string HttpRequest::getBodyParam(const string &key) {
    auto resIt = bodyParams.find(toLower(key));
    if (resIt == bodyParams.end()) {
        return "";
    }

    return resIt->second;
}

bool HttpRequest::isBrowserRequest() {
    bool isBrowser = false;
    auto headerIt = headers.find("user-agent");
    if (headerIt != headers.end()) {
        string &userAgent = headerIt->second;
        auto it = browserUserAgents->begin();
        while (it != browserUserAgents->end() && !isBrowser) {
            if (userAgent.find(*it) != string::npos) {
                isBrowser = true;
            }
        }
    }
    return isBrowser;
}

inline string HttpRequest::toLower(const string &input) {
    string res;
    for (char c: input) {
        res += (char) tolower(c);
    }
    return res;
}

map<string, string> HttpRequest::getParams() {
    return params;
}

map<string, string> HttpRequest::getHeaders() {
    return headers;
}

const string &HttpRequest::getMethod() const {
    return method;
}

const string &HttpRequest::getProtocolVersion() const {
    return protocolVersion;
}
