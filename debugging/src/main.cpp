#include <Arduino.h>

int BPin = 34, BtState, led = 2;

void setup() {

  Serial.begin(115200);
  pinMode(BPin, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
}

void loop() {
  
  BtState = digitalRead(BPin);
  Serial.println(BtState);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
}