#include "WiFi.h"
#include "EEPROM.h"
#define LENGTH(x) (strlen(x) + 1)   // length of char string
#define EEPROM_SIZE 200             // EEPROM size
#define WiFi_rst 25                  //WiFi credential reset pin (Boot button on ESP32)
String ssid;                        //string variable to store ssid
String pss;                         //string variable to store password
unsigned long rst_millis;
unsigned long working_millis;

#include <WiFiClientSecure.h>

#define API_KEY "AIzaSyBlka8m-3ElNKDZ306D6Tzz9LvgvUJw-z8" //API key de Youtube
#define CHANNEL_ID "UC8tBCcA_cSCWSO4XKhwzlPw" //ID del canal 

//const char* ssid     = "TVCABLE_ACSIMB";     // your network SSID (name of wifi network)
//const char* password = "fermu1234"; // your network password
//const char* ssid     = "RED FRANCISCO";     // your network SSID (name of wifi network)
//const char* password = "1802427491"; // your network password

const char*  server = "www.googleapis.com";  // Server URL
const char*  server2 = "www.anonigviewer.com";  // Server URL
const char*  server3 = "www.google.com";  // Server URL

WiFiClientSecure client;

uint32_t ContadorVistas, Suscriptores, OcultoSuscriptores, ContadorVideos;
uint32_t TimerLectura;
uint32_t Seguidores;
uint32_t Likes;


#include <WiFiUdp.h>
unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
uint32_t TimerSolicitarHora;
uint8_t WhileNTP;

void InitNTP (){
  // We start by connecting to a WiFi network
  udp.begin(localPort);
  Serial.print("Local port: ");
  //Serial.println(udp.localPort());
}
void sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
void NTP_TareaPeriodica (){
  WhileNTP=1;
  while (WhileNTP){
    if (TimerSolicitarHora == 1){
      WiFi.hostByName(ntpServerName, timeServerIP);
      sendNTPpacket(timeServerIP);
    }
    if (TimerSolicitarHora == 11){
      int cb = udp.parsePacket();
      if (!cb) {
        Serial.println("no packet yet");
      } else {
        Serial.print("packet received, length=");
        Serial.println(cb);
        // We've received a packet, read the data from it
        udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    
        //the timestamp starts at byte 40 of the received packet and is four bytes,
        // or two words, long. First, esxtract the two words:
    
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
        // combine the four bytes (two words) into a long integer
        // this is NTP time (seconds since Jan 1 1900):
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        Serial.print("Seconds since Jan 1 1900 = ");
        Serial.println(secsSince1900);
    
        // now convert NTP time into everyday time:
        Serial.print("Unix time = ");
        // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
        const unsigned long seventyYears = 2208988800UL;
        // subtract seventy years:
        unsigned long epoch = secsSince1900 - seventyYears;
        // print Unix time:
        Serial.println(epoch);
        // print the hour, minute and second:
        Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
        uint16_t Hora=(epoch  % 86400L) / 3600;
        if (Hora >= 5)
          Hora-=5;
        else{
          Hora+=12;
          Hora-=5;
        }
        Serial.print(Hora); // print the hour (86400 equals secs per day)
        
        Serial.print(':');
        if (((epoch % 3600) / 60) < 10) {
          // In the first 10 minutes of each hour, we'll want a leading '0'
          Serial.print('0');
        }
        uint16_t Minutos=(epoch  % 3600) / 60;
        Serial.print(Minutos); // print the minute (3600 equals secs per minute)
        Serial.print(':');
        if ((epoch % 60) < 10) {
          // In the first 10 seconds of each minute, we'll want a leading '0'
          Serial.print('0');
        }
        Serial.println(epoch % 60); // print the second
      }
    }
    if (TimerSolicitarHora == 101){
      TimerSolicitarHora=0;
      WhileNTP=0;
    }
    TimerSolicitarHora++;
    vTaskDelay(100);
  }
}


void writeStringToFlash(const char* toStore, int startAddr) {
  int i = 0;
  for (; i < LENGTH(toStore); i++) {
    EEPROM.write(startAddr + i, toStore[i]);
  }
  EEPROM.write(startAddr + i, '\0');
  EEPROM.commit();
}
String readStringFromFlash(int startAddr) {
  char in[128]; // char array of size 128 for reading the stored data 
  int i = 0;
  for (; i < 128; i++) {
    in[i] = EEPROM.read(startAddr + i);
  }
  return String(in);
}
void InitSmartConfig (){
  pinMode(WiFi_rst, INPUT);
  if (!EEPROM.begin(EEPROM_SIZE)) { //Init EEPROM
    Serial.println("failed to init EEPROM");
    delay(1000);
  }
  else
  {
    ssid = readStringFromFlash(0); // Read SSID stored at address 0
    Serial.print("SSID = ");
    Serial.println(ssid);
    pss = readStringFromFlash(40); // Read Password stored at address 40
    Serial.print("psss = ");
    Serial.println(pss);
  }

  WiFi.begin(ssid.c_str(), pss.c_str());

  delay(3500);   // Wait for a while till ESP connects to WiFi

  if (WiFi.status() != WL_CONNECTED) // if WiFi is not connected
  {
    //Init WiFi as Station, start SmartConfig
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();

    //Wait for SmartConfig packet from mobile
    Serial.println("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("SmartConfig received.");

    //Wait for WiFi to connect to AP
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("WiFi Connected.");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // read the connected WiFi SSID and password
    ssid = WiFi.SSID();
    pss = WiFi.psk();
    Serial.print("SSID:");
    Serial.println(ssid);
    Serial.print("PSS:");
    Serial.println(pss);
    Serial.println("Store SSID & PSS in Flash");
    writeStringToFlash(ssid.c_str(), 0); // storing ssid at address 0
    writeStringToFlash(pss.c_str(), 40); // storing pss at address 40
  }
  else
  {
    Serial.println("WiFi Connected");
    InitNTP();

  }
}
void RedesSocialesInit() {
  InitSmartConfig();
  //Initialize serial and wait for port to open:
 /* Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.println("\nStarting connection to server..."); */
}


uint8_t ObtenerSuscriptores (uint32_t* viewCount, uint32_t* subscriberCount, uint32_t* hiddenSubscriberCount, uint32_t* videoCount){
  char command[150] = "GET https://www.googleapis.com/youtube/v3/channels";
  char params[120];
  sprintf(params, "?part=statistics&id=%s&key=%s", CHANNEL_ID, API_KEY);
  strcat(command, params);
  strcat(command, " HTTP/1.0");
  Serial.println(command);

  client.setInsecure();//skip verification
  if (!client.connect(server, 443))
    //return -1;
    Serial.println("Connection failed YouTube!");
  else {
    Serial.println("Connected to server YouTube!");
    // Make a HTTP request:
    client.println(command);//client.println("GET https://www.googleapis.com/youtube/v3/channels?part=statistics&id=UC8tBCcA_cSCWSO4XKhwzlPw&key=AIzaSyBlka8m-3ElNKDZ306D6Tzz9LvgvUJw-z8 HTTP/1.0");
    client.println("Host: www.googleapis.com");
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    String BufferRx;
    while (client.available()) {
      char c = client.read();
      BufferRx+=c;
    }
    //Serial.println(BufferRx);
    String BFviewCount, BFsubscriberCount, BFhiddenSubscriberCount, BFvideoCount;
    uint16_t Index;
    
    Index = BufferRx.indexOf("\"viewCount\": \"") + sizeof ("\"viewCount\": \"") - 1;
    for (uint8_t i=0;i<15;i++){ 
      char c = BufferRx[Index+i];
      if (c == '\"'){
        break;
      }
      BFviewCount+=c;
    }
    Index = BufferRx.indexOf("\"subscriberCount\": \"") + sizeof ("\"subscriberCount\": \"") - 1;
    for (uint8_t i=0;i<15;i++){ 
      char c = BufferRx[Index+i];
      if (c == '\"'){
        break;
      }
      BFsubscriberCount+=c;
    }
    Index = BufferRx.indexOf("\"hiddenSubscriberCount\": ") + sizeof ("\"hiddenSubscriberCount\": ") - 1;
    for (uint8_t i=0;i<5;i++){ 
      char c = BufferRx[Index+i];
      if (c == 'f'){
        BFhiddenSubscriberCount+="false";
        break;
      }
      if (c == 't'){
        BFhiddenSubscriberCount+="true";
        break;
      }
    }
    Index = BufferRx.indexOf("\"videoCount\": \"") + sizeof ("\"videoCount\": \"") - 1;
    for (uint8_t i=0;i<15;i++){ 
      char c = BufferRx[Index+i];
      if (c == '\"'){
        break;
      }
      BFvideoCount+=c;
    }
    *viewCount = (uint32_t)BFviewCount.toInt();
    *subscriberCount = (uint32_t)BFsubscriberCount.toInt();
    if (BFhiddenSubscriberCount.equals("true"))
      *hiddenSubscriberCount=1;
    else
      *hiddenSubscriberCount=0;
    *videoCount = (uint32_t)BFvideoCount.toInt();
    client.stop();
    return 1; 
  }
}
uint8_t ObtenerInstagram (uint32_t* _Followers){
  //char command[150] = "GET https://www.instagram.com/acsimb_automatizacion_ec/";
  char command[200] = "GET https://www.anonigviewer.com/profile.php?u=viiptoys";
  char params[120];
  strcat(command, " HTTP/1.0");
  Serial.println(command);

  client.setInsecure();//skip verification
  if (!client.connect(server2, 443))
    //return -1;
    Serial.println("Connection failed Instagram!");
  else {
    Serial.println("Connected to server Instagram!");
    // Make a HTTP request:
    client.println(command);//client.println("GET https://www.googleapis.com/youtube/v3/channels?part=statistics&id=UC8tBCcA_cSCWSO4XKhwzlPw&key=AIzaSyBlka8m-3ElNKDZ306D6Tzz9LvgvUJw-z8 HTTP/1.0");
    client.println("Host: www.anonigviewer.com");
    client.println("Connection: close");
    client.println();
    vTaskDelay(2000);
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      //Serial.println(line);
      //Serial.println(line);
      if (line.indexOf(">Posts<") != -1){
        Serial.println("POSTS Instagram");
        Serial.println(line);
        while (client.connected()) {
          String line2 = client.readStringUntil('\n');
          if (line2.indexOf("<span class=\"color-") != -1){
              Serial.println("linea Instagram");
              Serial.println(line2);
             while (client.available()) {
                char dummy= client.read();
               }
             String SubStringFollower = line2.substring(line2.indexOf("\">") + 2,line2.indexOf("</span>"));
             //Serial.println(SubStringFollower);
             SubStringFollower.remove(SubStringFollower.length() - 4, 1);
             //Serial.println(SubStringFollower);
             *_Followers = (uint32_t)SubStringFollower.toInt();
            }
        }
       }
    }
    client.stop();
    return 1; 
  }
}
uint8_t ObtenerFacebook (uint32_t* _Likes){
  //char command[150] = "GET https://www.instagram.com/acsimb_automatizacion_ec/";
  char command[800] = "GET https://www.google.com/search?q=facebook+viiptoys+about&rlz=1C1CHBD_esEC904EC904&sxsrf=AOaemvIQ7uLnSWC9xPuH8NN_H3SZDq5ZwQ%3A1639703887139&ei=T-W7Yb6UCMmCwbkP8aq_gAQ&ved=0ahUKEwj-9-r91On0AhVJQTABHXHVD0AQ4dUDCA4&uact=5&oq=facebook+viiptoys+about&gs_lcp=Cgdnd3Mtd2l6EAM6BwgjELADECc6BwgAEEcQsANKBAhBGABKBAhGGABQyAtYvBpgzh5oAnACeACAAZ4BiAGoCZIBAzAuOZgBAKABAcgBCcABAQ&sclient=gws-wiz";
  char params[120];
  strcat(command, " HTTP/1.0");
  //Serial.println(command);

  client.setInsecure();//skip verification
  if (!client.connect(server3, 443))
    //return -1;
    Serial.println("Connection failed Facebook!");
  else {
    Serial.println("Connected to server Facebook!");
    // Make a HTTP request:
    client.println(command);//client.println("GET https://www.googleapis.com/youtube/v3/channels?part=statistics&id=UC8tBCcA_cSCWSO4XKhwzlPw&key=AIzaSyBlka8m-3ElNKDZ306D6Tzz9LvgvUJw-z8 HTTP/1.0");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
    vTaskDelay(2000);
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      //Serial.println(line);
      if (line.indexOf("likes") != -1){
        Serial.println("Encontrado Facebook");
        //Serial.println(line);
        while (client.available()) {
          char dummy= client.read();
         }
         String SubStringLikes = line.substring(line.indexOf(" likes") - 8,line.indexOf(" likes"));
         
         String NumLikes = SubStringLikes.substring(SubStringLikes.indexOf(" ")+1, SubStringLikes.length());
                  
         //Serial.println(SubStringLikes);
         //Serial.println(NumLikes);
        
         //Serial.println("convertido");
         *_Likes = (uint32_t)NumLikes.toInt();
       }
    }
    client.stop();
    return 1; 
  }
}
void RedesSocialesTask() {
  rst_millis = millis();
  while (digitalRead(WiFi_rst) == HIGH)
  {
    // Wait till boot button is pressed 
  }
  // check the button press time if it is greater than 3sec clear wifi cred and restart ESP 
  if (millis() - rst_millis >= 3000)
  {
    Serial.println("Reseting the WiFi credentials");
    writeStringToFlash("", 0); // Reset the SSID
    writeStringToFlash("", 40); // Reset the Password
    Serial.println("Wifi credentials erased");
    Serial.println("Restarting the ESP");
    delay(500);
    ESP.restart();            // Restart ESP
  }

  
  if ((working_millis % 60 ) == 0){
    if (ObtenerSuscriptores(&ContadorVistas, &Suscriptores, &OcultoSuscriptores, &ContadorVideos)){
      Serial.println("Datos Obtenidos del canal:");
      Serial.println(ContadorVistas);
      Serial.println(Suscriptores);
      Serial.println(OcultoSuscriptores);
      Serial.println(ContadorVideos);
    }
    else{
      Serial.println("Conexion fallida Youtube");
    }  
    if (ObtenerInstagram(&Seguidores)){
      Serial.println("Datos Obtenidos del Instagram:");
      Serial.println(Seguidores);
     }
    else{
      Serial.println("Conexion fallida Instagram");
    } 
    if (ObtenerFacebook(&Likes)){
      Serial.println("Datos Obtenidos del Facebook:");
      Serial.println(Likes);
     }
    else{
      Serial.println("Conexion fallida Facebook");
    }
  }
  working_millis++;
  vTaskDelay(1000);
}
