#include <WinSock2.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include "shared.h"

DeviceInfo DeviceInfo::SystemInfo()
{
    char name[32];
    DWORD size = 32;
    GetComputerNameA(name, &size);
    return DeviceInfo{name,
                      "Windows",
                      1248};
}

std::vector<DeviceInfo> clients;

int main()
{
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);

    SOCKET sock;
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    char broadcast = '1';

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        std::cout << "Error in setting Broadcast option";
        closesocket(sock);

        return 0;
    }

    int timeout = 3000;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) < 0)
    {
        std::cout << "Error in setting Broadcast option";
        closesocket(sock);

        return 0;
    }

    sockaddr_in recv_addr;

    int socklen = sizeof(recv_addr);

    char data[] = "hello from windows pc server";

    DeviceInfo client_info = DeviceInfo();
    int recvlen = sizeof(client_info);

    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(BROADCAST_PORT);
    recv_addr.sin_addr.s_addr = INADDR_BROADCAST;

    sendto(sock, data, strlen(data) + 1, 0, (sockaddr *)&recv_addr, sizeof(recv_addr));

    std::cout << "Discovering Clients, please wait..." << std::endl;

    while (recvfrom(sock, (char *)&client_info, recvlen, 0, (sockaddr *)&recv_addr, &socklen) >= 0)
    {
        clients.push_back(client_info);
    }

    std::cout << "Available clients: \n";

    for (const DeviceInfo &client : clients)
    {
        std::cout << "\n\tClient name: " << client.device_name << "\n\tClient system: " << client.device_system
                  << "\n\tClient port:" << client.port << std::endl;
    }

    std::cout << "\n\n\tpress any key to continue...";

    _getch();
    closesocket(sock);
    WSACleanup();

    return 0;
}