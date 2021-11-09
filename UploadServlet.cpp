
#include <filesystem>
#include <fstream>
#include <set>
#include "UploadServlet.hpp"

void UploadServlet::doGet(HttpRequest request, HttpResponse response) {
    response.setContentType("text/html");

    string content =
            "<!DOCTYPE html>" + LF +
            "<html>" + LF + "<head>" + LF + "<title>File Upload Form</title>" + LF + "</head>" + LF +
            "<body>" + LF + "<h1>Upload file</h1>" + LF +
            "<form method=\"POST\" action=\"/\"enctype=\"multipart/form-data\">" + LF +
            "<input type=\"file\" name=\"fileName\"/><br/><br/>" + LF +
            "Caption: <input type=\"text\" name=\"caption\"<br/><br/>" + LF + "<br />\n" +
            "Date: <input type=\"date\" name=\"date\"<br/><br/>" + LF +
            "<br/>" + LF +
            "<input type=\"submit\" value=\"Submit\"/>" + LF +
            "</form>" + LF;

    content += "</body>" + LF + "</html>" + LF;

    response.write(content);
}

void UploadServlet::doPost(HttpRequest request, HttpResponse response) {

    string caption = request.getBodyParam("caption");
    string formDate = request.getBodyParam("date");

    FilePart *filePart = request.getFilePart("fileName");

    if (filePart != nullptr && !caption.empty() && !formDate.empty()) {
        string path{IMAGES_FOLDER};
        path += formDate + "_" + caption + "." + filePart->getFileType();
        filePart->write(path);
        delete filePart;
    }

    string path = IMAGES_FOLDER;
    set<string> images;
    for (const auto &entry: filesystem::directory_iterator(path)) {
        images.insert(entry.path());
    }

    string content;
    bool isBrowser = request.isBrowserRequest();
    if (isBrowser) {
        response.setContentType("text/html");
        content += "<!DOCTYPE html>" + LF +
                   "<html>" + LF + "<head>" + LF + "<title>Files in upload folder</title>" + LF + "</head>" + LF +
                   "<body>" + LF +
                   "<h1>Files in upload folder</h1>" + LF +
                   "<ul>" + LF;

        for (const auto &imgPath: images) {
            content += "<li>";
            content += imgPath;
            content += "</li>" + LF;
        }

        content += "</ul>" + LF +
                   "</body>" + LF +
                   "</html>" + LF;
    } else {
        response.setContentType("application/json");

        content += "{"
                   "\"images\": [";
        for (const auto &fileName: images) {
            content += "\"" + fileName + "\",";
        }
        content.erase(content.end() - 1);
        content += "]"
                   "}" + LF;
    }

    response.write(content);
}
