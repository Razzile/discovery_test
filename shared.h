#include <memory.h>

const unsigned short BROADCAST_PORT = 19001;

struct DeviceInfo {
  char device_name[32];
  char device_system[32];
  char device_ip[17];
  unsigned short device_port;

  DeviceInfo() {
    memset(device_name, 0, 32);
    memset(device_system, 0, 32);
    memset(device_ip, 0, 16);
  }

  DeviceInfo(const char *name, const char *system, unsigned short port) {
    memcpy(device_name, name, 32);
    memcpy(device_system, system, 32);
    device_port = port;
  }

  void set_ip(const std::string &ip) {
    memcpy(device_ip, ip.data(), strnlen(ip.data(), 16) + 1);
  }

  static DeviceInfo SystemInfo();
};