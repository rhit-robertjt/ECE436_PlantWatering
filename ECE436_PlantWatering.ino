/*
 Taken from repeat timer example (Examples -> ESP32 -> Timer -> RepeatTimer)
*/

#include "Pump_Relay.h"

// Stop button is attached to PIN 0 (IO0)
#define BTN_STOP_ALARM 0

void setup() {
  Serial.begin(115200);
  Serial.println("Serial initiated");
  setupRelay(19, 20);

  delay(2000);
}

void loop() {
  // If button is pressed
  if (digitalRead(BTN_STOP_ALARM) == LOW) {
    // endRelayTimer();
    setBubblerOnTime(getBubblerOnTime() + 3);
    Serial.println(getBubblerOnTime());
  }
  delay(50);
}
