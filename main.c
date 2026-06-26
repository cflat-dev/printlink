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

    printf("[host]: Server listening on port %d\n", PORT);

    while (1) {
        printf("[host]: Waiting for a client...\n");
        int client_fd = accept(server_fd, NULL, NULL);
        printf("[host]: Client connected\n");

        char buf[512];
        int n = recv(client_fd, buf, sizeof(buf)-1, 0);

        if (n > 0) {
            buf[n] = '\0'; // ensure null-terminated for safety
            printf("[host]: Gcode: %s", buf);

            char *response = send_gcode(p, buf);

            // send WITHOUT null terminator
            send(client_fd, response, strlen(response), 0);
        } else {
            printf("[host]: Client disconnected without sending data\n");
        }

        close(client_fd);
        printf("[host]: Connection closed, waiting for next client...\n");
    }

    close(server_fd);
    return 0;
}
