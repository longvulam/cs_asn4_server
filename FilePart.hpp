#pragma once

#include <string>

using namespace std;

static const char TEMP_FOLDER[] = "temp/";
static const char IMAGES_FOLDER[] = "image/";

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

    void write(const string &basicString);

private:
    string fileName;
    string key;
    string fileType;

};

