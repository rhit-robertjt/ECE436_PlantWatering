// Following is code for ECE436 Collecting sensor values for Plant pot
// By Kaitlyn Kampwerth
/*he aud
 * The code will grab the sensor values of water level and soil moisture level
 */

#include <Arduino.h> 
#include <math.h>
#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp

DHTesp dht;

double temperature;
double humidity;
int levelPin;
int soilPin; 

int getWaterLevel() {
  int waterValue = analogRead(levelPin);
  Serial.print("Water Level from Sensor: ");
  Serial.println(waterValue);
  if (waterValue < 1000){
    Serial.println("you are below 1000");
    return 0;
  }
  // Goes up to a max of 37% before going to next
  else if (waterValue < 1900){
    double mathValue = 0.04089*waterValue-39.8;
    Serial.println("you are above 1000");
    return mathValue;
  }
  else {
    double OtherMathValue = 0.834*waterValue - 1693;
    Serial.println("you are above 1900");
    if (OtherMathValue > 100){
      return 100;
    }
    else if (OtherMathValue < 37){
      return 37;
    }
    return OtherMathValue; 
  }
}

int getSoilMoisture() {
  int soilValue = analogRead(soilPin);
  Serial.print("Soil Mositure Value: ");
  Serial.println(soilValue);
  if (soilValue < 800){
    Serial.println("Soilvalue below 800");
    return 0;
  }
  else if (soilValue < 2400){
    Serial.println("Soilvalue below 2400");
    double mathValues = 0.043*soilValue - 32.0;
    return mathValues;
  }
  else{
    double otherMathValues = 0.143*soilValue - 278.6;
    Serial.println("you are above 2400");
    if (otherMathValues > 100){
      return 100;
    }
    else if (otherMathValues < 70){
      return 70;
    }
    return otherMathValues; 
  }
}

void setupSensorPins(int WaterlevelPin, int soilMoisPin){
  levelPin = WaterlevelPin;
  soilPin = soilMoisPin;
}


/**
 * DHT_setup
 * Setup DHT library
 * Setup task and timer for repeated measurement
 * @return bool
 *    true if task and timer are started
 *    false if task or timer couldn't be started
 */
bool DHT_setup(int pin) {
	dht.setup(pin, DHTesp::DHT11);
	Serial.println("DHT initiated");
  return true;
}

/**
 * readDHT
 * Reads temperature from DHT11 sensor
 * @return bool
 *    true if temperature could be aquired
 *    false if aquisition failed
*/
bool readDHT() {
	// Reading temperature for humidity takes about 250 milliseconds!
	// Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  TempAndHumidity newValues = dht.getTempAndHumidity();
	// Check if any reads failed and exit early (to try again).
	if (dht.getStatus() != 0) {
		// Serial.println("DHT11 error status: " + String(dht.getStatusString()));
		return false;
	}

  temperature = newValues.temperature;
  humidity = newValues.humidity;

  Serial.println("T:" + String(newValues.temperature) + " H:" + String(newValues.humidity));
	return true;
}

double returnTemp() {return temperature;}

double returnHumid() {return humidity;}
