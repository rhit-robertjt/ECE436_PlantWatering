
#include <stdbool.h>
#include <Arduino.h>
#include "PlantSensors.h"
#include "Pump_Relay.h"

int bubblerRelayPin = 19;
int watererRelayPin = 20;
bool bubblerRelayState = false;
bool watererRelayState = false;
int bubblerTimerTrigger = 0;
int watererTimerTrigger = 0;
int bubblerRelayTimes[2] = { 5, 2 };
int watererRelayTimes[2] = { 6, 8 };
int minSoilMoisture = 20;

hw_timer_t *timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void ARDUINO_ISR_ATTR onTimer() {
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  bubblerTimerTrigger = bubblerTimerTrigger + 1;
  watererTimerTrigger = watererTimerTrigger + 1;
  // Serial.println(bubblerTimerTrigger);
  // Serial.println(watererTimerTrigger);
  portEXIT_CRITICAL_ISR(&timerMux);
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  // It is safe to use digitalRead/Write here if you want to toggle an output
  if (bubblerTimerTrigger >= bubblerRelayTimes[bubblerRelayState]) {
    digitalWrite(bubblerRelayPin, bubblerRelayState);
    bubblerRelayState = !bubblerRelayState;
    bubblerTimerTrigger = 0;
  }
  if (watererTimerTrigger >= watererRelayTimes[watererRelayState]) {
    if (getSoilMoisture() < minSoilMoisture){
      digitalWrite(watererRelayPin, false);
    }
    else{
      digitalWrite(watererRelayPin, watererRelayState);
    }
    watererRelayState = !watererRelayState;
    watererTimerTrigger = 0;
  }
}

void setBubblerOnTime(int time) {bubblerRelayTimes[0] = time;}
void setBubblerOffTime(int time) {bubblerRelayTimes[1] = time;}
int getBubblerOnTime() {return bubblerRelayTimes[0];}
int getBubblerOffTime() {return bubblerRelayTimes[1];}

void setWatererOnTime(int time) {watererRelayTimes[0] = time;}
void setWatererOffTime(int time) {watererRelayTimes[1] = time;}
int getWatererOnTime() {return watererRelayTimes[0];}
int getWatererOffTime() {return watererRelayTimes[1];}

int getBubblerRelayState() {return bubblerRelayState;}
int getWatererRelayState() {return watererRelayState;}

int getBubblerTimerTrigger() {return bubblerTimerTrigger;}
int getWatererTimerTrigger() {return watererTimerTrigger;}

void setMinSoilMoisture(int newSoilMoisture) {minSoilMoisture = newSoilMoisture;}

void endRelayTimer() {
  if (timer) {
    timerEnd(timer);
    timer = NULL;
  }
}

void setupRelay(int pin1, int pin2) {
  Serial.println("Init Relay Start");
  bubblerRelayPin = pin1;
  watererRelayPin = pin2;
  pinMode(bubblerRelayPin, OUTPUT);
  pinMode(watererRelayPin, OUTPUT);

  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();

  // Set timer frequency to 1Mhz
  timer = timerBegin(1000000);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer);

  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter) with unlimited count = 0 (fourth parameter).
  timerAlarm(timer, 1000000, true, 0);
  Serial.println("Init Relay End");
}