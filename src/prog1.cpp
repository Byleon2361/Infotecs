#include "../lib/lib.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
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
        std::cout << "Введите последовательность цифр" << "\n";
        std::cin >> str;
        if (checkStr(str) == 0)
        {
            std::cout << "Строка должна состоять из цифр" << "\n";
            exit(1);
        }
        if (str.size() > 64)
        {
            std::cout << "Строка не должна превышать 64 символа" << "\n";
            exit(1);
        }
        sort(str);
        mtx.lock();
        BUFFER = str;
        ISBUFFERFULL = 1;
        mtx.unlock();
    }
}
void func2()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = 0;

    if (bind(sockfd, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    getsockname(sockfd, (struct sockaddr*)&server_addr, &client_len);
    std::cout << "Server is running on port: " << ntohs(server_addr.sin_port) << std::endl;

    while (1)
    {
        if (ISBUFFERFULL)
        {
            mtx.lock();
            std::string str = BUFFER;
            BUFFER.clear();
            ISBUFFERFULL = 0;
            mtx.unlock();
            std::cout << str << "\n";

            sendto(sockfd, str.c_str(), str.length(), MSG_CONFIRM, (const struct sockaddr*)&client_addr, client_len);
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