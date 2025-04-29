// Following is code for ECE436 Collecting sensor values for Plant pot
// By Kaitlyn Kampwerth
/*
 * The code will grab the sensor values of water level and soil moisture level
 */

#include <Arduino.h> 
// constants won't change. Used here to set a pin number:
//int levelPin =  7;// the number of the water level pin
//int soilPin = 5;
int levelPin;
int soilPin; 

int getWaterLevel() {
  int waterValue = analogRead(levelPin);
  Serial.print("Water Level: ");
  Serial.println(waterValue);
  return waterValue;
}

int getSoilMoisture() {
  int soilValue = analogRead(soilPin);
  Serial.print("Soil Mositure Value: ");
  Serial.println(soilValue);
  return soilValue;
}

void setupSensorPins(int WaterlevelPin, int soilMoisPin){
  levelPin = WaterlevelPin;
  soilPin = soilMoisPin;
}

