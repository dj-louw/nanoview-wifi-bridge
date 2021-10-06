#include <Arduino.h>
#include <HTTPClient.h>

#include "credentials.h"
#include "OTA.h"


int counter = 0;

void setup(void) {
  Serial.begin(115200);
  Serial.println("ESP32 Booting...");

  setupOTA(WIFI_SSID, WIFI_PASSWORD);


//set real time clock
configTime(0, 0, "pool.ntp.org");   
struct tm timeinfo;
getLocalTime(&timeinfo);


}

void loop(void) {

    HTTPClient http;
    http.begin(influx_server_string);
    http.addHeader("Content-Type", "application/binary");


    if (counter > 6699){
      counter = 0;
    }
      
    String payload = "TestMeasurement,tagKey=testTag counter=" + String(counter*counter++) + "\n";

    Serial.println("posting: " + payload);

    // for(int i=0; i<bufferIndex; i++)
    // {
        // pos = sprintf(pos,"moisture level=%d,threshold=%d %d000000000\n",
        //                measBuffer[i],thresholdBuffer[i],timestampBuffer[i]);
    // }    

    sleep(1);

    http.POST(payload);
    http.end();


}