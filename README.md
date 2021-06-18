# SatTracker_esp32_v2
an improved version

on code :

adding #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
-
-
Change the location of your screen setup
This is useful if you update the library. You save your config on another place.
#include <../TFT_eSPI_Setups/User_Setup_st7789.h>    // this is usefull for opening a custom library setup
-
-
-
tracking many satellite on screen. search for the TLE sat code on the web (celestrak)

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
-
-
-
-
zomming function when ISS is above Europe. you must add one BMP map

on setting.h 
DEBUG mode
LOOP mode

If you uncomment ligne 13 : #define TIMELOOP   // uncomment for having a lopp from the same epochtime continuously for debuging
you have a time lapse of the same day

