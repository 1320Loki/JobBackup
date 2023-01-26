#include <Arduino.h>
#include "HX711.h"

//    https://github.com/bogde/HX711    GITHUB REPO OF THE CREATOR OF THE LIB

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 7;   //  DATA PIN OF THE HX711 DRIVER
const int LOADCELL_SCK_PIN = 8;    //  CLOCK PIN OF THE HX711 DRIVER
HX711 scale;                       //  SET NAME (IN THIS CASE ITS CALLED SCALE)

void setup(){}
void loop() {}

/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FIRST STEP:

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

1- UPLOAD THIS CODE TO DE ARDUINO/ESP32
2- THEN PLACE A KNOWN WEIGHT OBJECT ON TOP OF THE LOADCELL
3- SAVE/WRITE THE ANALOG VALUE OF THE PART 2
4- DIVIDE THE ANALOG VALUE OF (PART 2,3) AND THE KNOWN WEIGHT
5- SAVE/WRITE THAT VALUE

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SECOND STEP:

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

1- UPLOAD THIS CODE TO DE ARDUINO/ESP32
2- ON THE "scale.set_scale(XXXX.f);" REPLACE THE XXXX FOR THE VALUE SAVED ON
THE FIST STEP PART 5
*/