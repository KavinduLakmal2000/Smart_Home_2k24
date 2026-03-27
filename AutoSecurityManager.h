#ifndef AUTO_SECURITY_MANAGER_H
#define AUTO_SECURITY_MANAGER_H

#include <Arduino.h>

namespace AutoSecurityManager {

struct AutoSecurityContext {
  int hours;
  int outsidePirTopPin;
};

void run(const AutoSecurityContext& context);

}  // namespace AutoSecurityManager

#endif
