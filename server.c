#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

int main() {
    int server_port;
    printf("Server port: ");
    scanf("%d", &server_port);

    // Выделим сокет для TCP-сервера.
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Теперь же создадим структуру, в которой опишем на каком порту будет наш сервер.
    // А также не забудем забиндить наш сокет к указанному адресу.
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listen_fd, (struct sockaddr *) &addr, sizeof(addr));
    listen(listen_fd, 5);

    // Теперь дождемся наших клиентов.
    int client1_socket, client2_socket, socket_status;
    struct sockaddr_in client1_address, client2_address;
    printf("Waiting clients ...\n");
    client1_socket = accept(listen_fd, (struct sockaddr*)&client1_address, (socklen_t*)&socket_status);
    printf("First client connected!\n");
    client2_socket = accept(listen_fd, (struct sockaddr*)&client2_address, (socklen_t*)&socket_status);
    printf("Second client connected!\n");

    // Вот вроде все подключились, теперь получаем сообщения от первого клиента и пересылаем второму.
    char client1_message[8192];
    while (true) {
        memset(client1_message, 0, sizeof(client1_message));
        size_t read = recv(client1_socket, client1_message, sizeof(client1_message), 0);

        // Если прочитало 0 байт, то значит сокет закрыт.
        if (read == 0) {
            printf("First client disconnected!\n");
            break;
        }

        // Иначе же мы получили сообщение, выведем его в консоль и отправим второму клиенту.
        printf("Received message from first client: %s\n", client1_message);
        // Если пришло сообщение == "The End", то значит, что нужно закрыть сокеты.
        if (strcmp(client1_message, "The End") == 0) {
            break;
        }

        // Иначе же отправим сообщение второму клиенту.
        socket_status = send(client2_socket, client1_message, strlen(client1_message), 0);
        if (socket_status == -1) {
            printf("Failed to send message to client2\n");
            break;
        }
    }

    // Закрытие сокетов
    close(client1_socket);
    close(client2_socket);
    close(listen_fd);
    return 0;
}
