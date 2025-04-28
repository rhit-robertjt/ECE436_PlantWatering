/*
 Taken from repeat timer example (Examples -> ESP32 -> Timer -> RepeatTimer)
*/

#include "Pump_Relay.h"
#include "OLED_Debug.h"

// Stop button is attached to PIN 0 (IO0)
#define BTN_STOP_ALARM 0

void setup() {
  Serial.begin(115200);
  Serial.println("Serial initiated");

  setupRelay(21, 26);
  setBubblerOnTime(5*60);       // input in seconds, this is 5   min
  setBubblerOffTime(2.5*60*60); // input in seconds, this is 2.5 hrs
  setWatererOnTime(15*60);      // input in seconds, this is 15  min
  setWatererOffTime(12*60*60);  // input in seconds, this is 12  hrs

  OLED_setup();

  delay(2000);
}

void loop() {
  // If button is pressed
  if (digitalRead(BTN_STOP_ALARM) == LOW) {
    // endRelayTimer();
    delay(5); // debounce
    setBubblerOnTime(getBubblerOnTime() + 3);
    Serial.println(getBubblerOnTime());
  }
  delay(50);
}
