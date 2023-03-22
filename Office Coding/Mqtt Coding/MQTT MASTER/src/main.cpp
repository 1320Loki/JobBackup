/*
https://patorjk.com/software/taag/#p=display&f=Big&t=MQTT
  __  __  ____ _______ _______ 
 |  \/  |/ __ \__   __|__   __|
 | \  / | |  | | | |     | |   
 | |\/| | |  | | | |     | |   
 | |  | | |__| | | |     | |   
 |_|  |_|\___\_\ |_|     |_|   
                              
*/

//--------------------- essencials Libs --------------------//

#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <Servo.h>
#include "WiFi.h"

//--------------------- essencials Libs --------------------//

//--------------------- Milis function ---------------------//
unsigned long start;                  // Estructura De millis
unsigned long current, current2;      //
const unsigned long period1 = 1500;   //
const unsigned long period2 = 500;    //
//--------------------- Milis function ---------------------//

//--------------------- Code essencials --------------------//
 
//  WIFI CONNECTIONS
const char* ssid = "JosePC";
const char* password = "esp32wish";

#define TrialTopic "Trial"              //  Callback
#define mqtt_server "192.168.1.200"     //  IP of MQTT BROKER
WiFiClient Car1;                        //  MQTT CLIENT
PubSubClient client(Car1);              //  Must CHANGE 4 every device

//  Trialing and debbuging
float num;

//  INDICATION LEDS
int l2=2;

//  MOTOR 1 PINS
#define In1 0   
#define In2 4
#define ENA 16

//  MOTOR 2 PINS
#define ENB 5
#define In3 18  
#define In4 19

//  POSITIONING
int BPin = 34;                      //  Positioning Button
int Side = 0;                       //  Side of tilting. left = 1. Right = 2.
int Pos = 0;                        //  Cart Positions.  Values are 1-3 

bool Returning = false;             //  Returning state
int BtCont = 0, valid = 0;          //  Validation for the Buton algorithm
int BtState = 0, LastBtState = 0;   //  State pf the buttons

//  SERVO MOTORS
const int servoPin1 = 12, servoPin2 = 14;
Servo servo1, servo2;

//--------------------- Code essencials --------------------//


//////////////////////////////////////////////////////////////////////////////
/////////////////////////////  Created Funtions  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//  __________________________  WIFI Connects   __________________________  //
void conex()  {

  Serial.println();
  Serial.println("**************************************");
  Serial.print("Connecting to ");       // begin Wifi connect

  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  delay(2000);
  WiFi.begin(ssid, password);
  int wific = 0;
  
  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);
    Serial.println("CONECTANDO");
    wific++;

    if (wific > 5){ ESP.restart();  }
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {

  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");
    if (client.connect("")) {

      Serial.println("connected");
      client.subscribe("Trial");        //  topics subscribed to...
    }

    else  {

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
}  }  }

void callback(char* topic, byte* message, unsigned int length)  {

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {

    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  Serial.println();
  if (String(topic) == "Trial") {  Serial.println("Trial arrived"); }

}

void Alive()  {

  StaticJsonDocument<80> doc2;                 //  JSON static DOC
  char output[80];
  doc2["Status"] = "Alive";

  serializeJson(doc2, output);                 //  Json serialization
  Serial.println(output); 
  client.publish("Cart Status", output);       //  MQTT publishing
}
//  __________________________  WIFI Connects   __________________________  //

//  __________________________  Servo commands  __________________________  //
void servoLeft() {
  servo1.write(180);
  delay(1500);
}

void servoRight() {
  servo1.write(0);
  delay(1500);
}

void servoReset() {
  servo1.write(90);
  delay(3000);
}
//  __________________________  Servo commands  __________________________  //

//  __________________________  Motor commands  __________________________  //
void MovePositive() {

  digitalWrite(l2, HIGH);

  digitalWrite(In1, HIGH);  //  direction pin Motor 1
  digitalWrite(In2, LOW);   //  direction pin Motor 1
  digitalWrite(In3, HIGH);  //  direction pin Motor 2
  digitalWrite(In4, LOW);   //  direction pin Motor 2   

  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void MoveNegative() {

  digitalWrite(l2, HIGH); 

  digitalWrite(In1, LOW);   //  direction pin Motor 1
  digitalWrite(In2, HIGH);  //  direction pin Motor 1
  digitalWrite(In3, LOW);   //  direction pin Motor 2
  digitalWrite(In4, HIGH);  //  direction pin Motor 2   

  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void MotorStop() {

  digitalWrite(In1, LOW);   //  direction pin Motor 1
  digitalWrite(In2, LOW);   //  direction pin Motor 1
  digitalWrite(In3, LOW);   //  direction pin Motor 2
  digitalWrite(In4, LOW);   //  direction pin Motor 2 

  digitalWrite(l2, HIGH);
}

void MotorIdle() {

  digitalWrite(In1, LOW);   //  direction pin Motor 1
  digitalWrite(In2, LOW);   //  direction pin Motor 1
  digitalWrite(In3, LOW);   //  direction pin Motor 2
  digitalWrite(In4, LOW);   //  direction pin Motor 2 

  digitalWrite(l2, LOW);
}
//  __________________________  Motor commands  __________________________  //

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////  Created Funtions  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void setup()  {

  pinMode(l2, OUTPUT);    //  Red LED
  pinMode(BPin, INPUT);   //  Button

  pinMode(In1, OUTPUT);   //  Motor 1
  pinMode(In2, OUTPUT);   //  Motor 1
  pinMode(In3, OUTPUT);   //  Motor 2
  pinMode(In4, OUTPUT);   //  Motor 2
  pinMode(ENA, OUTPUT);   //  PWM Motor 1
  pinMode(ENB, OUTPUT);   //  PWM Motor 2

  servo1.attach(servoPin1);   //  Servo motor 1
  servo2.attach(servoPin2);   //  Servo motor 2

  Serial.begin(115200);
  conex();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {  reconnect();}     //  mqtt server conex
  current = millis();

  if(current - start >= period1) {
    
    Alive();
    start = millis();
  }
  client.loop();
}

/*

void reconnect() {

  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");
    if (client.connect("")) {

      Serial.println("connected");
      client.subscribe("Trial");
    }
    else  {

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
}  }  }




  StaticJsonDocument<80> doc2;                 //  JSON static DOC
  char output[80];
  doc2["Status"] = "Alive";
  doc2["Penelope"] = "Alive";

  serializeJson(doc2, output);                 //  Json serialization
  Serial.println(output); 
  client.publish("Cart Status", output);       //  MQTT publishing

/////////////////////////////////////////////////////////////////////////

  String Status = "Mamalo";
  int ano = 69;
  DynamicJsonDocument doc(2048);
  JsonArray arr = doc.createNestedArray("Array");
    arr.add(Status);
    arr.add(ano);

  char jsonStr[80];

  serializeJson(doc,jsonStr);
  Serial.print(jsonStr);
  Serial.println();

  client.publish("Nodo 2",jsonStr);

  
*/