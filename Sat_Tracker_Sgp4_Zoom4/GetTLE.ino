void GetTLE(int16_t i) {
//
// Connecting to Cletrak for getting TwoLine element (TLE)
// HTTP request
String SatUrl = String(CelestrakUrl) + String(SatTleURL[i]);  // this is the complete url search for this "Sat"
Serial.println(SatUrl); 

HTTPClient http;
http.begin(SatUrl);
int httpCode = http.GET();

if (httpCode > 0) { //Check for the returning code
  String payload = http.getString();
  String TLEName = payload.substring(0,20);
  String TLE1 =    payload.substring(26,96);
  String TLE2 =    payload.substring(97,167);
  
  Serial.println("Payload from Celestrak");
  Serial.println(TLEName);
  Serial.println(TLE1); 
  Serial.println(TLE2);
  Serial.println("end of string...");

  Serial.println("-----------------------------------------------");

  TLEName.toCharArray(TLENameChar[i],21); // récupère le param dans le tableau de char  
  TLE1.toCharArray(TLE1Char[i],71); // récupère le param dans le tableau de char
  TLE2.toCharArray(TLE2Char[i],71); // récupère le param dans le tableau de char

#ifdef DEBUG
  Serial.print("TLENameChar = ");
  Serial.println(TLENameChar[i]); // affiche le tableau de char
  Serial.print("TLE1Char = ");
  Serial.println(TLE1Char[i]); // affiche le tableau de char
  Serial.print("TLE1Char = ");
  Serial.println(TLE2Char[i]); // affiche le tableau de char
  Serial.println("----------------------------------------------");
#endif

    tft.drawString(TLENameChar[i], 20, 150+(i*14), FONT2);

  }else {
      Serial.println("Error on HTTP request");
  }

http.end(); //Free the resources
}
