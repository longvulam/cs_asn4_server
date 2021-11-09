#include <sys/socket.h>
#include <resolv.h>
#include <sys/stat.h>
#include "ServletBase.hpp"
#include "ServerThread.hpp"
#include "ServerSocket.hpp"

int main() {

    mkdir(TEMP_FOLDER, S_IRWXU);
    mkdir(IMAGES_FOLDER, S_IRWXU);

    ServerSocket serverSocket{8888};
    while (1) {
        ServerThread serverThread{serverSocket.Accept()};
        serverThread.start();
    }

    return 0;
}
