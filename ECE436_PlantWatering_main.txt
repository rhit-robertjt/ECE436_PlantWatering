/*
 Taken from repeat timer example (Examples -> ESP32 -> Timer -> RepeatTimer)
*/

#include "Pump_Relay.h"
#include "OLED_Debug.h"
// #include "DHT_Humid.h"
#include "PlantSensors.h"

// Stop button is attached to PIN 0 (IO0)
#define BTN_STOP_ALARM 0

int count = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Serial initiated");

  setupRelay(19, 20);
  // setBubblerOnTime(5*60);       // input in seconds, this is 5   min
  // setBubblerOffTime(2.5*60*60); // input in seconds, this is 2.5 hrs
  // setWatererOnTime(15*60);      // input in seconds, this is 15  min
  // setWatererOffTime(12*60*60);  // input in seconds, this is 12  hrs

  OLED_setup();
  DHT_setup(6);
  setupSensorPins(7,5);

  delay(2000);
}

void loop() {
  // If button is pressed
  if (digitalRead(BTN_STOP_ALARM) == LOW) {
    // endRelayTimer();
    delay(5); // debounce
    // setBubblerOnTime(getBubblerOnTime() + 3);
    // Serial.println(getBubblerOnTime());
    Serial.println("Temp: " + String(returnTemp()));
    Serial.println("Humid: " + String(returnHumid()));
    int soilValue = getSoilMoisture();
    int waterValue = getWaterLevel();
    Serial.print("Soil Moisture Value: ");
    Serial.println(soilValue);
    Serial.print("Water level Value: ");
    Serial.println(waterValue);
  }
  delay(1000);
  count = count + 1;
  showText(0, 0, String(getBubblerTimerTrigger())); // count
  readDHT();
}
