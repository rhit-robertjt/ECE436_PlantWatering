#include <Wire.h>               
#include "HT_SSD1306Wire.h"

static SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst

// Turn Vext ON (powers OLED)
void VextON(void) {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
}

// Turn Vext OFF (default)
void VextOFF(void) {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH);
}

void showText(int x, int y, String input) {
  VextON();
  display.clear();
  display.drawString(0, 0, input);
  display.display();
  VextOFF();
}

void OELD_setup() {
  Serial.println();
  VextON();
  // Initialising the UI will init the display too.
  display.init();

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  VextOFF();
  
  delay(2000);
}