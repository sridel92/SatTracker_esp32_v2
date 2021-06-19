# SatTracker_esp32_v2
an improved version     ( Version Zoom5 )

On code :

adding #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


Change the location of your screen setup
This is useful if you update the library. You save your config on another place.
#include <../TFT_eSPI_Setups/User_Setup_st7789.h>    // this is usefull for opening a custom library setup


Screen wire : disconnect the GPIO2 to the screen and use another GPIO16
on screen lib : #define TFT_DC 16
the GPIO2 is used by onboard LED on some ESP32

my example :
// Generic ESP32 setup for WAVESHARE ST7789v
#define TFT_MISO -1 //  non connected
#define TFT_MOSI 23 // -> DIN SPI DATA INPUT   
#define TFT_SCLK 18 // -> CLK SPI CLOCK INPUT
#define TFT_CS   15  // -> CS chip selection Not connected
#define TFT_DC   16   // -> DC
#define TFT_RST  4  // -> RST Connect reset to ensure display initialises



Switch boot

-GPIO12 - switch for rotation screen at startup (connect it to the ground about 5 seconds during boot)

-GPIO13 - switch for LOOP testing at startup  (connect it to the ground about 5 seconds during boot)





Tracking many satellite on screen. search for the TLE sat code on the web (celestrak)

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



zomming function when ISS is above Europe. you must add one BMP map


on setting.h 
-DEBUG mode
-LOOP mode


If you uncomment ligne 13 : #define TIMELOOP   // uncomment for having a lopp from the same epochtime continuously for debuging

you have a loop of the same day

