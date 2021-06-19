// ******************** //
// ***** Setup    ***** //
// ******************** //

#include <Arduino.h>
#include "TZ.h"
#define WIFI_HOSTNAME "S"

// DEBUGGING
//#define DEBUG      //uncomment for Enable serial output 
// beware of the serial ouput, this could avoid uploading the programm and you have to press EN and BOOT Button during the uplaoding

//#define TIMELOOP   // uncomment for having a lopp from the same epochtime continuously for debuging

// NTP Servers:
static const char ntpServerName[] = "pool.ntp.org";

const int timeZone = 1;   // Central European Time

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

#define MAP_MAXX   320
#define MAP_MAXY   240

const char  *pcMyName = "OriginalStefRid";    // Observer name

// Gps coordinate of observer
double  dMyLAT  =  48.91052;  // Latitude  - Asnières (France): N -> +, S -> -
double  dMyLON  =   2.29100;  // Longitude - Asnières (France): E -> +, W -> -
double  dMyALT  =  20.0;      // Altitude ASL (m)

/**************************
 * End Settings ***********
 **************************/
