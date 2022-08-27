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
#include <Arduino.h>
#include "pins.h"
#include <SPI.h>
#include <WiFi.h>
#include <Ethernet.h>
//#include <MQTTClient.h>

//#define HOST_NAME "zumbe" //arduino Ethernet library does not support setting this, but would be a trivial thing to add. Might considering forking it.


//#warning Ethernet and SdFat may conflict, but SD.h/FS.h definitely conflict with esp32 sadly
#include <SdFat.h>
#define SD_FAT_TYPE 3 //support both fat, exfat
// SDCARD_SS_PIN is defined for the built-in SD on some boards.
const uint8_t SD_CS_PIN = SD_SELECT;

// Try max SPI clock for an SD. Reduce SPI_CLOCK if errors occur.
#define SPI_CLOCK SD_SCK_MHZ(20)
// Try to select the best SD card configuration.
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_CLOCK)
SdFs sd;

//#include "SH1106.h"
//SH1106 display(0x3c, SDA_pin, SCL_pin);


uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//later, default to one less than the wifi mac or something
EthernetClient ethernet;

char wifi_ssid[129] = {0};
char wifi_psk[129] = {0};

void dv3k_hardreset(){
  // hardware reset the AMBE chip
  digitalWrite(AMBE_RST, LOW);
  delay(100);
  digitalWrite(AMBE_RST, HIGH);
  delay(100);
}


void parse_all_config_files(); //from configfiles.cpp
                               
void setup() {
  Serial.begin(460800); // "Serial" connected to FTDI chip TXD0/RXD0 pins
  Serial1.begin(115200); // "Serial1" connected to Debug/Nextion port
  Serial2.begin(460800, SERIAL_8N1, RX2_pin, TX2_pin); 
  // "Serial2" connected to AMBE chip IO16/IO17 pins
  //
  delay(250); //give host time to open serial port for monitoring/debugging
  Serial.println("Setup!");

  // set up reset pin on the AMBE
  pinMode (AMBE_RST, OUTPUT);
  digitalWrite(AMBE_RST, HIGH);

  SPI.begin();
  pinMode(ETHERNET_SELECT, OUTPUT);
  digitalWrite(ETHERNET_SELECT, HIGH); // disable Ehernet card


  if (!sd.begin(SD_CONFIG)) {
    Serial.println("SDCard Failed!");
    sd.initErrorPrint(&Serial);
    //may print error codes which you can lookup here:
    //https://github.com/greiman/SdFat/blob/master/doc/SdErrorCodes.txt
  } else {
    Serial.println("SDCard Working!");
    parse_all_config_files();
  }
  sd.end();
  digitalWrite(ETHERNET_SELECT, LOW); // Enable Ethernet card
  Ethernet.init(ETHERNET_SELECT); // Init the W5500
  Ethernet.begin(mac); // DHCP, uses mac from eth.txt if present and working
  Serial.println("Booted, ready!");
}

enum _serialmode {
  debug = 0, //doesn't do anything yet
  ambepassthrough, //everything (except for a leave-mode command) passed through to AMBE3k
};
enum _serialmode serialmode = debug;
uint8_t ambe3kreset[] = {0x61, 0x00, 0x01, 0x00, 0x33};
int8_t buf[5];
int bufi = 0;
bool possible_mode_switch = false;
void loop() {
  if( Serial.available() ){
    int x = Serial.read();
    buf[bufi++] =x;
    if( bufi > 5 ){
      possible_mode_switch = false;
      bufi=0;
      memset(buf, 0, 5);
    }
    if( x == 0x61 ){
      possible_mode_switch = true;
      memset(buf, 0, 5);
      bufi=0;
      buf[bufi++] =x;
    } else {
      if( strncmp( (const char *)buf, (const char *)ambe3kreset, 5) == 0 ){
        //if we get a reset packet, toggle ambepassthrough (and pass the packet too!)
        if( serialmode == ambepassthrough ){
          Serial.println("Mode switch: debug");
          serialmode = debug;
        } else {
          Serial.println("Mode switch: AMBE3k");
          serialmode = ambepassthrough;
        }
        Serial2.write(ambe3kreset,5);
        memset(buf, 0, 5);
      }
    }
  }
  if( Serial2.available() ){
    if( serialmode == ambepassthrough ){
      Serial.write(Serial2.read());
    }
  }
}
