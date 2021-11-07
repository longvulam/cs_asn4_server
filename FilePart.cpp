
#include "FilePart.hpp"

#include <utility>

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
