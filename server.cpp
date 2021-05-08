// Copyright 2021 видимо, что то очень нужное для тестов
// Author: ivansoya

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>

std::string get_disk_size(const char* path);

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server_address, client_address;
    char buf[1024];
    int bytes_read;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }
    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(3425);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(sock, (struct sockaddr *)&server_address,
        sizeof(server_address)) < 0) {
        perror("bind");
        exit(2);
    }
    
    while (1) {
        socklen_t len = sizeof(client_address);
        bytes_read = recvfrom(sock, buf, 1024, 0,
        (struct sockaddr *)&client_address, &len);
        buf[bytes_read] = '\0';
        std::string answer = get_disk_size(buf);
        // std::cout << answer << "\n";
        sendto(sock, answer.c_str(), answer.size(), 0,
            (struct sockaddr *)&client_address, len);
    }
    
    close(sock);
    
    return 0;
}

std::string get_disk_size(const char* path) {
    struct statvfs stat;
    if (statvfs(path, &stat) == 0) {
        int64_t free_space = stat.f_bfree * stat.f_frsize;
        int64_t close_space = stat.f_blocks * stat.f_frsize;
        std::string str;
        str = std::to_string(free_space) + " " + std::to_string(close_space);
        return str;
    } else {
        return "Incorrect path!";
    }
}
