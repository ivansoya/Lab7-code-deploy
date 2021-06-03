// Copyright 2021 видимо, что то очень нужное для тестов
// Author: ivansoya

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <string>
#include <vector>
#include <iostream>
#define NUMBER_OF_THREADS 2
pthread_t tid[NUMBER_OF_THREADS];

std::string get_disk_size(const char* path);
bool is_number(const std::string&);
void* client_proccess(void*);

sem_t semaphore;

int main(int argc, char *argv[]) {
    int port;
    if (argc != 2) {
        std::cout << "Incorrect number of arguments!" << std::endl;
        return 1;
     }
    std::string str = argv[1];
    // А ведь мог бы и так
    // std::regex_match(str, std::regex("[1-9][0-9]*")) == false)
    if (is_number(str) == false) {
        std::cout << "Port must be an integer!" << std::endl;
        return 1;
    }
    port = atoi(argv[1]);
    if (port < 0 || port > 65535) {
        std::cout << "Port is Incorrect!" << std::endl;
        return 1;
    }
    if (sem_init(&semaphore, 0, 1) != 0) {
        std::cerr << "Semaphore init failed" << std::endl;
        return 1;
    }
    int sock;
    struct sockaddr_in server_address;
    int bytes_read;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }
    std::cout << "Server was started!\n";
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr *)&server_address,
        sizeof(server_address)) < 0) {
        perror("bind");
        exit(2);
    }
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
         if (pthread_create(&tid[i], NULL, &client_proccess, &sock) < 0) {
              std::cout << "Couldn't create thread!/n";
         }
    }
    while (1) { }
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
         pthread_join(tid[i], NULL);
    }
    /*std::vector<std::string> queue;
    std::vector<struct sockaddr_in> adresses;
    while (1) {
        socklen_t len = sizeof(client_address);
        bytes_read = recvfrom(sock, buf, 1024, 0,
        (struct sockaddr *)&client_address, &len);
        if (bytes_read > 0) {
            adresses.push_back(client_address);
        }
        buf[bytes_read] = '\0';
        queue.push_back(buf);
        while (queue.size() > 0) {
            sem_wait(&semaphore);
            std::string answer = get_disk_size(queue[0].c_str());
            queue.erase(queue.begin());
            std::cout << buf << "\n";
            sendto(sock, answer.c_str(), answer.size(), 0,
              (struct sockaddr *)&adresses[0], len);
            adresses.erase(adresses.begin());
            sem_post(&semaphore);
        }
    }*/
    close(sock);
    return 0;
}

void* client_proccess(void* arg) {
    int sock = *reinterpret_cast<int *>(arg);
    char buf[1024];
    struct sockaddr_in client_address;
    socklen_t len = sizeof(client_address);
    while (1) {
        int bytes_read = recvfrom(sock, buf, 1024, 0,
            (struct sockaddr*)&client_address, &len);
        buf[bytes_read] = '\0';
        if (bytes_read > 0) {
            sem_wait(&semaphore);
            std::string answer = get_disk_size(buf);
            std::cout << "Request: " << buf << "\n";
            sendto(sock, answer.c_str(), answer.size(), 0,
                (struct sockaddr*)&client_address, len);
            std::cout << "Response: " << answer << "\n";
            sem_post(&semaphore);
        }
    }
    return arg;
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

bool is_number(const std::string& str) {
    if (*str.begin() == '0') return false;
    std::string::const_iterator it = str.begin();
    while (it != str.end() && std::isdigit(*it)) ++it;
    return !str.empty() && it == str.end();
}

