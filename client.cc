#include "asio.hpp"
#include "shared.h"
#include <WinSock2.h>
#include <conio.h>
#include <iostream>

DeviceInfo DeviceInfo::SystemInfo() {
  char name[32];
  DWORD size = 32;
  GetComputerNameExA(ComputerNamePhysicalDnsHostname, name, &size);
  return DeviceInfo{name, "Windows", 1248};
}

void error(const std::string &err) {
  std::cerr << "[Error] " << err << std::endl;
  exit(1);
}

int main() {
  asio::io_service io_service;

  asio::ip::udp::endpoint local_endpoint{asio::ip::address_v4::any(),
                                         BROADCAST_PORT};
  asio::ip::udp::socket socket{io_service, local_endpoint};

  socket.set_option(asio::ip::udp::socket::reuse_address(true));
  socket.set_option(asio::socket_base::broadcast(true));

  asio::ip::udp::endpoint sender_endpoint;

  DeviceInfo server_info;
  // wait for new data
  socket.receive_from(asio::buffer(&server_info, sizeof(server_info)),
                      sender_endpoint);
  server_info.set_ip(sender_endpoint.address().to_string());

  std::cout << "Sending device info to "
            << sender_endpoint.address().to_string() + ":" +
                   std::to_string(sender_endpoint.port())
            << std::endl;
  DeviceInfo device_info = DeviceInfo::SystemInfo();
  device_info.set_ip(local_endpoint.address().to_string());

  try {
    socket.send_to(asio::buffer(&device_info, sizeof(device_info)),
                   sender_endpoint);
  } catch (const asio::system_error &e) {
    error("Could not send device info: " + std::string{e.what()});
  }

  std::cout << "\n\nPress any key to continue...";

  _getch();
  socket.close();

  return 0;
}