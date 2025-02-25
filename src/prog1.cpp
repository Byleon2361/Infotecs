#include "../lib/lib.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#define DEFAULT_PORT 57866
std::string BUFFER;
int ISBUFFERFULL = 0;
std::mutex mtx;

int checkStr(std::string& str)
{
    for (int ch : str)
    {
        if (!(ch >= '0' && ch <= '9'))
        {
            return 0;
        }
    }
    return 1;
}

void func1()
{
    while (1)
    {
        std::string str;
        std::cout << "Enter a sequence of numbers" << "\n";
        std::cin >> str;
        if (checkStr(str) == 0)
        {
            perror("The line must consist of numbers");
            exit(EXIT_FAILURE);
        }
        if (str.size() > 64)
        {
            perror("The line must not exceed 64 characters.");
            exit(EXIT_FAILURE);
        }
        myLib::sort(str);
        mtx.lock();
        BUFFER = str;
        ISBUFFERFULL = 1;
        mtx.unlock();
    }
}

void func2()
{
    int sockfd, new_sockfd = 0;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(DEFAULT_PORT);

    if (bind(sockfd, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) < 0)
    {
        perror("listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if (new_sockfd == 0)
        {
            std::cout << "Waiting for client connection..." << "\n";
            new_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
            if (new_sockfd < 0)
            {
                perror("accept failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            std::cout << "Client connected" << "\n";
        }

        if (ISBUFFERFULL)
        {
            mtx.lock();
            std::string str = BUFFER;
            BUFFER.clear();
            ISBUFFERFULL = 0;
            mtx.unlock();

            std::cout << str << "\n";

            int s = myLib::sum(str);
            str = std::to_string(s);
            if (send(new_sockfd, str.c_str(), str.size(), 0) < 0)
            {
                perror("send failed");
                close(new_sockfd);
                new_sockfd = 0;
                continue;
            }
        }

        char buf[1];
        if (recv(new_sockfd, buf, sizeof(buf), MSG_PEEK | MSG_DONTWAIT) == 0)
        {
            std::cout << "Client disconnected\n";
            close(new_sockfd);
            new_sockfd = 0;
        }
    }
    close(sockfd);
}

int main()
{
    std::thread th1(func1);
    std::thread th2(func2);
    th1.join();
    th2.join();
    return 0;
}