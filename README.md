# SatTracker_esp32_v2
an improved version

on code :

adding #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

change the location of your screen setup

#include <../TFT_eSPI_Setups/User_Setup_st7789.h>    // this is usefull for opening a custom library setup


tracking many satellite on screen

zomming function when ISS is above Europe. you must add one BMP map

on setting.h 
DEBUG mode
LOOP mode

If you uncomment ligne 13 : #define TIMELOOP   // uncomment for having a lopp from the same epochtime continuously for debuging
you have a time lapse of the same day

