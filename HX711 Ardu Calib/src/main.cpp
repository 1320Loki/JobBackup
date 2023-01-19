#include <Arduino.h>

#include "HX711.h"
//    https://github.com/bogde/HX711

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 7;
const int LOADCELL_SCK_PIN = 8;
HX711 scale;


void setup() {
  Serial.begin(9600);
  Serial.println("HX711 Demo");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  
  scale.set_scale(5905.f);     // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

}

void loop() {
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 2);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 1);

  scale.power_down();			        // put the ADC in sleep mode
  delay(5000);
  scale.power_up();
}





/*
void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale();
  scale.tare();
}

void loop() {

  //Serial.println(scale.get_units(10), 1);
  Serial.println(scale.get_units(), 1);
  delay(1000);
}
*/