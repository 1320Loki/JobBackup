
#include <SPI.h>
#include <Wire.h>
#include "HX711.h"
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ArduinoJson.hpp>
#include <Adafruit_MotorShield.h>

// Set your MAC address and IP address here
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 220);

const char* server = "192.168.1.200";

// Ethernet and MQTT related objects
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);
String MqttInMsg;

int posicao_recebida;
int gramas_recebidas;
int posicao_largada;

//--------------------- Sistema --------------------//
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

const int LOADCELL_DOUT_PIN = 6;   //  DATA PIN OF THE HX711 DRIVER
const int LOADCELL_SCK_PIN = 7;    //  CLOCK PIN OF THE HX711 DRIVER
HX711 scale;                       //  SET NAME (IN THIS CASE ITS CALLED SCALE)

int enable =2;
int fim_curso = 3;
int leitura_fim_curso;
int posicao_atual;
int peso;

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////  Created Funtions  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void calibrar_balanca () {

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(11900.f);     // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0
}

void pesagem (int x) {
  peso = 0;
  Serial.println("X:");
  Serial.println(x);

  while (peso < x){

    //digitalWrite(pepper,LOW);
    Serial.print("Not enough. Actual value:\t");
    Serial.println(peso, 1);
    //Serial.print("\t| average:\t");
    //Serial.println(scale.get_units(10), 1);
    peso = scale.get_units();
    Serial.println("Peso:");
    Serial.print(peso);
    scale.power_down();			        // put the ADC in sleep mode
    delay(500);
    scale.power_up();
  }

  Serial.print("Done. You have:\t");
  Serial.println(peso, 1);
  scale.power_down();
}

//////////////////////////////////////////////////////////////////////////////
void atuador (int x) {
  int atuador1 = 4;
  int atuador2 = 5;
  pinMode(atuador1,OUTPUT);
  pinMode(atuador2,OUTPUT);
  if (x == 1) {
      digitalWrite(atuador1,LOW);
      digitalWrite(atuador2,LOW); 
  } 
  else if (x == 0) {
    digitalWrite(atuador1,HIGH); // em Baixo
    digitalWrite(atuador2,HIGH);
  } 
  else {
    Serial.println("Isso nao");
  }
} // 1 para subir, 0 para descer

//////////////////////////////////////////////////////////////////////////////
void posicaoInicial () {

  leitura_fim_curso=digitalRead(fim_curso);

  while (leitura_fim_curso == 1) {
    myMotor->step(1, BACKWARD, SINGLE);
        //Serial.println(acao);
    leitura_fim_curso = digitalRead(fim_curso);
    delay(15);
    //Serial.println(leitura_posicao);
  }
  posicao_atual=0;
}
//////////////////////////////////////////////////////////////////////////////
void posicao1 (int x) {

  if (x > posicao_atual) {
    int steps = (x-posicao_atual)*25;
    for (int step = 0; step < steps; step++) {
      myMotor->step(1, BACKWARD, SINGLE);
      delay(15);       
    }
    posicao_atual=x;
  }
  else if (x < posicao_atual) {
    int steps = (posicao_atual-x)*25;
    for (int step = 0; step < steps; step++) {
       myMotor->step(1, FORWARD, SINGLE);
       delay(15);
    }
    posicao_atual=x;
} }
//////////////////////////////////////////////////////////////////////////////
void Online()  {                   //  FUNCTION created to send a mqtt msg letting know that the client is online

  StaticJsonDocument<80> doc2;     //  JSON static DOC
  char output[80];
  doc2["Spices Position:"] = posicao_recebida;
  doc2["Position"] = posicao_atual;
  doc2["Grams"] = peso;

  serializeJson(doc2, output);                        //  Json serialization
  Serial.println("Publiquei:");
  Serial.println(output);  
  mqttClient.publish("Spices_Outputs", output);       //  MQTT publishing, ("Topic",payload)

}

//////////////////////////////////////////////////////////////////////////////
void SpiceMeasure(char* topic, byte* message, unsigned int length)    //Mqtt Callback
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  MqttInMsg = messageTemp;
  Serial.println();

  if (String(topic) == "Spices_Input") {       //  String() is used to convert the in to a string needed for comparations

    Serial.println("Trial arrived");
    Serial.println(MqttInMsg);

    char charAt0 = MqttInMsg.charAt(0);
    String subString = MqttInMsg.substring(1,3);
    char charAt3= MqttInMsg.charAt(3); 

    posicao_recebida= charAt0-'0';
    gramas_recebidas=subString.toInt();
    posicao_largada=charAt3-'0';

    Serial.println(posicao_recebida);
    Serial.println(gramas_recebidas);

    posicao1(posicao_recebida);    //posicao1 atuador pesagem
    delay(1000);

    atuador(1);
    delay(1500);
    
    Serial.println("Estou aqui");
    pesagem(gramas_recebidas);
    delay(1000);

    atuador(0);
    delay(2000);

    posicao1(posicao_largada);
    delay(1000);

} }

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////  Created Funtions  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////


void setup() {

  Serial.begin(9600);
  Ethernet.begin(mac, ip); 
  delay(3000);   

  mqttClient.setServer(server, 1883); 

  if (mqttClient.connect("Spices")) {

    Serial.println("Connection has been established, well done");
    mqttClient.setCallback(SpiceMeasure);
  } 

  else {  Serial.println("Looks like the server connection failed...");  } 

  pinMode(enable,INPUT);
  pinMode(fim_curso,INPUT_PULLUP);

  AFMS.begin();
  myMotor->setSpeed(50);

  calibrar_balanca();
  atuador(0);
  posicaoInicial();
  // Ensure that we are subscribed to the topic "MakerIOTopic",  If any device in the world publishes a message to “MakerIOTopic” then our program will call the function “subscribeReceive.”
  mqttClient.subscribe("Spices_Input");
  Serial.println("Setup ready.");
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
// This is needed at the top of the loop!
  mqttClient.loop();
  // Attempt to publish a value to the topic "MakerIOTopic"
  Online();
  delay(1000);
 


  mqttClient.loop();
  delay(2000);

}
