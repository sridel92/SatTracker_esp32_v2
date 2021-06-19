
// *******************************************************************
// Draw a + mark centred on x,y
void drawMarker(int x, int y)
{
  //tft.drawLine(x - 4, y, x + 4, y, TFT_BLUE);
  //tft.drawLine(x, y - 4, x, y + 4, TFT_BLUE);
  tft.drawCircle(x, y, 1, TFT_BLUE);
}

// *******************************************************************
// Draw a + mark centred on x,y
// i is the sat number and set the color of the mark
//
void drawMarkerColor(int x, int y, int i)
{

  
  switch (i) {
    case 0:
      tft.drawLine(x - 2, y, x + 2, y, TFT_BLUE);
      tft.drawLine(x, y - 1, x, y + 1, TFT_BLUE);
      tft.fillRect(xd, yd, 25, 4, TFT_BLUE);            // fillRect(0, 0, _width, _height, color);
      tft.drawString(TLENameChar[i], xd+30, yd);
      tft.drawNumber(x, xd+90, yd);
      tft.drawNumber(y, xd+110, yd);
      break;
    
    case 1:
      tft.drawLine(x-1, y, x+1, y, TFT_RED);
      tft.drawLine(x, y-1, x, y+1, TFT_RED);
      tft.fillRect(xd, yd+15, 25, 4, TFT_RED);
      tft.drawString(TLENameChar[i], xd+30, yd+15);
      tft.drawNumber(x, xd+90, yd+15);
      tft.drawNumber(y, xd+110, yd+15);
      break;
    
    case 2:
      tft.drawLine(x, y, x, y, TFT_YELLOW);
      tft.drawLine(x, y, x, y, TFT_YELLOW);
      tft.fillRect(xd, yd+30, 25, 4, TFT_YELLOW);
      tft.drawString(TLENameChar[i], xd+30, yd+30);
      tft.drawNumber(x, xd+90, yd+30);
      tft.drawNumber(y, xd+110, yd+30);  
      break;
      
    case 3:
      tft.drawLine(x, y, x, y, TFT_ORANGE);
      tft.drawLine(x, y, x, y, TFT_ORANGE);
      tft.fillRect(xd, yd+45, 25, 4, TFT_ORANGE);
      tft.drawString(TLENameChar[i], xd+30, yd+45);
      tft.drawNumber(x, xd+90, yd+45);
      tft.drawNumber(y, xd+110, yd+45);
      break;
      
    case 4:
      tft.drawLine(x, y, x, y, TFT_WHITE);
      tft.drawLine(x, y, x, y, TFT_WHITE);
    break;
    
    case 5:
      tft.drawLine(x, y, x, y, TFT_BLACK);
      tft.drawLine(x, y, x, y, TFT_BLACK);
    break;
    
    case 6:
      tft.drawLine(x, y, x, y, TFT_CYAN);
      tft.drawLine(x, y, x, y, TFT_CYAN);
    break;

    default:
    // if nothing else matches, do the default
    // default is optional
    break;
  }
}


// *******************************************************************
void DrawMap(){
  
  // map is 320 x 160 this is the main wolrd MAP on memory
  drawBmp("/Map4.bmp", 0, 0);

  int xMyPos = 160+dMyLON; //
  int yMyPos = 90-dMyLAT; //
    tft.drawLine(xMyPos - 15, yMyPos, xMyPos + 15, yMyPos, TFT_YELLOW);
    tft.drawLine(xMyPos, yMyPos - 15, xMyPos, yMyPos + 15, TFT_YELLOW);
    tft.drawRect(180-30, 30, 50, 45, TFT_YELLOW);

  // Draw 2 lines for lat 0 longitude 0
  tft.drawLine(160, 0, 160, 180, TFT_RED);
  tft.drawLine(0, 90, 320, 90, TFT_RED);
}


// *******************************************************************
void DrawMapEurope() {
  drawBmp("/MapEuropeNew.bmp", 0, 0);
    tft.fillRect(300,0, 320,240, TFT_BLACK);
  //
  // Europe croping is
  // startin at lat 75° to lat 30°
  // ending at lon : -15 to lon 45°
  // 
  // screen is 300 x 225
  //   
  // drawCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color),
// double  dMyLAT  =  48.91052;  // Latitude  - Asnières (France): N -> +, S -> -
// double  dMyLON  =   2.29100;  // Longitude - Asnières (France): E -> +, W -> -
  int xMyPos = ( 2.29100 + 15 ) * 5; //
  int yMyPos = ( 60 - 48.91052 ) * 5; //
  tft.drawCircle(xMyPos, yMyPos, 2, TFT_YELLOW);
  tft.drawCircle(xMyPos, yMyPos, 6, TFT_YELLOW);
  tft.drawCircle(xMyPos, yMyPos, 8, TFT_YELLOW);
/*
  //test pointing some known aera on the Europe
  for (int b = 0; b < 61; b = b + 15) {
    for (int c = 0; c < 46; c = c + 15) {
      tft.drawCircle(b * 5, c * 5, 5, TFT_YELLOW);
    }
  }
*/
  int LonA =round( (-23.6772 + 30 ) * 5); // //64.757511, -23.677213 (west iceland)
  int LatA =round( (  60 -64.7575 ) * 5);
  tft.drawCircle(LonA, LatA, 4, TFT_RED);
  LonA =round( ( -10.3207 + 15 ) * 5); // //51.849399, -10.320701 (Kerry Ireland)
  LatA =round( ( 60 - 51.8493 ) * 5);
  tft.drawCircle(LonA, LatA, 4, TFT_RED);
  LonA =round( ( 18.173264 + 15 ) * 5); // //40.357003, 18.173264 (LECCE ITALIE)
  LatA =round( ( 60 - 40.357003 ) * 5);
  tft.drawCircle(LonA, LatA, 4, TFT_RED);
// Draw 1 line for longitude 0
  tft.drawLine(15*5,  0, 15*5, 225, TFT_RED);
}

// *******************************************************************
void DrawMapJapan() {
  drawBmp("/MapJapan300.bmp", 0, 0);
  //
  //  croping is
  // startin at lat 75° to lat 30°
  // ending at lon : 105 to lon 165°
  // 
  // screen is 300 x 225
  //   
  // drawCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color),
// double  dMyLAT  =  48.91052;  // Latitude  - Asnières (France): N -> +, S -> -
// double  dMyLON  =   2.29100;  // Longitude - Asnières (France): E -> +, W -> -
  
  int xMyPos = ( 2.29100 + 30 ) * 5; //
  int yMyPos = ( 75 - 48.91052 ) * 5; //
  
  tft.drawCircle(xMyPos, yMyPos, 2, TFT_RED);
  tft.drawCircle(xMyPos, yMyPos, 6, TFT_RED);
}

// *******************************************************************
// ***** Mark a dot when ISS is above my head
//
void SatAlert()
{
//dMyLAT  =  48.91052;  // Latitude  - Asnières (France): N -> +, S -> -
//dMyLON  =   2.29100;  // Longitude - Asnières (France): E -> +, W -> -
//sat.satLon
//sat.satLat
/*
if (digitalRead(2) == HIGH && digitalRead(3) == HIGH) { // if BOTH the switches read HIGH
*/

//       47     <     50 + 5  et      47 >          50 - 5          
if ( sat.satLon < 50  )
    {
      //tft.drawCircle(200, 200, 20, TFT_YELLOW);
      Serial.println("yellow");
    }
    else
    {
      //tft.drawCircle(200, 200, 20, TFT_RED);
      Serial.println("red.....");
    }
}

// *******************************************************************
// ***** splash screen
//
void SplashScreen(){
  //
  
}
