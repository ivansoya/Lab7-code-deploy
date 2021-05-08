// Copyright 2021 видимо, что то очень нужное для тестов
// Author: ivansoya
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
        printf("Error at WSAStartup()\n");
    // Create a socket.
    SOCKET m_socket;
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_socket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    // Connect to a server.
    struct sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("192.168.0.110");
    clientService.sin_port = htons(3425);
    // Send and receive data.
    int bytesSent;
    int bytesRecv = SOCKET_ERROR;
    std::string inputPath;
    std::cout << "Enter path: ";
    std::cin >> inputPath;
    char recvbuf[128] = "";
    bytesSent = sendto(m_socket, inputPath.c_str(), inputPath.size(), 0,
        (struct sockaddr*)&clientService, sizeof(clientService));
    while (bytesRecv == SOCKET_ERROR) {
        int len = sizeof(clientService);
        bytesRecv = recvfrom(m_socket, recvbuf, 128, 0,
            (struct sockaddr*)&clientService, &len);
        if (bytesRecv == 0 || bytesRecv == WSAECONNRESET) {
            printf("Connection Closed.\n");
            break;
        }
        if (bytesRecv < 0)
            return 1;
        std::cout << recvbuf << std::endl;
        system("pause");
    }
    return 0;
}
