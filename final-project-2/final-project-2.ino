#include "system.h"
void setup() {
  Serial.begin(1000000);
  CleanScreen();
  StartScreen();
  SetupDHT11Sensor();
  delay(1000);
  SetupSoilSensor();
  delay(1000);
  SetupWifi();
  delay(3000);
  SetSensorParameterVeg();
}

void loop() {
  RefreshSensor();
  WebServer();
  DoAction();


}
