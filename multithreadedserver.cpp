#include <sys/socket.h>
#include <resolv.h>
#include "Servlet.hpp"
#include "ServerThread.hpp"

static void *doInServerThread(void *);

int main() {
    int sock;
    struct sockaddr_in server;
    int msgsock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("opening stream socket");
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(8888);

    if (bind(sock, (struct sockaddr *) &server, sizeof (server)) < 0) {
        perror("binding stream socket");
    }
    listen(sock, 5);
    while (1) {
        msgsock = accept(sock, (struct sockaddr *) 0, (socklen_t *) sizeof(server));
        if (msgsock == -1) {
            perror("accept");
        }
        ServerThread thread{msgsock};
        thread.start();
    }

    return 0;
}
