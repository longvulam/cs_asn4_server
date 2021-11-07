
#include <sstream>
#include <iostream>
#include <cstring>
#include <fstream>
#include "HttpRequest.hpp"

static const string BOUNDARY_KEY = "boundary=";
static const char *CONTENT_TYPE_KEY = "Content-Type";
static const char *HEADER_SEPARATOR = " ";
static constexpr char CONTENT_DISPOSITION_SEPARATOR = ';';
static constexpr char HEADER_VALUE_SEPARATOR = ':';
static constexpr char PARAM_PATH_SEPARATOR = '?';
static constexpr char PARAM_VALUE_SEPARATOR = '=';
static constexpr char PARAMS_SEPARATOR = '&';

HttpRequest::HttpRequest(const string &inputBuffer) {
    readHeaders(inputBuffer);
}

void HttpRequest::readHeaders(const string &inputBuffer) {
    stringstream strStream{inputBuffer};
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

    // READING BODY
    readBody(strStream, line);

    // FOR DEBUGGING
    //    for (const auto &param: params) {
    //        cout << param.first << " - " << param.second << endl;
    //    }
    //
    //    for (const auto &header: headers) {
    //        cout << header.first << " - " << header.second << endl;
    //    }
}

void HttpRequest::readBody(stringstream &strStream, string &line) {

    auto boundaryEnd = "--" + boundary + "--";
    // READ ALL BODY PARTS
    while (getline(strStream, line) && line != boundaryEnd) {
        cout << line << endl;

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
        while (getline(ss, valuePart, CONTENT_DISPOSITION_SEPARATOR)) {
            string trimmed = trim(valuePart);
            if (!isFile || trimmed == "form-data") {
                continue;
            }

            stringstream valueReaderStream{trimmed};
            string temp;
            getline(valueReaderStream, temp, '=');
            string key = temp;
            getline(valueReaderStream, temp, '=');
            string value = temp;

            if (trim(key) == "filename") {
                fileName = value;
                filePart.setFileName(fileName);
            } else if (trim(key) == "name") {
                filePart.setKey(value);
            }
        }

        // Content Type
        getline(strStream, line);
        if (line.find(CONTENT_TYPE_KEY) != string::npos) {
            string contentTypeVal = line.substr(strlen(CONTENT_TYPE_KEY) + 2);
            string replacedTypeVal = replaceAll(contentTypeVal, '\r');
            if (!replacedTypeVal.starts_with("image/")) {
                continue;
            }
            filePart.setFileType(replacedTypeVal);
            string replacedFilename = replaceAll(fileName, '\r');
            replacedFilename = replaceAll(replacedFilename, '\"');
            readFile(strStream, line, replacedFilename);

            fileParts.insert(make_pair(filePart.getKey(), filePart));
        }


        // Content Description (WE DONT NEED TO HANDLE THAT, I THINK)
        getline(strStream, line);


        auto charIndex = line.find(boundaryEnd);
        if (charIndex != string::npos) {
            // END OF BODY
            break;
        }

        while (getline(strStream, line) && line != ("--" + boundary)) {
            cout << line << endl;
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
        while (getline(strStream, line)) {
            line += "\n";
            outputFile.write(line.c_str(), sizeof(char) * line.length());
        };
    }
    outputFile.close();
}


bool HttpRequest::getIsMultipart() {
    bool isMultipart;
    auto resIt = headers.find("Content-Type");
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
        params.insert(make_pair(paramKey, paramValue));
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

FilePart HttpRequest::getFilePart(const string &key) const {
    auto resIt = fileParts.find(key);
    if(resIt == fileParts.end()){
        throw "Invalid key at getFilePart: " + key;
    }
    return resIt->second;
}
