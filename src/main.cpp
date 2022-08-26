/*
 *   Copyright (C) 2019-2022 by by David PA7LIM/Jim KI6ZUM/Mike W2FBI
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <stdint.h>
#include <Arduino.h>
#include "pins.h"

#include <SD.h>
#include <FS.h>
#include <SPI.h>
#include <WiFi.h>
#include <Ethernet.h>
#include <MQTTClient.h>

//#include "SH1106.h"
//SH1106 display(0x3c, SDA_pin, SCL_pin);


uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetClient ethernet;

const char* ssid     = "ham";
const char* password = "AAAAAAAAAAAA";

void setup() {
  pinMode(ETHERNET_SELECT, OUTPUT);
  digitalWrite(ETHERNET_SELECT, LOW); // Enable Ehernet card
  Ethernet.init(ETHERNET_SELECT); // Init the W5500
  Ethernet.begin(mac); // DHCP 

  Serial.begin(460800); // "Serial" connected to FTDI chip TXD0/RXD0 pins

  Serial1.begin(115200); // "Serial1" connected to Debug/Nextion port

  Serial2.begin(460800, SERIAL_8N1, RX2_pin, TX2_pin); 
  // "Serial2" connected to AMBE chip IO16/IO17 pins

  SPI.begin();
  if (!SD.begin(SD_SELECT)){
    Serial.println("No SD card found");
  } else {
    File file = SD.open("/NET.INI"); // read the textfile NET.INI from SDcard
    if(!file){
      Serial.println("Failed to open file for reading");
      return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
      Serial.write(file.read());
    }
    file.close();
  }
  WiFi.begin(ssid, password);

}

void loop() {
}

