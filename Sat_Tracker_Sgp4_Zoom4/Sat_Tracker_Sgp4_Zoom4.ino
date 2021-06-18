   /* ====================================================================
   Copyright (c) 2019 Thorsten Godau (dl9sec). All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. Neither the name of the author(s) nor the names of any contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR(S) OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
   SUCH DAMAGE.
   ====================================================================*/
 
//
//https://github.com/Hopperpop/Sgp4-Library/tree/master/examples
//
// Call up the SPIFFS FLASH filing system this is part of the ESP Core
#define FS_NO_GLOBALS
#include <FS.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// for the BMP files
// So I used image magick with the following settings to get the proper color bit depth and compression:
// magick convert -depth 24 -compress none -background black -alpha remove image.bmp new-image.bmp
////////////////////////////////////////////////////////

#ifdef ESP32
  #include "SPIFFS.h"  // For ESP32 only
#endif

//#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"
#include <TimeLib.h>
#include <WiFiUdp.h>
#include "settings.h"                               // Important: see settings.h to configure your settings!!!
#include <../TFT_eSPI_Setups/User_Setup_st7789.h>    // this is usefull for opening a custom library setup
time_t getNtpTime();
#include <Sgp4.h>
#include <Ticker.h>   //https://github.com/sstaub/Ticker

Sgp4 sat;
Ticker tkSecond;  // this lib create some task at schedule time (second...)

#include "Free_Fonts.h"       // Include the header file attached to this sketch
#include <TFT_eSPI.h>         // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();    // Invoke custom library

int year_;
int mon_; 
int day_; 
int hr_; 
int min_; 
double sec_;
long        count    = 0;           // Loop count
double      dSatLAT  = 0;           // Satellite latitude
double      dSatLON  = 0;           // Satellite longitude
double      dSatAZ   = 0;           // Satellite azimuth
double      dSatEL   = 0;           // Satellite elevation
double      dSunLAT  = 0;           // Sun latitude
double      dSunLON  = 0;           // Sun longitude
double      dSunAZ   = 0;           // Sun azimuth
double      dSunEL   = 0;           // Sun elevation
char        acBuffer[20];           // Buffer for ASCII time
int         xpos      = 190;        // Position of watch
int         xposInit  = 0;
int         ypos      = 200;        // Position of watch
int         ysecs     = ypos;
int         xyfont    = 4;
int         xd        = 15;
int         yd        = 190;
int         IssLat    = 0;
int         IssLon    = 0;
byte        xcolon = 0, xsecs = 0;
uint32_t    targetTime = 0;                 // for next 1 second timeout
unsigned long unixtime = 1617906546;
//int timezone = 12 ;                       //utc + 12
int framerate;
static uint8_t conv2d(const char* p);       // Forward declaration needed for IDE 1.6.x

const int ledPin =  34;                     // the number of the LED pin
int ledState = LOW;                         // ledState used to set the LED

unsigned int colour = 0;
const long Blink    = 1000;                 // time to refresh screen (milliseconds)
unsigned long RefreshPreviousMillis = 0;    // will store last time screen was was refreshed
unsigned long BlinkPreviousMillis = 0;      // will store last time screen was was refreshed
unsigned long previousMillis = 0;        
unsigned long interval = 1000;

// *************************************************************
String CelestrakUrl = "http://104.168.149.178";      //Web address to get TLE (CELESTRAK)
char TLENameChar[][21] =  { "sat0", "sat1", "sat2", "sat3", "sat3", "sat3" };
char TLE1Char[][71] =     { "sat0", "sat1", "sat2", "sat3", "sat3", "sat3" };
char TLE2Char[][71] =     { "sat0", "sat1", "sat2", "sat3", "sat3", "sat3" };

int Sat = 4;        //  Enter the number of tracked satellite
// Enter the code of the tracked satellites from Celestrak
char *SatTleURL[] = {
    "/satcat/tle.php?CATNR=25544",
    "/satcat/tle.php?CATNR=48274",
    "/satcat/tle.php?CATNR=33591",
    "/satcat/tle.php?CATNR=43013",
    "/satcat/tle.php?CATNR=11060",
    "/satcat/tle.php?CATNR=42982",
    };
/*
 * 25544 : ISS
 * 48209 : Crew dragon2
 * 42982 : KESTREL EYE KE2M
 * 28654 : NOAA 18 (B)
 * 33591 : NOAA 19
 * 43013 : NOAA 20
 * 11060 : TIROS N
 * 48274 : TIANHE
 */

HTTPClient client;
String HourStr[10];

const char* ntpServer = "pool.ntp.org";   // NTP server to request epoch time
unsigned long epochTime;                  // Variable to save current epoch time
unsigned long getTime() {                 // Function that gets current epoch time
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
  Ticker tkSecond;  // this lib create some task at schedule time (second...)
}

void setup()
{
  WiFi.mode(WIFI_STA);         // explicitly set mode, esp defaults to STA+AP
  Serial.begin(115200);
  WiFiManager wm;

      tft.begin();
      tft.setRotation(1);                 // landscape
      tft.fillScreen(TFT_BLACK);
      tft.setFreeFont(TT1);               // Select the orginal small TomThumb font
      tft.drawString("Sat Tracker portal", 20, 80, FONT2);
      tft.drawString("Use mobile or laptop", 20, 100, FONT2);
      tft.drawString("search for wifi lan 'Esp32_SatPortal' ", 20, 120, FONT2);
      tft.drawString("-",   20, 140, FONT2);
      tft.drawString("--",  20, 150, FONT2);
      tft.drawString("---", 20, 160, FONT2);

    //reset settings - wipe credentials for testing
    //wm.resetSettings();     // uncomment for reset settings - wipe credentials for testing

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
 // res = wm.autoConnect();                          // auto generated AP name from chipid
    res = wm.autoConnect("Esp32_SatPortal");            // anonymous ap
 // res = wm.autoConnect("Esp32_SatPortal","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        tft.drawString("--", 20, 150, FONT2);
        tft.drawString("---> connected to residential Wifi", 20, 160, FONT2);
        delay(1000);
        tft.fillScreen(TFT_BLACK);
    }

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield();        // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nSPIFFS initialised.");

  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);

//  Slapsh screen
  drawBmp("/ISS_20years.bmp", 0, 0);  // 320x106
  delay(500);
  tft.setFreeFont(TT1);               // Select the orginal small TomThumb font
  tft.drawString("Satellite tracker", 20, 120, FONT2);
  delay(1000);

  tft.fillRect(0,107, 320,240, TFT_BLACK);
  delay(500);
  tft.fillRect(0,0, 320,106, TFT_WHITE);
  delay(500);

  //drawBmp("/Ntp.bmp", 20, 10); // 100x72 
  tft.drawString("I'm getting time from NTP server...", 10, 130, FONT2);
  
  configTime(0, 0, ntpServer);
  epochTime = getTime();
  tft.drawNumber(epochTime, 10, 170, FONT4);  //ok
  delay(1000);
  tft.drawString("This is the Unixtime", 100, 210, FONT2);
  delay(1000);
  tft.fillRect(0,107, 320,240, TFT_BLACK);
  delay(500);
  drawBmp("/Celestrak.bmp", 0, 0);
  delay(500);
  tft.drawString("Connecting to Celestrak", 20, 120, FONT2);
  delay(500);
  tft.drawString("Loocking for the lastest TLE", 20, 135, FONT2);


// Here we are going the download the TLE definition of each Satellite
for (int i = 0; i < Sat; i++) {
    #ifdef DEBUG
    Serial.println("");
    Serial.print("Main : Satellite number ");
    Serial.println(i);
    Serial.println("");
    Serial.println("Main ----------> go to GetTLE <------------------");
    #endif
    GetTLE(i);
  }


  tft.drawString("TLE1 & 2 uplaoded...", 160, 210, FONT2);
  delay(500);
  tft.fillRect(0,0, 320,120, TFT_WHITE);
  delay(500);
  tft.fillRect(0,121, 320,240, TFT_BLACK);
  delay(500);
  drawBmp("/Nasa.bmp", 0, 122);     // 120x102
  delay(1000);
  drawBmp("/Esa.bmp", 130, 125);    // 132x60
  delay(1000);
  drawBmp("/CsaAsc.bmp", 0, 5);     // 110x106
  delay(1000);
  drawBmp("/Jaxa.bmp", 120, 10);    // 120x77
  delay(1000);
  tft.fillRect(0,0, 320,120, TFT_WHITE);
  delay(200);
  tft.fillRect(0,121, 320,240, TFT_BLACK);
  delay(500);
  drawBmp("/MissionAlpha.bmp", 20, 70);
  delay(1000);
  drawBmp("/Crew2.bmp", 0, 0);
  delay(2000);

  int i = 0;

  sat.site(dMyLAT, dMyLON  ,dMyALT); //set site latitude[°], longitude[°] and altitude[m]
  
  //Display TLE epoch time
  double jdC = sat.satrec.jdsatepoch;
  invjday(jdC , timeZone, true, year_, mon_, day_, hr_, min_, sec_);
 
  tkSecond.attach(1,Second_Tick);  
  targetTime = millis() + 1000;
  tft.fillScreen(TFT_BLACK);

  epochTime = 1619830800; 

} // EndSetup
// *************************

void loop()
{
  IssLat = 0;
  IssLon = 0;
    #ifdef TIMELOOP  // to create a time loop, timelapse from the same date
    epochTime = epochTime + 5;
    #else
    epochTime = getTime();
    #endif

  DrawWatch();
  tft.setFreeFont(TT1);     // Select the orginal small TomThumb font
  int i = 0;
  int b = 0;
  int c = 0;
  int ZoomLatStart = 60;
  int ZoomLatEnd   = ZoomLatStart -45 ;
  int ZoomLonStart = -15;  // value -15
  int ZoomLonEnd   = ZoomLonStart + 60;   // value is 45
//
//
//
//                                                   ******************
DrawMap();  // *****************************************   World Map *********************
//                                                   ******************

while ( ( IssLon < ZoomLonStart && IssLat < ZoomLatStart ) or IssLon > ZoomLonEnd or IssLat < ZoomLatEnd ) {
  #ifdef TIMELOOP  // to create a time loop, timelapse from the same date
  epochTime = epochTime + 5;
  #else
  epochTime = getTime();
  #endif

  for (i=0; i<Sat; i++) {
    tft.setFreeFont(TT1); 
    sat.init(TLENameChar[i],TLE1Char[i],TLE2Char[i]);     // initialize satellite parameters 
    sat.findsat(epochTime);
    
      if (i==0) {
         IssLat = sat.satLat;
         IssLon = sat.satLon;
        }
        
    float xx = ( ( sat.satLon + 180 ) *  320 ) / 360 ;    // Longitude is 360 wide but screen 320 pixels
    float yy = 0;
    int xTft = xx;
    int yTft = (90 - sat.satLat);
    drawMarkerColor( xTft, yTft, i);
    }

    #ifdef DEBUG
    Serial.print("do world =  ");
    Serial.print(b);
    b++;
    Serial.print(" -->> lat : ");
    Serial.print(sat.satLat);
    Serial.print(" - Lon : ");
    Serial.println(sat.satLon);
    #endif

    DrawWatch();
  }

//tft.fillScreen(TFT_BLACK);
//
//
//
//                                                          ******************
DrawMapEurope();  // *****************************************   Europe Map *********************
//                                                          ******************
// europe map is starting at lat : 75N and lon : -30W
//                 ending at lat : 30N and lon :  30E
// map is 60° wide and 45° high
// but the screen is from 0 to 300 pixel wide    =  Lon  = xTFT
//                        0 to 225 pixels high   =  Lat  = yTFT
// 
// sgp4 is provide Lat values from -180 to 180 and Lon values from 9à to -90
// for sat.lon value - xTFT = (sat.satLon + 30 ) * 5
//     sat.Lat value - yTft = (sat.satLat -15 ) * 5
//
  while  (  IssLon + 1 >= ZoomLonStart ) {
 
    #ifdef TIMELOOP  // to create a time loop, timelapse from the same date
    epochTime = epochTime + 5;
    #else
    epochTime = getTime();
    #endif

    sat.init(TLENameChar[0],TLE1Char[0],TLE2Char[0]);     // initialize satellite parameters 
    sat.findsat(epochTime);
    IssLat = sat.satLat;
    IssLon = sat.satLon;
    int xx = ( sat.satLon - ZoomLonStart) * 5 ;           // Longitude is 360 wide but screen 320 pixels
    int yy = ( 60 - sat.satLat ) * 5 ;
    int xTft = xx;
    int yTft = yy;
    tft.drawCircle(xTft, yTft, 3, TFT_BLUE);
    
    #ifdef DEBUG // serial is sending lat and lon
    Serial.print("Europe = ");
    Serial.print(c);
    c++;
    Serial.print("-> lat: ");
    Serial.print(sat.satLat);
    Serial.print(" y = ");
    Serial.print(yTft);
    Serial.print(" / Lon: ");
    Serial.print(sat.satLon);
    Serial.print(" x = ");
    Serial.println(xTft);
    #endif
      tft.setFreeFont(TT1);                            // Select the orginal small TomThumb font
      tft.fillRect(0, yd, 25, 3, TFT_BLUE);            // fillRect(0, 0, _width, _height, color);
      tft.drawString(TLENameChar[i], 30, yd);
      tft.drawNumber(sat.satLon,     90, yd);
      tft.drawNumber(sat.satLat,    110, yd);
       
      if (  IssLon >= ZoomLonEnd or IssLat <= ZoomLatEnd ) {
        break;
      }
    DrawWatch();

      #ifdef TIMELOOP  // to create a time loop, timelapse from the same date
        delay(50);
      #endif
    
  }
  tft.fillRect(0,120, 320,240, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  //framerate += 1;
} // end of loop


// *******************************************************************
void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


// *******************************************************************
// Progress bar helper
void drawProgress(uint8_t percentage, String text) {
"xxx";
}


// *******************************************************************
/*-------- NTP code ----------*/
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// *******************************************************************
// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

// *******************************************************************
// Function to extract numbers from compile time string
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

// *******************************************************************
void DrawWatch(){    
  tft.setFreeFont(TT1);           // Select the font
  // Draw hours and minutes
  xposInit = xpos;
    if (hr_ < 10) xpos  += tft.drawChar('0',    xpos, ypos, xyfont);     // Add hours leading zero for 24 hr clock
      xpos              += tft.drawNumber(hr_,  xpos, ypos, xyfont);     // Draw hours
      xcolon = xpos;                                                     // Save colon coord for later to flash on/off later
      xpos              += tft.drawChar(':',    xpos, ypos, xyfont);     // adding a char space for ':' in xpos
    if (min_ < 10) xpos += tft.drawChar('0',    xpos, ypos, xyfont);     // Add minutes leading zero
      xpos              += tft.drawNumber(min_, xpos, ypos, xyfont);     // Draw minutes
      xpos              += tft.drawChar(':',    xpos, ypos, xyfont);     // adding a char space for ':' in xpos
      xsecs = xpos;                                                      // Save seconds 'x' position for later display updates
    if (sec_ < 10) xpos += tft.drawChar('0',    xpos, ypos, xyfont);     // Add leading zero
                           tft.drawNumber(sec_, xpos, ypos, xyfont);     // Draw seconds
   xpos = xposInit;
}



// *******************************************************************
void Second_Tick() {
  unixtime += 1;      
  invjday(sat.satJd , timeZone,true, year_, mon_, day_, hr_, min_, sec_);
  #ifdef DEBUG
  Serial.println(String(day_) + '/' + String(mon_) + '/' + String(year_) + ' ' + String(hr_) + ':' + String(min_) + ':' + String(sec_));
  Serial.println("azimuth = " + String( sat.satAz) + " elevation = " + String(sat.satEl) + " distance = " + String(sat.satDist));
  Serial.println("latitude = " + String( sat.satLat) + " longitude = " + String( sat.satLon) + " altitude = " + String( sat.satAlt));
  switch(sat.satVis){
    case -2:
      Serial.println("Visible : Under horizon");
      //tft.setFreeFont(FF1);                 // Select the font
      //tft.drawString("Visible : Under horizon", 150, 210, 2);
    break;
    case -1:
      Serial.println("Visible : Daylight");
      //tft.setFreeFont(FF1);                 // Select the font
      //tft.drawString("Visible : Daylight", 150, 210, 2);
      break;
    default:
      Serial.println("Visible : " + String(sat.satVis));   //0:eclipsed - 1000:visible
      //tft.setFreeFont(FF1);                                // Select the font
      //tft.drawString("*** Visible ***", 200, 230, 2);
      //tft.drawFloat(sat.satVis, 190, 210, 2);
      break;
  }

  Serial.println("Framerate: " + String(framerate) + " calc/sec");
  Serial.println();
  #endif
  framerate=0;
}


// *******************************************************************
// There follows a crude way of flagging that this example sketch needs fonts which
// have not been enbabled in the User_Setup.h file inside the TFT_HX8357 library.
//
// These lines produce errors during compile time if settings in User_Setup are not correct
//
// The error will be "does not name a type" but ignore this and read the text between ''
// it will indicate which font or feature needs to be enabled
//
// Either delete all the following lines if you do not want warnings, or change the lines
// to suit your sketch modifications.

#ifndef LOAD_GLCD
//ERROR_Please_enable_LOAD_GLCD_in_User_Setup
#endif

#ifndef LOAD_GFXFF
ERROR_Please_enable_LOAD_GFXFF_in_User_Setup!
#endif
