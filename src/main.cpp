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

// #define MAX_SRV_CLIENTS 1
// WiFiServer server(23);
// WiFiClient serverClients[MAX_SRV_CLIENTS];

 #define RXD2 17
 #define TXD2 16



void setup(void)
{
  Serial.begin(115200);
  Serial.println("ESP32 Booting...");

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  setupOTA(WIFI_SSID, WIFI_PASSWORD);

  // set real time clock
  configTime(0, 0, "pool.ntp.org");
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  // setupInflux();


  Serial.println("Setting up hardware serial...");
  Serial2.begin(2400,SERIAL_8N1, RXD2, TXD2);
  Serial.println("/n Done!");

  // server.begin();
  // server.setNoDelay(true);

  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 23' to connect");

 }

void loop(void)
{
  OTALoopFunctions();

  uint8_t i;
  if (WiFi.status() == WL_CONNECTED) {
    //check UART for data
    
    if(Serial2.available()){
      //size_t len = Serial2.available();
      uint8_t start_byte, type_byte;
      int packet_to_read_size = 0;
      int offset = 0;
      
      /*
        repeat read until start is found
        read next byte and determine packet type
        define read extents for each packet type
        read in each packet, byte by byte
        calculate CRC on packet
        print
      */

      do
      {
        Serial2.readBytes((uint8_t *)&start_byte, 1);
      } while (start_byte != 0xAA);
      
      //TelnetStream.println("Start Found!");

      Serial2.readBytes((uint8_t*)&type_byte,1);
      
      switch (type_byte) {
            case 0x10: 
                //TelnetStream.println("Live Power Packet");
                packet_to_read_size = 36;
                break;
            case 0x11:
                //TelnetStream.println("Accumulated Power Packet");
                packet_to_read_size = 66;
                break;
            case 0x12:
                //TelnetStream.println("Firmware Version Packet");
                packet_to_read_size = 3;
                break;
            default:
                TelnetStream.println("Unknown Packet");
                //continue; // Uknown packet type
        }

        uint8_t packet[packet_to_read_size];

        while(packet_to_read_size > 0){
          Serial2.readBytes((uint8_t*)&packet[offset],1);
          offset++;
          packet_to_read_size--;
        }

        //ESP_LOGD(RX_TASK_TAG, "Packet Type %02x: Read %d bytes", p.type, offset);
        //ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, p.data, offset, ESP_LOG_DEBUG);
        uint16_t packet_crc = *((uint16_t *) (&(packet[offset-2])));
        //ESP_LOGD(RX_TASK_TAG, "CRC: %04x == %04x [%s]", packet_crc, nv_crc(p.data, offset-2), ((packet_crc == nv_crc(p.data, offset-2)) ? "OK" : "BAD"));
        if (packet_crc == nv_crc(packet, offset-2)) {
            //TelnetStream.println("CRC SUCCESS");
            switch (type_byte) {
                case 0x10: { 
                    struct nv_live_power *nvlp = (struct nv_live_power *)&(packet);
                    print_live_power(nvlp);
                    break;
                }
                case 0x11: {
                    struct nv_accumulated_energy *nvae = (struct nv_accumulated_energy *)&(packet);
                    print_accumulated_energy(nvae);
                    break;
                }
                case 0x12: {
                    break;
                }
                default:
                    TelnetStream.println("Unknown Packet");
                    //continue; // Uknown packet type
            }
        } else {
          TelnetStream.println("CRC FAIL");
        }
        offset = 0;


        // now we should have the full packet

        // for(int i = 0; i <= sizeof packet; i++){
        //   TelnetStream.print(packet[i]);
        // }
        // TelnetStream.println("\n Done \n \n \n");

      
    }
  }
  else {
    Serial.println("WiFi not connected!");
    delay(1000);
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

  //loopInflux();
}


