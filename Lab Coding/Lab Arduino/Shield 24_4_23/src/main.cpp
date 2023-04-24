#include <SPI.h>
const int CS_PIN = 9; // Pin CS del MCP4231

//////////////////////////////////////////////////////////////////////////////////////////
void setResistance(int channel, int value) {
  digitalWrite(CS_PIN, LOW); // Seleccionar el MCP4231
  SPI.transfer(channel << 7); // Enviar el byte de comando de canal
  SPI.transfer(value); // Enviar el byte de valor de resistencia
  digitalWrite(CS_PIN, HIGH); // Deseleccionar el MCP4231
}
//////////////////////////////////////////////////////////////////////////////////////////

void setup() {

  pinMode(CS_PIN, OUTPUT); // Establecer el pin CS como salida
  SPI.begin(); // Iniciar la comunicación SPI
  delay(100); // Esperar 100ms para asegurarnos de que el MCP4231 esté listo

  Serial.begin(9600);
}

void loop() {

  Serial.println("2");
  setResistance(0, 55);
  setResistance(1, 55);
  delay(5000);

  Serial.println("1");
  setResistance(0, 200);
  setResistance(1, 200);
  delay(5000);
}

