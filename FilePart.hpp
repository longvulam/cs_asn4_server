#pragma once

#include <string>

using namespace std;

static const string TEMP_FOLDER = "../temp/";

class FilePart {
public:
    FilePart() = default;

    FilePart(string fileType, string key, string fileName);

    const string &getFileName() const;

    void setFileName(const string &newVal);

    const string &getKey() const;

    void setKey(const string &newVal);

    const string &getFileType() const;

    void setFileType(const string &newVal);

private:
    string fileName;
    string key;
    string fileType;

};

