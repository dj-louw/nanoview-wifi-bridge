# NanoView Wifi Bridge
This project will create a wifi bridge intended to connect your Nanoview power monitor to the rest of the world.

This project is heavily inspired by and based on the work done by Denvera at https://github.com/denvera/esp32-nanoview. All the serial wrangling and decoding is compliments of their efforts.

## Hardware
This project makes use of an ESP32, that connects directly to the NanoHub serial line.

See the below diagram:
![Nanoview interface with esp32-Wiring drawio](https://user-images.githubusercontent.com/1496180/160196168-465c4582-ff0c-43d4-ba93-323c13e2b7b6.png)

I make use of the following product to convert between the logic level of the Nanoview and the ESP32: https://www.robotics.org.za/BOB-12009

## Some notes
There are some gotchas that you have to be careful of:

* Don't power the ESP from the Nanoview directly. For some reason this does not work so well. I don't think the Nanoview supplies proper power on it's PWR line. I don't have the equpment to properly test and verify this. All I know is that it does not work. Hence the 9V rail in the above design.
* This post has some additional information: http://silico.co.za/blog/2019/04/13/hacking-the-nanoview-with-a-rpi-zero-w/

## Plan / Features
* Implement the protocol decoding straight in the ESP32 [done]
* Directly integrate with InfluxDB to ingest the data in a raw format [done]
* Use Grafana to then graph the data, and from there feed into HomeAssistant or similar [done]
