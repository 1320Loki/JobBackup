#include <Arduino.h>

int analogtrial = A5;
int printed, cont1 = 0;
void setup() {

  Serial.begin(9600);
  pinMode(analogtrial, OUTPUT);
}

void loop() {
  
  printed = digitalRead(analogtrial);
  Serial.println(printed);
  //if (printed < 1000){  cont1++;  }
  //while ((cont1 > 0))  { Serial.println("buuuzzzz");  }
  delay(100);
}