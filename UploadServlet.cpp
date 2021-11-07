

#include <fstream>
#include <sys/dir.h>
#include "UploadServlet.hpp"

void UploadServlet::doGet(HttpRequest request, HttpResponse response) {
    // TODO: return upload html page.

}

void UploadServlet::doPost(HttpRequest request, HttpResponse response) {

    size_t readLength;
    char outBuffer[1024];
    // Need to write to filepart **
    string captionName = request.getParams().find("caption")->second;
    string formDate = request.getParams().find("date")->second;
    //string fileName = request.getBody().find("fileName")->second;

//    if(fileName.empty()) {
//        response.setStatus(302);
//        response.sendRedirect("upload");
//        return;
//    }

    if (formDate.empty()) {
        formDate = "Non Provided";
    }
    if (captionName.empty()) {
        captionName = "No Caption";
    }
    char buf2 = '\0';

    //response.setContentType("text/html");
    string top = " <!DOCTYPE html><html><body><ul>";
    string bottom = "</ul></body></html>";
    //string responseHTML = top + getRecords(path) + bottom;


//   strcpy(&buf2, responseHTML);

    // write buffer to the end.
//    if ((readLength = write(clientsock, outBuffer, 1024)) < 0) {
//        perror("writing socket");
//    }

//    response.setContentType("text/html");



    // TODO: write content images folder to the response as an html list.
//    response.write("");

// TODO: possible path to images folder.
    // ../images

}

std::vector<std::string> UploadServlet::getRecords(char *path) {
        std::vector<std::string> files;
        struct dirent *entry;
        DIR *dir = opendir(path);

        if (dir == nullptr)
        {
            return files;
        }
        while ((entry = readdir(dir)) != nullptr)
        {
            files.emplace_back(entry->d_name);
        }
        closedir(dir);

        return files;
}
