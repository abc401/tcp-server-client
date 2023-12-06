
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080

void check_error(int result, char *message)
{
    if (result != SOCKET_ERROR)
    {
        return;
    }
    fprintf(stderr, "[-] %s: code(%d)\n", message, WSAGetLastError());
    exit(EXIT_FAILURE);
}

int main()
{
    int result;

    WSADATA wsaData;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        printf("WSA startup failed: %d\n", result);
        exit(EXIT_FAILURE);
    }

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET)
    {
        fprintf(stderr, "[-] Could not create socket: %d\n", WSAGetLastError());
    }

    printf("[+] Binding socket to %s:%d\n", "localhost", PORT);
    struct sockaddr_in *server_addr = malloc(sizeof(struct sockaddr_in));
    memset(server_addr, 0, sizeof(struct sockaddr_in));
    server_addr->sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr->sin_port = PORT;
    server_addr->sin_family = AF_INET;

    result = bind(sockfd, (struct sockaddr *)server_addr, sizeof(*server_addr));
    check_error(result, "Bind failed");
    printf("[+] Bind success.\n");

    result = listen(sockfd, 5);
    check_error(result, "Could not listen to socket");
    printf("[+] Listening on localhost:%d\n", PORT);

    char msg[] = "Hello world";
    char buffer[1024];

    while (1)
    {
        struct sockaddr_storage client_addr;
        int client_addr_size = sizeof(client_addr);
        SOCKET client_socket = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_size);

        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("[+] Client said: %s\n", buffer);

        send(client_socket, msg, sizeof(msg), 0);
        printf("[+] Server said: %s\n", msg);
        closesocket(client_socket);
    }

    WSACleanup();
}