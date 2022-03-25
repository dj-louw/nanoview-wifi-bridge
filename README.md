# NanoView Wifi Bridge
This project will create a wifi bridge intended to connect your Nanoview power monitor to the rest of the world.

## Hardware
This project makes use of an ESP32, that connects directly to the NanoHub serial line.

## Plan / Features
* Implement the protocol decoding straight in the ESP32 [done]
* Directly integrate with InfluxDB to ingest the data in a raw format [done]
* Use Grafana to then graph the data, and from there feed into HomeAssistant or similar [done]