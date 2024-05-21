#pragma once
#include <string>

namespace Bluetooth {
  void Begin(const std::string & device_name);
  void End();
  void Service();
  bool IsConnected();
}