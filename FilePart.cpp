
#include "FilePart.hpp"

#include <utility>
#include <fstream>

FilePart::FilePart(string fileType, string key, string fileName) : fileName(std::move(fileName)), key(std::move(key)), fileType(std::move(fileType)) {

}

const string &FilePart::getFileName() const {
    return fileName;
}

void FilePart::setFileName(const string &newVal) {
    FilePart::fileName = newVal;
}

const string &FilePart::getKey() const {
    return key;
}

void FilePart::setKey(const string &newVal) {
    FilePart::key = newVal;
}

const string &FilePart::getFileType() const {
    return fileType;
}

void FilePart::setFileType(const string &newVal) {
    FilePart::fileType = newVal;
}

void FilePart::write(const string& filePath) {

    string tempFilePath {TEMP_FOLDER};
    tempFilePath += fileName;
    ifstream ifs{tempFilePath, ios::binary};

    char buffer[1024]; // create a buffer
    ofstream outf;
    outf.open(filePath, ios::binary);

    while (ifs.read(buffer, sizeof(buffer)))
    {
        outf.write(buffer, ifs.gcount());
    }

    outf.write(buffer, ifs.gcount());
    outf.close();
    ifs.close();
}
