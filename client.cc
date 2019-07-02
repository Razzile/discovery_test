#include <WinSock2.h>
#include <conio.h>
#include <iostream>
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

    sockaddr_in recv_addr;
    sockaddr_in send_addr;
    int socklen = sizeof(recv_addr);

    char buffer[128];
    int recvlen = sizeof(buffer);

    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(BROADCAST_PORT);
    recv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr *)&recv_addr, sizeof(recv_addr)) < 0)
    {
        std::cout << "Error in binding: " << WSAGetLastError();
        _getch();
        closesocket(sock);

        return 0;
    }

    /**
     * We don't need to receive data from the server in this example, just send data back*/
    recvfrom(sock, buffer, recvlen, 0, (sockaddr *)&send_addr, &socklen);

    std::cout << "Received Message: " << buffer;
    std::cout << "\n\n\tPress Any to send message";

    DeviceInfo info = DeviceInfo::SystemInfo();
    if (sendto(sock, (char *)&info, sizeof(info), 0, (sockaddr *)&send_addr, sizeof(send_addr)) < 0)
    {
        std::cout << "Error in Sending. " << WSAGetLastError();
        std::cout << "Press any key to continue....";

        _getch();
        closesocket(sock);

        return 0;
    }

    else
    {
        std::cout << "READER sends the broadcast message Successfully";
    }

    std::cout << "\n\n\tpress any key to CONT...";

    _getch();
    closesocket(sock);
    WSACleanup();

    return 0;
}