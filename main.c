#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "lib/libprintlink.h"

#define PORT 9000


int main() {
    printer_t *p = NULL;

    while (p == NULL) {
        printf("[host]: trying to find a printer\n");
        p = find_printer();
        usleep(1000000);
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("[host]: G-code streamer listening on port %d\n", PORT);

    while (1) {
        printf("[host]: Waiting for a client...\n");
        int client_fd = accept(server_fd, NULL, NULL);
        printf("[host]: Client connected\n");

        char buf[512];
        char reply[512];

        while (1) {
            int n = recv(client_fd, buf, sizeof(buf)-1, 0);
            if (n <= 0) {
                printf("[host]: Client disconnected\n");
                break;
            }

            buf[n] = '\0';
            printf("[host]: Gcode: %s", buf);

            // Send to printer
            send_gcode(p, buf);

            // Get printer reply
            int rn = get_printer_reply(p, reply, sizeof(reply));
            if (rn <= 0) {
                strcpy(reply, "ERR\n");
            }

            // Stream reply back to client
            send(client_fd, reply, strlen(reply), 0);
        }

        close(client_fd);
        printf("[host]: Connection closed, waiting for next client...\n");
    }

    close(server_fd);
    return 0;
}
