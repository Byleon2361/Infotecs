#include "../lib/lib.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 57866

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to server at 127.0.0.1:" << DEFAULT_PORT << "\n";

    while (1)
    {
        int n = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (n < 0)
        {
            perror("recv failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        else if (n == 0)
        {
            std::cout << "Server disconnected" << "\n";
            close(sockfd);
            break;
        }

        buffer[n] = '\0';
        std::string str(buffer);
        if (myLib::count(str))
        {
            std::cout << str << "\n";
        }
        else
        {
            std::cout << "The string is more than 2 characters and the number is not a multiple of 32" << "\n";
        }
    }

    close(sockfd);
    return 0;
}