// Copyright 2021 видимо, что то очень нужное для тестов
// Author: ivansoya

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
    char buf[1024];
    std::string ip = argv[1];
    int port = atoi(argv[2]);
    // std::cout << str << std::endl;
    int sock;
    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    socklen_t len = sizeof(addr);
    // -1 нужен, чтобы не учитывать лишний ввод
    for (;;) {
        std::string str;
        std::cin >> str;
        sendto(sock, str.c_str(), str.size(), 0,
           (struct sockaddr *)&addr, len);
        int bytes = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&addr,
           &len);
        buf[bytes] = '\0';
        std::cout << buf << std::endl;
    }
    close(sock);
    return 0;
}

