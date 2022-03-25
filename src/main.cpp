#include <Arduino.h>
#include <HTTPClient.h>
#include <TelnetStream.h>

#include "credentials.h"
#include "OTA.h"
#include "influx.h"
#include "nanoview.h"

#include <string>
#include <sstream>
#include <iomanip>

// define the rx and tx pins, just to make sure
#define RXD2 17
#define TXD2 16

void setup(void)
{
  Serial.begin(115200);
  Serial.println("ESP32 Booting...");

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  setupOTA(WIFI_SSID, WIFI_PASSWORD);

  configTime(0, 0, "pool.ntp.org");
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  Serial.println("Setting up hardware serial...");
  Serial2.begin(2400, SERIAL_8N1, RXD2, TXD2);
  Serial.println("/n Done!");

  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 23' to connect");
}

void loop(void)
{
  OTALoopFunctions();

  // uint8_t i;

  if (WiFi.status() == WL_CONNECTED)
  {

    // check UART for data
    if (Serial2.available())
    {
      uint8_t start_byte, type_byte;
      int packet_to_read_size = 0;
      int offset = 0;

      /*
        Basic Algorithm:
         1. repeat read until start is found
         2. read next byte and determine packet type
         3. define read extents for each packet type
         4. read in each packet, byte by byte
         5. calculate CRC on packet
         6. print
      */

      do
      {
        Serial2.readBytes((uint8_t *)&start_byte, 1);
      } while (start_byte != 0xAA);

      Serial2.readBytes((uint8_t *)&type_byte, 1);

      switch (type_byte)
      {
      case 0x10:
        packet_to_read_size = 36;
        break;
      case 0x11:
        packet_to_read_size = 66;
        break;
      case 0x12:
        packet_to_read_size = 3;
        break;
      default:
        TelnetStream.println("Unknown Packet");
      }

      uint8_t packet[packet_to_read_size];

      while (packet_to_read_size > 0)
      {
        Serial2.readBytes((uint8_t *)&packet[offset], 1);
        offset++;
        packet_to_read_size--;
      }

      uint16_t packet_crc = *((uint16_t *)(&(packet[offset - 2])));
      if (packet_crc == nv_crc(packet, offset - 2))
      {
        switch (type_byte)
        {
        case 0x10:
        {
          struct nv_live_power *nvlp = (struct nv_live_power *)&(packet);
          print_live_power(nvlp);
          break;
        }
        case 0x11:
        {
          struct nv_accumulated_energy *nvae = (struct nv_accumulated_energy *)&(packet);
          print_accumulated_energy(nvae);
          break;
        }
        case 0x12:
        {
          break;
        }
        default:
          TelnetStream.println("Unknown Packet");
        }
      }
      else
      {
        TelnetStream.println("CRC FAIL");
      }
      offset = 0;
    }
  }
  else
  {
    Serial.println("WiFi not connected!");
    delay(1000);
    ESP.restart();
  }

  switch (TelnetStream.read())
  {
  case 'R':
    TelnetStream.println("rebooting...");
    TelnetStream.stop();
    delay(100);
    ESP.restart();
    break;
  case 'C':
    TelnetStream.println("closing connection....");
    TelnetStream.stop();
    break;
  }
}
