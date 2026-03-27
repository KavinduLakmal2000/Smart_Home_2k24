#ifndef TEMPERATURE_READER_MANAGER_H
#define TEMPERATURE_READER_MANAGER_H

#include <Arduino.h>
#include <DHT.h>

namespace TemperatureReaderManager {

struct TemperatureReaderContext {
  DHT& dht;
  DHT& dht2;
  DHT& dht3;
  unsigned long& lastDHTReadTime;
  unsigned long dhtInterval;
  float& envH;
  float& systemTemp;
  float& batteryTemp;
  float& envT;
  long& dhtchk;
  long& dhtchk1;
  long& dhtchk2;
};

void run(TemperatureReaderContext& context);

}  // namespace TemperatureReaderManager

#endif
