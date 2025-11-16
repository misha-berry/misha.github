#include <Arduino.h>
#include <SD.h>//SdFat SD;
#include <Wire.h> //Needed for I2C to GNSS

#include <SparkFun_u-blox_GNSS_v3.h> //http://librarymanager/All#SparkFun_u-blox_GNSS_v3

SFE_UBLOX_GNSS myGNSS;
int fileWork = 0;
// Callback: printPVTdata will be called when new NAV PVT data arrives
// See u-blox_structs.h for the full definition of UBX_NAV_PVT_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setAutoPVTcallback
//        /                  _____  This _must_ be UBX_NAV_PVT_data_t
//        |                 /               _____ You can use any name you like for the struct
//        |                 |              /
//        |                 |              |
void printPVTdata(UBX_NAV_PVT_data_t *ubxDataStruct)
{
  File myFile = SD.open("test.txt", FILE_WRITE);

    Serial.println();

    Serial.print(F("Time: ")); // Print the time
    uint8_t hms = ubxDataStruct->hour; // Print the hours
    if (hms < 10) {
      Serial.print(F("0")); // Print a leading zero if required
      myFile.print(F("0"));
    }
    Serial.print(hms);
    Serial.print(F(":"));
    myFile.print(hms);
    myFile.print(F(":"));
    hms = ubxDataStruct->min; // Print the minutes
    if (hms < 10) {
      Serial.print(F("0")); // Print a leading zero if required
      myFile.print(F("0"));
    }
    myFile.print(hms);
    myFile.print(F(":"));
    hms = ubxDataStruct->sec; // Print the seconds
    if (hms < 10) {
      Serial.print(F("0")); // Print a leading zero if required
      myFile.print(F("0"));
    }
    Serial.print(hms);
    Serial.print(F("."));
    myFile.print(hms);
    myFile.print(F("."));
    uint32_t millisecs = ubxDataStruct->iTOW % 1000; // Print the milliseconds
    if (millisecs < 100) {Serial.print(F("0"));myFile.print(F("0"));} // Print the trailing zeros correctly
    if (millisecs < 10) {Serial.print(F("0")); myFile.print(F("0"));}
    Serial.print(millisecs);
    myFile.print(millisecs);


    int32_t latitude = ubxDataStruct->lat; // Print the latitude
    Serial.print(F(" Lat: "));
    Serial.print(1.0*latitude/10000000, 7);
    myFile.print(F(" Lat: "));
    myFile.print(1.0*latitude/10000000, 7);

    int32_t longitude = ubxDataStruct->lon; // Print the longitude
    Serial.print(F(" Long: "));
    Serial.print(1.0*longitude/10000000, 7);
    myFile.print(F(" Long: "));
    myFile.print(1.0*longitude/10000000, 7);

    int32_t altitude = ubxDataStruct->hMSL; // Print the height above mean sea level
    Serial.print(F(" Height above MSL: "));
    Serial.print(altitude*0.001);
    Serial.println(F(" (m)"));
    myFile.print(F(" Height above MSL: "));
    myFile.print(altitude*0.001);
    myFile.println(F(" (m)"));
  // c write to file
  // fprintf(fptr, "Time: ");
  // //if (hms < 10) fprintf(fptr,"0"); // Print a leading zero if required
  // fprintf(fptr,"%02d:%02d:%02d.%04lu",ubxDataStruct->hour, ubxDataStruct->min, ubxDataStruct->sec, ubxDataStruct->iTOW % 1000);
  // fprintf(fptr, " Lat: %02.0000000f Long: %02.0000000f", 1.0*latitude/10000000, 1.0*longitude/10000000);
  // fprintf(fptr," Height above MSL: %f (m)", altitude*0.001);
  // fclose(fptr); 
}

void setup()
{
  Serial.begin(115200);
  delay(1000); 
  Serial.println("SparkFun u-blox Example");

  Wire.begin(); // Start I2C

  //myGNSS.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

  while (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Retrying..."));
    delay (1000);
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); // Set the I2C port to output UBX only (turn off NMEA noise)
  
  myGNSS.setNavigationFrequency(2); // Produce two solutions per second
  
  myGNSS.setAutoPVTcallbackPtr(&printPVTdata); // Enable automatic NAV PVT messages with callback to printPVTdata
  

}

void loop()
{
  myGNSS.checkUblox(); // Check for the arrival of new data and process it.
  myGNSS.checkCallbacks(); // Check if any callbacks are waiting to be processed.

  Serial.print(".");
  delay(50);
}