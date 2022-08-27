#include <SdFat.h>
#include <WiFi.h>

extern char wifi_ssid[129];
extern char wifi_psk[129];

#define MAX_KV_LEN 129
typedef struct _kv {
  char key[MAX_KV_LEN];
  char value[MAX_KV_LEN];
} kv;

int readBytesUntil(FsFile * file, const char* needle, char * buf, int max){
  return file->fgets(buf, max, (char*) needle);//cast because fgets here is silly
                           //fgets silently drops \r, which makes windows compat easy :)
}
int parse_kvpair(FsFile * file, kv * parsed){
  char line[MAX_KV_LEN] = {0};
  char * linep = line;
  int r = 0;
  r = readBytesUntil(file, "=", line, MAX_KV_LEN-1);
  if( !r ){
    return -1;
  }
  //Serial.print("line=");
  //Serial.println(line);
  strncpy(parsed->key, line, min(r,MAX_KV_LEN-1));
  r = readBytesUntil(file, "\n", line, MAX_KV_LEN-1);
  if( !r ){
    return -1;
  }
  //Serial.print("line=");
  //Serial.println(line);
  if( line[0] == '"' ){
    for(int i=1;i<r;i++){
      if( line[i] == '"' ){
        r = i;
      }
    }
    //error to not ahve a closing quote on the same line
    //error to have a double quote in the parse string (doesn't support escapes)
    strncpy(parsed->value, line+1, r-1 );
    parsed->value[r] = '\0';
  } else {
    strncpy(parsed->value, line, r-1 ); //skip the newline that gets included
    parsed->value[r] = '\0';
  }
  return 0;
}


void parse_wifi(FsFile * file){
  //sets global ssid and password for now
  //expects file like 
  //ssid="your wifi ssid here"
  //psk="your wifi password here
  //
  //spaces are not currently tolerated
  //
  if( ! file ){
    return;
  }
  kv read = {"",""};
  int e = parse_kvpair(file, &read);
  if( e != 0 ){
    //error
    return;
  }
  if( strncmp(read.key, "ssid", 4) == 0){
    strncpy(wifi_ssid, read.value, min(strlen(read.value),(size_t)MAX_KV_LEN-1));
  } else if (strncmp(read.key, "psk", 3) == 0){
    strncpy(wifi_psk, read.value, min(strlen(read.value),(size_t)MAX_KV_LEN-1));
  }
  e = parse_kvpair(file, &read);
  if( e != 0 ){
    //error
    return;
  }
  if( strncmp(read.key, "ssid", 4) == 0){
    strncpy(wifi_ssid, read.value, min(strlen(read.value),(size_t)MAX_KV_LEN-1));
  } else if (strncmp(read.key, "psk", 3) == 0){
    strncpy(wifi_psk, read.value, min(strlen(read.value),(size_t)MAX_KV_LEN-1));
  }
}
void parse_wifi_success(){
  Serial.println("Wifi Success!");
  Serial.print("ssid=");
  Serial.println(wifi_ssid);
  Serial.print("psk=");
  Serial.println(wifi_psk);
  WiFi.begin(wifi_ssid, wifi_psk); //DHCP
}


extern uint8_t mac[]; // = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void printmac(){
  for( int i = 0; i < 6; i++){
    Serial.print(mac[i],HEX);
    if( i != 5 ){
      Serial.print(":");
    }
  }
  Serial.println("");
}
int string2macaddress( uint8_t * mac, char * s){
  //s="8c:aa:b5:92:e4:04"
  int len = strlen(s);
  Serial.print("strlen(\"");
  Serial.print(s);
  Serial.print("\") == ");
  Serial.println(len);
  if( len != 12 && len != 17 ){ //unseparated ascii digits or bytes with separators ("DEADBEEFFEED" or "DE:AD:BE:...")
    Serial.println("Invalid mac address format");
    return -1;
  }
  int macidx = 0; //6 bytes, macidx in 0..11 where if it's even it zeros the byte, gets shifted and added, and if it's odd it just gets added
  for( int i = 0; i < strlen(s); i++){
    int c = s[i];
    int x = -1;
    if( c >= '0' && c <= '9' ){
      x = c-'0';
    }
    if( c >= 'A' && c <= 'F' ){
      x = c-'A' + 0xA;
    }
    if( c >= 'a' && c <= 'f' ){
      x = c - 'a' + 0xA; 
    }
    //Serial.print("i="); Serial.print(i,HEX);
    //Serial.print(", c="); Serial.print(c,HEX);
    //Serial.print(", x="); Serial.print(x,HEX);
    //Serial.println("");
    if( x != -1 ){
      int offset = macidx /2;
      if( macidx % 2 == 0 ){
        mac[offset] = 0;
        mac[offset] |= x << 4;
        macidx++;
      } else {
        mac[offset] |= x;
        macidx++;
      }
      //printmac();
    }
  }
  return 0;
}
void parse_mqtt(FsFile * file){
  //sets globals
  //expects file like 
  //host="hostname.example.com"
  //port=1883
  //
  //spaces are not currently tolerated
  //
  if( ! file ){
    return;
  }
}
void parse_mqtt_success(){
  Serial.println("MQTT config parsed!");
}
void parse_eth(FsFile * file){
  //sets globals
  //expects file like 
  //mac="DEADBEEFFEED"
  //
  //spaces are not currently tolerated
  //
  if( ! file ){
    return;
  }
  kv read = {"",""};
  int e = parse_kvpair(file, &read);
  if( e != 0 ){
    Serial.print("Error parsing in eth: ");
    Serial.println(e, HEX);
    return;
  }
  if( strncmp(read.key, "mac", 3) == 0){
    Serial.println("Read mac");
    string2macaddress( mac, read.value);
  } else {
    Serial.println("failed to read mac");
    Serial.print(read.key);
    Serial.print(" = ");
    Serial.println(read.value);
  }
}
void parse_eth_success(){
  Serial.println("eth config parsed!");
  Serial.print("mac=");
  printmac();
}


typedef void (*parser_cb)(FsFile* file);
typedef void (*success_cb)();
void parse_file_with(const char * filename, parser_cb parse, success_cb win){
  FsFile file;
  file.open(filename, FILE_READ); 
  if(!file){
    Serial.print("Failed to open ");
    Serial.print(filename);
    Serial.println(" for reading");
    return;
  }
  parse(&file);
  file.close();
  win();
}
void parse_all_config_files(){
    parse_file_with("/wifi.txt", parse_wifi, parse_wifi_success);
    parse_file_with("/mqtt.txt", parse_mqtt, parse_mqtt_success);
    parse_file_with("/eth.txt", parse_eth, parse_eth_success);
}
