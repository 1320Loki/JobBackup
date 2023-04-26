
#include <Arduino.h>

#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

const int CS_PIN = 9; // Pin CS del MCP4231
bool stop =  false;

//////////////////////////////////////////////////////////////////////////////////////////
void setResistance(int channel, int value) {
  digitalWrite(CS_PIN, LOW);    // Seleccionar el MCP4231
  SPI.transfer(channel << 7);   // command byte
  SPI.transfer(value);          // Resistance byte
  digitalWrite(CS_PIN, HIGH);   // Deseleccionar el MCP4231
}
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
void Speed1() {

  int tim = 1000;

  Serial.println("110");
  setResistance(0, 110);
  setResistance(1, 110);
  delay(tim);

  Serial.println("100");
  setResistance(0, 100);
  setResistance(1, 100);
  delay(tim);

  Serial.println("90");
  setResistance(0, 90);
  setResistance(1, 90);
  delay(tim);

  while(stop == false){
    delay(5000);
    stop == true;

    Serial.println("90");
    setResistance(0, 90);
    setResistance(1, 90);
    delay(tim);

    Serial.println("100");
    setResistance(0, 100);
    setResistance(1, 100);
    delay(tim);

    Serial.println("110");
    setResistance(0, 110);
    setResistance(1, 110);
    delay(tim);

  }
}
//////////////////////////////////////////////////////////////////////////////////////////


void setup() {

  pinMode(CS_PIN, OUTPUT); // Establecer el pin CS como salida
  SPI.begin(); // Iniciar la comunicación SPI
  delay(100); // Esperar 100ms para asegurarnos de que el MCP4231 esté listo

  Serial.begin(9600);
}

void loop() {

  Speed1();
  delay(3000);

}

/*

__0   48k
__5   44k
_10   40k
_20   37k
_30   22k
_40   29k
_50   25k
_60   21k
_70   18k
_80   14k
_90   10k
100   6k
110   3.1k
120   1.2k
130   80
150   90

*/

/*

Serial.println("150");
  setResistance(0, 150);
  setResistance(1, 150);
  delay(tim);

  Serial.println("130");
  setResistance(0, 130);
  setResistance(1, 130);
  delay(tim);
  
  Serial.println("125");
  setResistance(0, 125);
  setResistance(1, 125);
  delay(tim);
  
  Serial.println("120");
  setResistance(0, 120);
  setResistance(1, 120);
  delay(tim);

  Serial.println("110");
  setResistance(0, 110);
  setResistance(1, 110);
  delay(tim);

  Serial.println("100");
  setResistance(0, 100);
  setResistance(1, 100);
  delay(tim);

  Serial.println("90");
  setResistance(0, 90);
  setResistance(1, 90);
  delay(tim);

  Serial.println("80");
  setResistance(0, 80);
  setResistance(1, 80);
  delay(tim);

  Serial.println("70");
  setResistance(0, 70);
  setResistance(1, 70);
  delay(tim);

  Serial.println("60");
  setResistance(0, 60);
  setResistance(1, 60);
  delay(tim);

  Serial.println("50");
  setResistance(0, 50);
  setResistance(1, 50);
  delay(tim);

  Serial.println("40");
  setResistance(0, 40);
  setResistance(1, 40);
  delay(tim);

  Serial.println("30");
  setResistance(0, 30);
  setResistance(1, 30);
  delay(tim);

  Serial.println("20");
  setResistance(0, 20);
  setResistance(1, 20);
  delay(tim);

  Serial.println("10");
  setResistance(0, 10);
  setResistance(1, 10);
  delay(tim);
  
  Serial.println("5");
  setResistance(0, 5);
  setResistance(1, 5);
  delay(tim);

  Serial.println("0");
  setResistance(0, 0);
  setResistance(1, 0);
  delay(tim);

*/