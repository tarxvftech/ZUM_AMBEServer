#define NOBUILD
#ifndef NOBUILD

#define VERSION 1
#define RELEASE 0
// using example packets from:
// https://github.com/g4klx/AMBETools/blob/master/Common/DV3000SerialController.cpp

const unsigned char DV3000_START_BYTE   = 0x61U;
const unsigned char DV3000_TYPE_CONTROL = 0x00U;
const unsigned char DV3000_TYPE_AMBE    = 0x01U;
const unsigned char DV3000_TYPE_AUDIO   = 0x02U;
const unsigned char DV3000_CONTROL_RATEP  = 0x0AU;
const unsigned char DV3000_CONTROL_GAIN   = 0x4BU;
const unsigned char DV3000_CONTROL_PRODID = 0x30U;
const unsigned char DV3000_CONTROL_VERSTRING = 0x31U;
const unsigned char DV3000_CONTROL_READY = 0x39U;
const unsigned char DV3000_CONTROL_RESETSOFTCFG = 0x34U;

// enable/disable specific tests if desired
#define LED_TEST 0
#define SD_TEST 0
#define ETHERNET_TEST 0
//#define WIFI_TEST 0
//#define WIFI_SERVER_TEST 0
#define AMBE_TEST 0
#define OLED_TEST 0
//#define BUTTON_TEST 0
#define AMBE_PASSTHROUGH 0
//#define WIFI_AMBE_SERVER 0
const unsigned char DV3000_REQ_PRODID[] = {DV3000_START_BYTE, 0x00U, 0x01U, DV3000_TYPE_CONTROL, DV3000_CONTROL_PRODID};
const unsigned int DV3000_REQ_PRODID_LEN = 5U;
const unsigned char DV3000_REQ_VERSTRING[]     = {DV3000_START_BYTE, 0x00U, 0x01U, DV3000_TYPE_CONTROL, DV3000_CONTROL_VERSTRING};
const unsigned int DV3000_REQ_VERSTRING_LEN = 5U;

uint8_t DV3000_REQ[]      = { 0x61U, 0x00U, 0x01U, 0x00U, 0x30U };
#define DV3000_REQ_LEN 5U
uint8_t DV3000_REQ_RESP[] = {0x61, 0x00, 0x0b, 0x00, 0x30, 0x41, 0x4d, 0x42, 0x45, 0x33, 0x30, 0x30, 0x30, 0x52, 0x00 };
#define DV3000_REQ_RESP_LEN 15U

uint8_t DV3000_REQ_3[]      = { 0x61, 0x00, 0x07, 0x00, 0x34, 0x05, 0x00, 0x00, 0x07, 0x00, 0x10 };
#define DV3000_REQ_3_LEN 11U
uint8_t DV3000_REQ_3_RESP[] = { 0x61, 0x00, 0x01, 0x00, 0x39    };
#define DV3000_REQ_3_RESP_LEN 5U

uint8_t DV3000_REQ_4[]      = { 0x61, 0x00, 0x03, 0x00, 0x05, 0x10, 0x40 };
#define DV3000_REQ_4_LEN 7U
uint8_t DV3000_REQ_4_RESP[] = { 0x61, 0x00, 0x02, 0x00, 0x05, 0x00 };
#define DV3000_REQ_4_RESP_LEN 6U

uint8_t DV3000_REQ_5[]      = { 0x61, 0x00, 0x0D, 0x00, 0x0A, 0x01, 0x30, 0x07, 0x63, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48 };
#define DV3000_REQ_5_LEN 17U
uint8_t DV3000_REQ_5_RESP[] = { 0x61, 0x00, 0x02, 0x00, 0x0A, 0x00 };
#define DV3000_REQ_5_RESP_LEN 6U

uint8_t DV3000_REQ_RESET[]        = { DV3000_START_BYTE, 0x00U, 0x07U, DV3000_TYPE_CONTROL, DV3000_CONTROL_RESETSOFTCFG, 0x05U, 0x00U, 0x00U, 0x0FU, 0x00U, 0x00U };
#define DV3000_REQ_RESET_LEN 11U
// packet after hard reset same as soft reset? (yes, 0x00 0x39 is the READY packet)
uint8_t DV3000_RESP_RESET[]       = { 0x61, 0x00, 0x01, 0x00, 0x39 };
#define DV3000_RESP_RESET_LEN 5U


#if defined (OLED_TEST)
// some info on how to use 1.3" OLED screens
// https://robocraze.com/blog/post/oled-working-and-demo
// Take an image in BMP format and use the conversion tool provided in the following website to convert it to XBM format.
// https://convertio.co/image-converter/
// Or if you wish to design a small XBM image pixel by pixel you can find a useful tool at the link below.
// http://4umi.com/web/javascript/xbm.php

#include "SH1106.h"
SH1106 display(0x3c, SDA_pin, SCL_pin);

// wifi logo found on these web pages
// https://github.com/rene-mt/esp8266-oled-sh1106/blob/master/examples/SH1106Demo/images.h
// https://robocraze.com/blog/post/oled-working-and-demo
#define WiFi_Logo_width 60
#define WiFi_Logo_height 36
const uint8_t WiFi_Logo_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
  0xFF, 0x03, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0xFF, 0xFF, 0xFF, 0x07, 0xC0, 0x83, 0x01, 0x80, 0xFF, 0xFF, 0xFF,
  0x01, 0x00, 0x07, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0C, 0x00,
  0xC0, 0xFF, 0xFF, 0x7C, 0x00, 0x60, 0x0C, 0x00, 0xC0, 0x31, 0x46, 0x7C,
  0xFC, 0x77, 0x08, 0x00, 0xE0, 0x23, 0xC6, 0x3C, 0xFC, 0x67, 0x18, 0x00,
  0xE0, 0x23, 0xE4, 0x3F, 0x1C, 0x00, 0x18, 0x00, 0xE0, 0x23, 0x60, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xE0, 0x03, 0x60, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x07, 0x60, 0x3C, 0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C,
  0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00, 0xE0, 0x8F, 0x71, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xC0, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x08, 0x00,
  0xC0, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x0C, 0x00, 0x80, 0xFF, 0xFF, 0x1F,
  0x00, 0x00, 0x06, 0x00, 0x80, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x07, 0x00,
  0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0xF8, 0xFF, 0xFF,
  0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00,
  0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// iron man bitmap found on this webpage:
// https://sandhansblog.wordpress.com/2017/04/16/interfacing-displaying-a-custom-graphic-on-an-0-96-i2c-oled/
#define ironman_width 44
#define ironman_height 64
static const unsigned char ironman_bits[] = {
  0x00, 0x00, 0xfc, 0x03, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x3f, 0x00, 0x00,
  0x00, 0xf8, 0xff, 0xff, 0x01, 0x00, 0x00, 0x86, 0xff, 0x1f, 0x06, 0x00,
  0x80, 0x81, 0xff, 0x1f, 0x18, 0x00, 0xc0, 0x80, 0xff, 0x1f, 0x30, 0x00,
  0x20, 0x80, 0xff, 0x1f, 0xc0, 0x00, 0x10, 0x00, 0xff, 0x0f, 0x80, 0x01,
  0x08, 0x00, 0xff, 0x0f, 0x00, 0x01, 0x08, 0x00, 0xff, 0x0f, 0x00, 0x02,
  0x08, 0x00, 0xff, 0x0f, 0x00, 0x02, 0x08, 0x00, 0xfe, 0x07, 0x00, 0x02,
  0x08, 0x00, 0xfe, 0x07, 0x00, 0x02, 0x08, 0x00, 0xfe, 0x07, 0x00, 0x02,
  0x08, 0x00, 0x00, 0x00, 0x00, 0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x08, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x0c, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x0c, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x0c, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x06, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xc6, 0x07, 0x00, 0x00, 0x7c, 0x0e,
  0xe6, 0xff, 0xff, 0xff, 0xff, 0x0e, 0xe6, 0xff, 0x03, 0xf8, 0xff, 0x0c,
  0x02, 0x3f, 0x00, 0x80, 0x1f, 0x0c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x0c,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x0c,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x0c,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x07, 0x00, 0x00, 0x00, 0x00, 0x06, 0x16, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x2e, 0x00, 0x00, 0x00, 0x80, 0x07, 0x6e, 0x00, 0x00, 0x00, 0x40, 0x07,
  0x6e, 0x00, 0x00, 0x00, 0x60, 0x07, 0xde, 0x00, 0x00, 0x00, 0xb0, 0x07,
  0xde, 0x00, 0x00, 0x00, 0xb0, 0x07, 0x9c, 0x01, 0x00, 0x00, 0xd8, 0x03,
  0xbc, 0x01, 0x00, 0x00, 0xf8, 0x03, 0x3c, 0x01, 0x00, 0x00, 0xe8, 0x03,
  0x7c, 0x03, 0x00, 0x00, 0xf4, 0x03, 0xfc, 0x02, 0x00, 0x00, 0xf4, 0x03,
  0xfc, 0xc2, 0xff, 0x1f, 0xfa, 0x03, 0xf8, 0x25, 0xff, 0x27, 0xfa, 0x01,
  0xf8, 0x38, 0x00, 0x60, 0xf9, 0x01, 0xf8, 0x10, 0x00, 0xc0, 0xf8, 0x01,
  0xf0, 0x00, 0x00, 0x00, 0xf8, 0x01, 0xf0, 0x00, 0x00, 0x00, 0xf8, 0x01,
  0xf0, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0xe0, 0xc1, 0xff, 0x0f, 0xf8, 0x00,
  0xc0, 0xe3, 0xff, 0x1f, 0x7c, 0x00, 0x80, 0xff, 0xff, 0x7f, 0x3e, 0x00,
  0x00, 0xff, 0xff, 0x7f, 0x0f, 0x00, 0x00, 0xf6, 0x00, 0xfe, 0x07, 0x00,
  0x00, 0x3c, 0x00, 0xb0, 0x01, 0x00, 0x00, 0x08, 0x00, 0x40, 0x00, 0x00 };

void testOLED(void)
{
  display.init(); // Init the OLED
  display.clear();

  //if you want to invert the display
  display.flipScreenVertically();  

  // Available default fonts defined in SH1106Fonts.h:
  // ArialMT_Plain_10, ArialMT_Plain_16, ArialMT_Plain_24
  display.setFont(ArialMT_Plain_10);

  display.drawString(0, 0, "ZUM AMBE BOARD v" + String(VERSION) + "." + String(RELEASE));
  display.drawString(0, 10, "Second Line");
  display.drawString(0, 20, "Third Line");
  display.drawString(0, 30, "Fourth Line");
  display.drawString(0, 40, "Fifth Line");
  //display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  display.drawXbm( 42, 0, ironman_width, ironman_height, ironman_bits);

  display.display();
}
#endif

#if defined (AMBE_TEST)
void testAMBE() 
{
  char readchar;
  char string[100];
  // "Serial2" connected to AMBE chip IO16/IO17
  Serial2.begin(460800, SERIAL_8N1, RX2_pin, TX2_pin);
  Serial2.setTimeout(1000);

  // set the FTDI speed to 460800 to match speed of real usage
  Serial.begin(460800);

  // set up reset pin on the AMBE
  pinMode (AMBE_RST, OUTPUT);
  digitalWrite(AMBE_RST, HIGH);

  // hardware reset the AMBE chip
  digitalWrite(AMBE_RST, LOW);
  delay(100);
  digitalWrite(AMBE_RST, HIGH);
  delay(100);

  // flush receive buffer from AMBE chip
  while (Serial2.available() > 0)
    Serial2.read();

  // request 1
#if !defined(AMBE_PASSTHROUGH)
  Serial.println("sending product version request packet");
#endif
  Serial2.write(DV3000_REQ_3, DV3000_REQ_3_LEN);
  delay(100);
  while (Serial2.available() > 0)
  {
    readchar = (char)Serial2.read();
#if !defined(AMBE_PASSTHROUGH)     
    sprintf(string, "0x%.2x-%c", readchar, readchar);
    Serial.println(string);
#endif     
  }

#if !defined(AMBE_PASSTHROUGH)     
  Serial.println("");
#endif

  // request 2
#if !defined(AMBE_PASSTHROUGH)
  Serial.println("sending product version request packet");
#endif
  Serial2.write(DV3000_REQ_4, DV3000_REQ_4_LEN);
  delay(100);
  while (Serial2.available() > 0)
  {
    readchar = (char)Serial2.read();
#if !defined(AMBE_PASSTHROUGH)     
    sprintf(string, "0x%.2x-%c", readchar, readchar);
    Serial.println(string);
#endif     
  }

#if !defined(AMBE_PASSTHROUGH)     
  Serial.println("");
#endif

  // request 3
#if !defined(AMBE_PASSTHROUGH)
  Serial.println("sending product version request packet");
#endif
  Serial2.write(DV3000_REQ_5, DV3000_REQ_5_LEN);
  delay(100);
  while (Serial2.available() > 0)
  {
    readchar = (char)Serial2.read();
#if !defined(AMBE_PASSTHROUGH)     
    sprintf(string, "0x%.2x-%c", readchar, readchar);
    Serial.println(string);
#endif     
  }

#if !defined(AMBE_PASSTHROUGH)     
  Serial.println("");
#endif

  // request Product ID
#if !defined(AMBE_PASSTHROUGH)
  Serial.println("");
  Serial.println("sending product id request packet");
#endif
  Serial2.write(DV3000_REQ_PRODID, DV3000_REQ_PRODID_LEN);
  delay(100);
  while (Serial2.available() > 0)
  {
    readchar = (char)Serial2.read();
#if !defined(AMBE_PASSTHROUGH)
    sprintf(string, "0x%.2x-%c", readchar, readchar);
    Serial.println(string);
#endif     
  }

#if !defined(AMBE_PASSTHROUGH)
  Serial.println("");
#endif 

  // request the Product Version
#if !defined(AMBE_PASSTHROUGH)
  Serial.println("sending product version request packet");
#endif
  Serial2.write(DV3000_REQ_VERSTRING, DV3000_REQ_VERSTRING_LEN);
  delay(100);
  while (Serial2.available() > 0)
  {
    readchar = (char)Serial2.read();
#if !defined(AMBE_PASSTHROUGH)     
    sprintf(string, "0x%.2x-%c", readchar, readchar);
    Serial.println(string);
#endif     
  }

#if !defined(AMBE_PASSTHROUGH)     
  Serial.println("");
#endif


  Serial2.end();
}
#endif

#if defined(SD_TEST)
#include <SD.h>
#include <FS.h>
void testSD(void)
{
  // SDcard
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
}
#endif

#if defined(AMBE_PASSTHROUGH)
void passthroughAMBE() 
{
  // "Serial2" connected to AMBE chip IO16/IO17 pins
  Serial2.begin(460800, SERIAL_8N1, RX2_pin, TX2_pin);
  // not sure if the timeout really needs to be set or not
  Serial2.setTimeout(1000);
  // "Serial" connected to FTDI chip TXD0/RXD0 pins
  Serial.begin(460800); 

  // set up reset pin on the AMBE
  pinMode (AMBE_RST, OUTPUT);
  digitalWrite(AMBE_RST, HIGH);

  // hardware reset the AMBE chip
  digitalWrite(AMBE_RST, LOW);
  delay(100);
  digitalWrite(AMBE_RST, HIGH);
  delay(100);

  while (1) {
    if (Serial2.available() > 0)
      Serial.write(Serial2.read());
    if (Serial.available() > 0)
      Serial2.write(Serial.read());
  }
}
#endif



#if defined(ETHERNET_TEST)
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

void testEthernet(void)
{
  char server[] = "www.google.com";    // name address for Google (using DNS)
  unsigned long beginMicros, endMicros;
  unsigned long byteCount = 0;
  bool printWebData = true;  // set to false for better speed measurement

  pinMode(ETHERNET_SELECT, OUTPUT);
  digitalWrite(ETHERNET_SELECT, LOW); // Enable Ehernet card
  Ethernet.init(ETHERNET_SELECT); // Init the W5500

  Ethernet.begin(mac); // DHCP 
                       // Ethernet.begin(mac, ip, dns, gateway, subnet); // Uncomment for static IP

                       // Show IP information to your serial port  
  Serial.println (F("DHCP configuration done:"));
  Serial.print (F("- IP: "));
  Serial.println (Ethernet.localIP());
  Serial.print (F("- Netmask: "));
  Serial.println (Ethernet.subnetMask());
  Serial.print (F("- Default Gateway: "));
  Serial.println (Ethernet.gatewayIP());

  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    // Make a HTTP request:
    client.println("GET /index.html HTTP/1.1");
    client.println("Host: www.ki6zum.com");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }

  beginMicros = micros();
  // if there are incoming bytes available
  // from the server, read them and print them:
  int len = client.available();
  if (len > 0) {
    byte buffer[80];
    if (len > 80) len = 80;
    client.read(buffer, len);
    if (printWebData) {
      Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    }
    byteCount = byteCount + len;
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    endMicros = micros();
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    Serial.print("Received ");
    Serial.print(byteCount);
    Serial.print(" bytes in ");
    float seconds = (float)(endMicros - beginMicros) / 1000000.0;
    Serial.print(seconds, 4);
    float rate = (float)byteCount / seconds / 1000.0;
    Serial.print(", rate = ");
    Serial.print(rate);
    Serial.print(" kbytes/second");
    Serial.println();
  }
}
#endif

#if defined(WIFI_TEST)
#include "WiFi.h"
void testWiFi(void)
{
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");
}
#endif 

#if defined (WIFI_AMBE_SERVER)
#include "WiFi.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// ambeserver code based on code as part of opendv project from G4KLX
// https://github.com/g4klx/DummyRepeater/blob/master/DV3000/AMBEserver.c

// some example Arduino UDP code:
// https://thearduinoandme.wordpress.com/tutorials/esp8266-send-receive-binary-data/
// https://www.arduino.cc/en/Reference/WiFiUDPParsePacket
// https://forum.arduino.cc/index.php?topic=464996.0
// https://www.arduino.cc/en/Tutorial/WiFiSendReceiveUDPString
// better ones
// http://esp8266tutorials.blogspot.com/2016/10/esp8266-udp-server-example.html
// ++++ https://siytek.com/esp8266-udp-send-receive/

#define DV3K_TYPE_CONTROL 0x00
#define DV3K_TYPE_AMBE 0x01
#define DV3K_TYPE_AUDIO 0x02

static const unsigned char DV3K_START_BYTE   = 0x61;

static const unsigned char DV3K_CONTROL_RATEP  = 0x0A;
static const unsigned char DV3K_CONTROL_PRODID = 0x30;
static const unsigned char DV3K_CONTROL_VERSTRING = 0x31;
static const unsigned char DV3K_CONTROL_RESET = 0x33;
static const unsigned char DV3K_CONTROL_READY = 0x39;
static const unsigned char DV3K_CONTROL_CHANFMT = 0x15;

#define dv3k_packet_size(a) (1 + sizeof((a).header) + ntohs((a).header.payload_length))

#define DV3000_VERSION          "2015-07-11"
#define DEFAULT_PORT            2460
#define DEFAULT_TTY             "/dev/ttyUSB0"
#define DEFAULT_BAUD            460800

// struct sockaddr_in sa = { 0 };
//static int debug = 0;

#pragma pack(push, 1)
struct dv3k_packet {
  unsigned char start_byte;
  struct {
    unsigned short payload_length;
    unsigned char packet_type;
  } header;
  union {
    struct {
      unsigned char field_id;
      union {
        char prodid[16];
        char ratep[12];
        char version[48];
        short chanfmt;
      } data;
    } ctrl;
    struct {
      unsigned char field_id;
      unsigned char num_samples;
      short samples[160];
      unsigned char cmode_field_id;
      short cmode_value;
    } audio;
    struct {
      struct {
        unsigned char field_id;
        unsigned char num_bits;
        unsigned char data[9];
      } data;
      struct {
        unsigned char field_id;
        unsigned short value;
      } cmode;
      struct {
        unsigned char field_id;
        unsigned char tone;
        unsigned char amplitude;
      } tone;
    } ambe;
  } payload;
};
#pragma pack(pop)


void dump(char *text, struct dv3k_packet *packet, unsigned int length)
{
  unsigned int offset = 0U;
  unsigned int i;
  unsigned char *data = (unsigned char *) packet;
  char string[100];

  sprintf(string, "Received %s packet with type %d and length %d:\n", text, 
      (char *)packet->header.packet_type, ntohs(packet->header.payload_length));
  Serial.print((char *)string);

  while (length > 0U) {
    unsigned int bytes = (length > 16U) ? 16U : length;

    sprintf(string, "%04X:  ", offset);
    Serial.print((char *)string);

    for (i = 0U; i < bytes; i++) {
      sprintf(string, "%02X ", data[offset + i]);
      Serial.print((char *)string);
    }

    for (i = bytes; i < 16U; i++) {
      Serial.print("   ");
    }

    Serial.print("   *");

    for (i = 0U; i < bytes; i++) {
      unsigned char c = data[offset + i];

      if (isprint(c))
        Serial.print(c);
      else
        Serial.print('.');
    }

    Serial.print("*\n");

    offset += 16U;

    if (length >= 16U)
      length -= 16U;
    else
      length = 0U;
  }

}


const char* ssid     = "XXX";
const char* password = "XXX";

WiFiUDP UDPTestServer;
unsigned int UDPPort = 2807;
IPAddress ip(192, 168, 1, 6);
IPAddress gw(192, 168, 1, 1);
IPAddress dns(192, 168, 1, 1);
IPAddress sn(255, 255, 255, 0);

const int packetSize = 2;
byte packetBuffer[packetSize];
int writeUDPPacket(struct dv3k_packet *packetm int length)
{
  int length;
  UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
  length = UDP.write(packet, length);
  UDP.endPacket();
  return(length);
}

//  int cb = UDPTestServer.parsePacket();
//  if (cb) {
//    UDPTestServer.read(packetBuffer, packetSize);

int readUDPPacket(struct dv3k_packet *packet, int length)
{
  int len=0;
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize); 
    // TODO: do something to make sure packetSize is not larger than length
    len = UDP.read(packet, length);
    //    Serial.print("Packet received: ");
    //    Serial.println(packet);
  }
  return(len);
}

int writeSerialPacket(struct dv3k_packet *packet, int length)
{
  //TODO: how to use the AMBE_RTS pin? hold off sending data to chip until RTS is low?
  // The AMBE-3000R™ Vocoder Chip sets RTSn low to indicate that it is ready to receive data. When RTSn is high, the Chip is not ready to receive packet data.
  return(Serial.write(packet, length));
}

int readSerialPacket(int fd, struct dv3k_packet *packet)
{
  ssize_t bytes, bytesLeft;
  char bytesRead;
  int i;
  char string[100];

  for(i = 0; i < sizeof(struct dv3k_packet); ++i) {
    //              bytesRead = read(fd, packet, 1);
    bytesRead = Serial.read();
    if(bytes == -1) {
      Serial.println("AMBEserver: Error reading from serial port");
      return 0;
    }
    else {
      *packet = (ssize_t)bytesRead;
    }
    if(packet->start_byte == DV3K_START_BYTE)
      break;
  }
  if(packet->start_byte != DV3K_START_BYTE) {
    Serial.println("AMBEserver: Couldn't find start byte in serial data");
    return 0;
  }

  bytesLeft = sizeof(packet->header);
  while(bytesLeft > 0) {
    bytes = readUDPPacket(((uint8_t *) &packet->header) + sizeof(packet->header) - bytesLeft, bytesLeft);
    //              bytes = read(fd, ((uint8_t *) &packet->header) + sizeof(packet->header) - bytesLeft, bytesLeft);
    if(bytes == -1) {
      Serial.println("AMBEserver: Couldn't read serial data header");
      return 0;
    }
    bytesLeft -= (size_t) bytes;
  }

  bytesLeft = ntohs(packet->header.payload_length);
  if(bytesLeft > sizeof(packet->payload)) {
    Serial.println("AMBEserver: Serial payload exceeds buffer size");
    return 0;
  }

  while(bytesLeft > 0) {
    bytes = readUDPPacket(((uint8_t *) &packet->payload) + (ntohs(packet->header.payload_length) - bytesLeft), bytesLeft);
    //        bytes = read(fd, ((uint8_t *) &packet->payload) + (ntohs(packet->header.payload_length) - bytesLeft), bytesLeft);
    if(bytes == -1) {
      Serial.println("AMBEserver: Couldn't read payload");
      return 0;
    }

    bytesLeft -= (size_t) bytes;
  }

  return 1;
}

static inline int checkResponse(struct dv3k_packet *responsePacket, unsigned char response)
{
  if(responsePacket->start_byte != DV3K_START_BYTE ||
      responsePacket->header.packet_type != DV3K_TYPE_CONTROL ||
      responsePacket->payload.ctrl.field_id != response)
    return 0;

  return 1;
}

int initDV3K(int fd, int hwReset)
{
  struct dv3k_packet responsePacket;
  char prodId[17];
  char version[49];

  struct dv3k_packet ctrlPacket = {
    .start_byte = DV3K_START_BYTE,
    .header.packet_type = DV3K_TYPE_CONTROL,
    .header.payload_length = htons(1),
    .payload.ctrl.field_id = DV3K_CONTROL_RESET
  };

  if(hwReset == 1) {
    if(hardwareReset() == 0)
      return 0;
  } else {
    if(writeSerialPacket(&ctrlPacket, dv3k_packet_size(ctrlPacket)) == -1) {
      fprintf(stderr, "AMBEserver: error writing reset packet: %s\n", strerror(errno));
      return 0;
    }
  }

  if(readSerialPacket(fd, &responsePacket) == 0) {
    fprintf(stderr, "AMBEserver: error receiving response to reset\n");
    return 0;
  }

  if(checkResponse(&responsePacket, DV3K_CONTROL_READY) == 0) {
    fprintf(stderr, "AMBEserver: invalid response to reset\n");
    return 0;
  }

  ctrlPacket.payload.ctrl.field_id = DV3K_CONTROL_PRODID;
  if(writeSerialPacket(&ctrlPacket, dv3k_packet_size(ctrlPacket)) == -1) {
    fprintf(stderr, "AMBEserver: error writing product id packet: %s\n", strerror(errno));
    return 0;
  }

  if(readSerialPacket(fd, &responsePacket) == 0) {
    fprintf(stderr, "AMBEserver: error receiving response to product id request\n");
    return 0;
  }

  if(checkResponse(&responsePacket, DV3K_CONTROL_PRODID) == 0) {
    fprintf(stderr, "AMBEserver: invalid response to product id query\n");
    dump("ProdID Response:", &responsePacket, sizeof(responsePacket));
    return 0;
  }
  strncpy(prodId, responsePacket.payload.ctrl.data.prodid, sizeof(prodId));

  ctrlPacket.payload.ctrl.field_id = DV3K_CONTROL_VERSTRING;
  if(writeSerialPacket(&ctrlPacket, dv3k_packet_size(ctrlPacket)) == -1) {
    fprintf(stderr, "AMBEserver: error writing version packet: %s\n", strerror(errno));
    return 0;
  }

  if(readSerialPacket(fd, &responsePacket) == 0) {
    fprintf(stderr, "AMBEserver: error receiving response to version request\n");
    return 0;
  }

  if(checkResponse(&responsePacket, DV3K_CONTROL_VERSTRING) == 0) {
    fprintf(stderr, "AMBEserver: invalid response to version query\n");
    return 0;
  }
  strncpy(version, responsePacket.payload.ctrl.data.version, sizeof(version));

  fprintf(stdout, "AMBEserver: Initialized %s version %s.\n", prodId, version); 

  return 1;
}


int processSerial()
{
  struct dv3k_packet packet;

  if(readSerialPacket(&packet) == 0) {
    fprintf(stderr, "AMBEserver: processSerial failed to read packet\n");
    return 0;
  }

  if (debug)
    dump("Serial data", &packet, dv3k_packet_size(packet));

  if (sa.sin_port != 0U)
    //              if (sendto(sockFd, &packet, dv3k_packet_size(packet), 0, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) != dv3k_packet_size(packet)) {
    if (writeUDPPacket(&packet, dv3k_packet_size(packet)) != dv3k_packet_size(packet)) {
      fprintf(stderr, "AMBEserver: error when writing to the socket: %s\n", strerror(errno));
      return 0;
    }

  return 1;
}

int processSocket()
{
  socklen_t addrLen = sizeof(struct sockaddr_in);
  ssize_t bytesRead;
  struct dv3k_packet packet;

  //      bytesRead = recvfrom(&packet, sizeof(packet), 0, (struct sockaddr *) &sa, &addrLen);
  bytesRead = readUDPPacket(&packet, sizeof(packet));
  if (bytesRead < 0) {
    fprintf(stderr, "AMBEserver: error when reading from the socket: %s\n", strerror(errno));
    return 0;
  }

  if (debug)
    dump("Socket data", &packet, bytesRead);

  if (packet.start_byte != DV3K_START_BYTE) {
    fprintf(stderr, "AMBEserver: invalid start byte when reading from the socket, 0x%02X", packet.start_byte);
    return 1;
  }

  if (bytesRead != dv3k_packet_size(packet)) {
    fprintf(stderr, "AMBEserver: invalid length when reading from the socket, %zd=%zd", bytesRead, dv3k_packet_size(packet));
    return 1;
  }

  if (writeSerialPacket(&packet, dv3k_packet_size(packet)) != dv3k_packet_size(packet)) {
    fprintf(stderr, "AMBEserver: error when writing to the serial port: %s\n", strerror(errno));
    return 0;
  }

  return 1;
}

/*
   loop()
   {
   for (;;) {
   FD_ZERO(&fds);
   FD_SET(sockFd, &fds);
   FD_SET(serialFd, &fds);

   if (select(topFd, &fds, NULL, NULL, NULL) < 0) {
   fprintf(stderr, "AMBEserver: error from select: %s\n", strerror(errno));
   exit(1);
   }

   if(FD_ISSET(sockFd, &fds))
   if (!processSocket(sockFd, serialFd))
   exit(1);

   if(FD_ISSET(serialFd, &fds))
   if (!processSerial(sockFd, serialFd))
   exit(1);
   }

   }

*/

#endif

#if defined (WIFI_SERVER_TEST)
#include "WiFi.h"
// this wifi server test code was from here. No copyright message but includes "by Tom Igoe"
// https://forum.arduino.cc/index.php?topic=154182.0

void testWiFiServer(void)
{
  char ssid[] = "SSID";                      // your network SSID (name)
  char key[] = "password";                   // your network key
                                             //int keyIndex = 0;                             // your network key Index number
  int status = WL_IDLE_STATUS;                    // the Wifi radio's status

  Serial.print("Attempting to connect to WEP network, SSID: ");
  Serial.println(ssid);
  status = WiFi.begin(ssid, key);

  // wait 10 seconds for connection:
  delay(10000);

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

  WiFiServer server(80);
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> turn the LED on pin 9 on<br>");
            client.print("Click <a href=\"/L\">here</a> turn the LED on pin 9 off<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(9, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(9, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }

  WiFi.disconnect();

}
#endif

#if defined(BUTTON_TEST)
#define buttonAP 6
void testButtons(void)
{
  pinMode (buttonAP, INPUT);

  Serial.println("Button test");
  Serial.println("press and release the AP button");
  for (int i=0;i<100;i++) {
    if (digitalRead(buttonAP) == 0)
      Serial.println("open");
    else
      Serial.println("closed");
    delay(10);
  }
}
#endif

#if defined(LED_TEST)
void testLEDs(void)
{
  pinMode (XCODE_LED, OUTPUT);
  pinMode (STAT_LED, OUTPUT);

  for (int i=0;i<10;i++) {
    digitalWrite(XCODE_LED, HIGH);
    digitalWrite(STAT_LED, HIGH);
    delay(50);
    digitalWrite(XCODE_LED, LOW);
    digitalWrite(STAT_LED, LOW);
    delay(50);
  }
}
#endif

void setup() {
  pinMode(XCODE_LED, OUTPUT);     // initialize the XCODE LED
  pinMode(STAT_LED, OUTPUT);      // initialize the STAT LED
  digitalWrite(STAT_LED,LOW);       // Turn off the STAT LED
  digitalWrite(XCODE_LED,LOW);      // Turn off the XCODE LED

  //  Serial.begin(9600);             // debug information to serial via FTDI chip

#if defined(LED_TEST)
  //Serial.println("LED Test");
  testLEDs();
#endif

#if defined(SD_TEST)
  Serial.println("SD Test");
  testSD();
#endif

#if defined(ETHERNET_TEST)
  Serial.println("Ethernet Test");
  testEthernet();
#endif

#if defined(WIFI_TEST)
  Serial.println("WIFI Test");
  testWiFi();
#endif

#if defined(WIFI_SERVER_TEST)
  Serial.println("WIFI Server Test");
  testWiFiServer();
#endif

#if defined(AMBE_TEST)
  Serial.println("AMBE Test");
  testAMBE();
#endif

#if defined(OLED_TEST)
  Serial.println("OLED Test");
  testOLED();
#endif

#if defined(AMBE_PASSTHROUGH)
  passthroughAMBE();
#endif

#if defined(BUTTON_TEST)
  Serial.println("BUTTON Test");
  testButtons();
#endif

}
/*

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

// wifi connection variables
const char* ssid = “yourSSIDHere”;
const char* password = “YourPasswordHere”;
boolean wifiConnected = false;

// UDP variables
unsigned int localPort = 8888;
WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = “acknowledged”; // a string to send back

void setup() {
// Initialise Serial connection
Serial.begin(115200);

// Initialise wifi connection
wifiConnected = connectWifi();

// only proceed if wifi connection successful
if(wifiConnected){
udpConnected = connectUDP();
if (udpConnected){
// initialise pins
pinMode(5,OUTPUT);
}
}
}

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
int packetSize = UDP.parsePacket();
if(packetSize){
UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
}


void loop() {
// if there's data available, read a packet
int packetSize = Udp.parsePacket();
if (packetSize) {
Serial.print("Received packet of size ");
Serial.println(packetSize);
Serial.print("From ");
IPAddress remoteIp = Udp.remoteIP();
Serial.print(remoteIp);
Serial.print(", port ");
Serial.println(Udp.remotePort());
// read the packet into packetBufffer
int len = Udp.read(packetBuffer, 255);
if (len > 0) {
packetBuffer[len] = 0;
}
Serial.println("Contents:");
Serial.println(packetBuffer);
// send a reply, to the IP address and port that sent us the packet we received
Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
Udp.write(ReplyBuffer);
Udp.endPacket();
}
}
*/

/*
   ------------------------------------------------------
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Set WiFi credentials
#define WIFI_SSID "Gysotc"
#define WIFI_PASS "ENPhaUr3gxEn"
#define UDP_PORT 4210

// UDP
WiFiUDP UDP;
char packet[255];
char reply[] = "Packet received!";

void setup() {
// Setup serial port
Serial.begin(115200);
Serial.println();

// Begin WiFi
WiFi.begin(WIFI_SSID, WIFI_PASS);

// Connecting to WiFi...
Serial.print("Connecting to ");
Serial.print(WIFI_SSID);
// Loop continuously while WiFi is not connected
while (WiFi.status() != WL_CONNECTED)
{
delay(100);
Serial.print(".");
}

// Connected to WiFi
Serial.println();
Serial.print("Connected! IP address: ");
Serial.println(WiFi.localIP());

// Begin listening to UDP port
UDP.begin(UDP_PORT);
Serial.print("Listening on UDP port ");
Serial.println(UDP_PORT);

}

void loop() {

// If packet received...
int packetSize = UDP.parsePacket();
if (packetSize) {
Serial.print("Received packet! Size: ");
Serial.println(packetSize); 
int len = UDP.read(packet, 255);
if (len > 0)
{
packet[len] = '\0';
}
Serial.print("Packet received: ");
Serial.println(packet);

// Send return packet
UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
UDP.write(reply);
UDP.endPacket();

}

}
*/

//int openSocket(in_port_t port)
//{
//
//
//      struct sockaddr_in sa = { 0 };
//      int fd;
//
//      fd = socket(AF_INET, SOCK_DGRAM, 0);
//      if (fd < 0) {
//              fprintf(stderr, "AMBEserver: error when creating the socket: %s\n", strerror(errno));
//              return fd;
//      }
//
//      sa.sin_family = AF_INET;
//      sa.sin_port = htons(port);
//      sa.sin_addr.s_addr = htonl(INADDR_ANY);
//
//      if (bind(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
//              fprintf(stderr, "AMBEserver: error when binding the socket to port %u: %s\n", port, strerror(errno));
//              return -1;
//      }
//
//      if (debug)
//              fprintf(stdout, "opened the UDP socket on port %u\n", port);
//
//      return fd;
// }

#endif
