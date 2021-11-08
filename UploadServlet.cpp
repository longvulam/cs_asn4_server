
#include <filesystem>
#include <fstream>
#include <sys/dir.h>
#include "UploadServlet.hpp"

void UploadServlet::doGet(HttpRequest request, HttpResponse response) {
    string content =
            "<!DOCTYPE html>\r\n"
            "<html>\r\n<head>\r\n<title>File Upload Form</title>\r\n</head>\r\n"
            "<body>\r\n<h1>Upload file</h1>\r\n"
            "<form method=\"POST\" action=\"/\"enctype=\"multipart/form-data\">\r\n"
            "<input type=\"file\" name=\"fileName\"/><br/><br/>\r\n"
            "Caption: <input type=\"text\" name=\"caption\"<br/><br/>\r\n<br />\n"
            "Date: <input type=\"date\" name=\"date\"<br/><br/>\r\n<br/>\n"
            "<input type=\"submit\" value=\"Submit\"/>\r\n"
            "</form>\r\n";

    content += "</body>\r\n</html>\r\n";

    response.write(content);

}

void UploadServlet::doPost(HttpRequest request, HttpResponse response) {

    // Need to write to filepart **

//    if(fileName.empty()) {
//        response.setStatus(302);
//        response.sendRedirect("upload");
//        return;
//    }

    string caption = request.getBodyParam("caption");
    string formDate = request.getBodyParam("date");

    FilePart *filePart = request.getFilePart("fileName");
    if (filePart != nullptr && !caption.empty() && !formDate.empty()) {
        string path{IMAGES_FOLDER};
        path += formDate + "_" + caption + "." + filePart->getFileType();
        filePart->write(path);
        delete filePart;
    }

    string content =
            "<!DOCTYPE html>\r\n"
            "<html>\r\n<head>\r\n<title>Files in upload folder</title>\r\n</head>\r\n"
            "<body>\r\n"
            "<h1>Files in upload folder</h1>\r\n"
            "<ul>\r\n";

    namespace fs = std::filesystem;
    string path = IMAGES_FOLDER;
    for (const auto &entry: fs::directory_iterator(path)) {
        content += "<li>";
        content += entry.path();
        content += "</li>\r\n";
    }

    content += "</ul>\r\n"
               "</body>\r\n"
               "</html>\r\n";

    response.write(content);

}

std::vector<std::string> UploadServlet::getRecords(char *path) {
    std::vector<std::string> files;
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == nullptr) {
        return files;
    }
    while ((entry = readdir(dir)) != nullptr) {
        files.emplace_back(entry->d_name);
    }
    closedir(dir);

    return files;
}
