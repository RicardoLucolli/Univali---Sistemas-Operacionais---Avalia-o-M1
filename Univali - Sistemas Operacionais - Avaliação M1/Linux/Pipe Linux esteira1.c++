
//• Esteira 1: produtos de maior peso (5 Kg) – passa 1 item a cada 2 segundos pelo sensor.
//• Esteira 2: produtos de peso médio (2 Kg) – passa 1 item a cada 1 segundo pelo sensor.

#include <iostream>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "/tmp/pipeso"

using namespace std;
//using namespace std::literals;

void esteira1() {

    int sockfd, newsockfd, len;
    struct sockaddr_un local, remote;
    char buffer[1024];
    int p1 = 0, p1_kg = 0;

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Falha em criar o pipe");
        return;
    }

    // Bind socket to local address
    memset(&local, 0, sizeof(local));
    local.sun_family = AF_UNIX;
    strncpy(local.sun_path, SOCK_PATH, sizeof(local.sun_path) - 1);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(sockfd, (struct sockaddr *)&local, len) < 0)
    {
        perror("Falha em capturar o socket");
        close(sockfd);
        return;
    }

    // Listen for connections
    if (listen(sockfd, 5) < 0)
    {
        perror("Falha em escutar o socket");
        close(sockfd);
        return;
    }

    printf("Servidor Named pipe ouvindo em %s...\n", SOCK_PATH);

    // Accept connections
    memset(&remote, 0, sizeof(remote));
    len = sizeof(remote);
    newsockfd = accept(sockfd, (struct sockaddr *)&remote, (socklen_t*)&len);
    if (newsockfd < 0)
    {
        perror("Falha em aceitar coneccao");
        close(sockfd);
        return;
    }

    printf("Cliente conectado!\n");

    while (1) {
        // Read data from client

        std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

        // Read data from client
        if (read(newsockfd, buffer, sizeof(buffer)) < 0)
        {
            perror("Falha em ler do socket");
            close(newsockfd);
            close(sockfd);
            return;
        }


        auto end = std::chrono::steady_clock::now();

        cout << "esteira1:\t" << p1 << " produtos (" << p1_kg << ")\n";
        cout << "esteira2:\t" << atoi(buffer)/2 << " produtos (" << buffer << ")\n";
        cout << "(" << (end - start).count() << " nanoseconds)\n\n";
        this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(2));
        p1 ++;
        p1_kg += 5;
    }
}

int main()
{
    esteira1();
    return 0;
}