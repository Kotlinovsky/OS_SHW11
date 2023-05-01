#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main() {
    int server_port;
    char* server_host = malloc(128);
    printf("Enter: <address> <port>\n");
    scanf("%s %d", server_host, &server_port);

    // Забьем адрес в структуру.
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_host);
    server_address.sin_port = htons(server_port);

    // Запускаем сокет, сразу подключимся.
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    int socket_status = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if (socket_status == -1) {
        printf("Failed to connect to server\n");
        exit(1);
    }

    char client_message[8192];
    printf("Receiving messages:\n");
    while (true) {
        size_t size = recv(client_socket, &client_message, sizeof(client_message), 0);

        if (size == 0) {
            printf("Connection closed!");
            break;
        } else if (size == -1) {
            printf("Error during reading!");
            break;
        }

        printf("%s\n", client_message);
        if (strcmp(client_message, "The End") == 0) {
            break;
        }
    }

    return 0;
}
