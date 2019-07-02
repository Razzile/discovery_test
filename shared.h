#include <memory.h>

const unsigned short BROADCAST_PORT = 19001;

struct DeviceInfo {
  char device_name[32];
  char device_system[32];
  unsigned short port;

  DeviceInfo() {
    memset(device_name, 0, 32);
    memset(device_system, 0, 32);
  }

  DeviceInfo(const char *device_name, const char *device_system,
             unsigned short port) {
    memcpy(this->device_name, device_name, 32);
    memcpy(this->device_system, device_system, 32);
    this->port = port;
  };

  static DeviceInfo SystemInfo();
};