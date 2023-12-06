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

    printf("[+] Connecting socket to %s:%d\n", "localhost", PORT);

    struct sockaddr_in *server_addr = malloc(sizeof(struct sockaddr_in));
    memset(server_addr, 0, sizeof(struct sockaddr_in));
    server_addr->sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr->sin_port = PORT;
    server_addr->sin_family = AF_INET;

    char msg[] = "Hello from client";
    result = connect(sockfd, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in));
    check_error(result, "Failed to connect");
    send(sockfd, msg, sizeof(msg), 0);
    printf("Client said: %s\n", msg);

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server said: %s\n", buffer);

    WSACleanup();
}