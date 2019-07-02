#include "asio.hpp"
#include "shared.h"
#include <WinSock2.h>
#include <conio.h>
#include <iostream>
#include <vector>

DeviceInfo DeviceInfo::SystemInfo() {
  char name[32];
  DWORD size = 32;
  GetComputerNameExA(ComputerNamePhysicalDnsHostname, name, &size);
  return DeviceInfo{name, "Windows", 1248};
}

std::vector<DeviceInfo> clients;

void error(const std::string &err) {
  std::cerr << "[Error] " << err << std::endl;
  exit(1);
}

int main() {
  asio::io_service io_service;
  asio::ip::udp::socket socket{io_service};
  asio::error_code err;
  socket.open(asio::ip::udp::v4(), err);

  if (err) {
    error("Could not open socket: " + err.message());
  }

  socket.set_option(asio::ip::udp::socket::reuse_address(true));
  socket.set_option(
      asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{3000});
  socket.set_option(asio::socket_base::broadcast(true));

  asio::ip::udp::endpoint sender_endpoint{asio::ip::address_v4::broadcast(),
                                          BROADCAST_PORT};

  std::cout << "Broadcasting device info... " << std::endl;

  DeviceInfo device_info = DeviceInfo::SystemInfo();

  socket.send_to(asio::buffer(&device_info, sizeof(device_info)),
                 sender_endpoint);

  std::cout << "Discovering Clients, please wait..." << std::endl;

  DeviceInfo client_info;
  try {
    while (socket.receive_from(asio::buffer(&client_info, sizeof(client_info)),
                               sender_endpoint) > 0) {
      client_info.set_ip(sender_endpoint.address().to_string());
      clients.push_back(client_info);
    }
  } catch (const asio::system_error &e) {
    if (e.code().value() != asio::error::timed_out) {
      error("Could not receive data: " + std::string{e.what()});
    }
  }

  std::cout << "Available clients: \n";

  for (const DeviceInfo &client : clients) {
    std::cout << "\n\tClient name: " << client.device_name
              << "\n\tClient system: " << client.device_system
              << "\n\tClient address: " << client.device_ip << ":"
              << client.device_port << std::endl;
  }

  std::cout << "\n\nPress any key to continue...";

  _getch();
  socket.close();

  return 0;
}
