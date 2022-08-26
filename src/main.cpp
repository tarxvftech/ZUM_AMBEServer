/*
 * Example source code for the ZUM AMBE server board
 *   Copyright (C) 2019-2021 by by David PA7LIM/Jim KI6ZUM
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

#include "SH1106.h"
SH1106 display(0x3c, SDA_pin, SCL_pin);

#include "WiFi.h"

#include <MQTTClient.h>


#include <Ethernet.h>
#include <SPI.h>
uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 1, 1);
IPAddress dns(8, 8, 8, 8);


// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {

  pinMode(ETHERNET_SELECT, OUTPUT);


  // "Serial" connected to FTDI chip TXD0/RXD0 pins
  Serial.begin(460800); 
  delay(10);

  // "Serial1" connected to Debug/Nextion port
  Serial.begin(115200); 

  // "Serial2" connected to AMBE chip IO16/IO17 pins
  Serial2.begin(460800, SERIAL_8N1, RX2_pin, TX2_pin);
  // not sure if the timeout really needs to be set or not
  Serial2.setTimeout(1000);


  digitalWrite(ETHERNET_SELECT, LOW); // Enable Ehernet card
  Ethernet.init(ETHERNET_SELECT); // Init the W5500
  Ethernet.begin(mac); // DHCP 

  //Serial1.println();
  //Serial1.println();
  //Serial1.print("Connecting to ");
  //Serial1.println(ssid);

  //WiFi.begin(ssid, password);
  //  WiFi.config(ip, gw, sn,  dns);
  //  WiFi.config(IPAddress(192, 168, 1, 60), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));

  // TODO: put a timeout here
  //while (WiFi.status() != WL_CONNECTED) {
    //delay(500);
    //Serial1.print(".");
  //}

  //Serial1.println("");
  //Serial1.println("WiFi connected"); 
  //Serial1.println("IP address: ");
  //Serial1.println(WiFi.localIP());

}

void loop() {
}

