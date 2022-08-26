#ifndef _PINS_H
#define _PINS_H

// ESP32 pin name to Arduino pin number mapping
#define SVP    5
#define SVN    8
#define IO34  34
#define IO35  35
#define IO32  32
#define IO33  33
#define IO25  25
#define IO26  26
#define IO27  27
#define IO14  14
#define IO12  12
#define IO13  13
#define IO15  15
#define IO02   2
#define IO00   0
#define IO04   4
#define IO16  16
#define IO17  17
#define IO05   5
#define IO18  18
#define IO23  23
#define IO19  19
#define IO22  22
#define IO21  21

// ZUM AMBE board pin definitions
#define SD_SELECT       IO32 // SD card select pin
#define ETHERNET_SELECT IO05 // Ethernet select pin
#define ETHERNET_RST    IO12 // Ethernet reset pin
#define SDA_pin         IO25 // I2C connector
#define SCL_pin         IO26 // I2C connector
#define RX1_pin         IO13 // Debug/Nextion connector
#define TX1_pin         IO14 // Debug/Nextion connector
#define RX2_pin         IO16 // AMBE3000 RX to ESP32 TXD
#define TX2_pin         IO17 // AMBE3000 TX to ESP32 RXD
#define AMBE_RST        IO21 // AMBE reset PIN
#define AMBE_RTS        IO35 // AMBE3000 request to send pin
#define XCODE_LED       IO27 // LED on Board  
#define STAT_LED        IO33 // LED on Board
#define AP_BUTTON       IO04 // AP button state
#define MOSI            IO23 // SPI MOSI
#define MISO            IO19 // SPI MISO
#define SCLK            IO18 // SPI SCLK
#define FTDI_RXD        TXD0 // RXD from FTDI to ESP32 TXD
#define FTDI_TXD        RXD0 // TXD from FTDI to ESP32 RXD



#endif
