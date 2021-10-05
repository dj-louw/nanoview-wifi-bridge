#include <Arduino.h>


#include "credentials.h"
#include "OTA.h"

void setup(void) {
  Serial.begin(115200);
  Serial.println("ESP32 Booting...");
  setupOTA(WIFI_SSID, WIFI_PASSWORD);


}

void loop(void) {
  AsyncElegantOTA.loop();
}