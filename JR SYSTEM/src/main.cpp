#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;

// linear actuator
const int relay1 = 4, relay2 = 7;
// Just the Button uwu
const int ob1 = 10;
int ob1Value;

HX711 scale;

// set the scale value to zero
int scaleval = 0;

// boolean to get in and out of the while
bool testing = false;

void setup() {
  //
  Serial.begin(9600);
  Serial.println("HX711 Demo");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  
  scale.set_scale(-1960.f);     // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

  // linear actuator
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  // Just the Button uwu
  pinMode(ob1, INPUT);

  //  All the way UP
  digitalWrite(relay2, HIGH);
  digitalWrite(relay1, HIGH);
  delay(2100);

}

void loop() {

  while(testing == false){
    digitalWrite(relay2, LOW);
    digitalWrite(relay1, LOW);
    scale.power_up();
    Serial.print("one reading:\t");
    Serial.println(scale.get_units(), 2);
    scaleval = scale.get_units(0);

    if(scaleval >= 700){ // if the scale value read is bigger than 700g the linear actuator stops and leaves the while cycle
      // to get out of the while cycle
      testing = true;
      // it stops the linear actuator
      digitalWrite(relay2, LOW);
      digitalWrite(relay1, HIGH);
      Serial.println("Out of the loop");
      //break;
    }
    delay(100);
    
  }
  if(testing == true){
    // the two relays in HIGH makes it move upwards
    digitalWrite(relay2, HIGH);
    digitalWrite(relay1, HIGH);

    // "button" ob1 forces boolean variable testing to false to restart the program mannually
    ob1Value = digitalRead(ob1);
    //Serial.println(ob1Value);
    if(ob1Value == 1) {
      testing = false;
    }
    delay(1000);
  }
}