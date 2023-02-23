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
#include "WiFi.h"
//--------------------- essencials Libs --------------------//

//--------------------- Milis function ---------------------//
unsigned long start;                  // Estructura De millis
unsigned long current, current2;      //
const unsigned long period1 = 1500;   //
const unsigned long period2 = 500;    //
//--------------------- Milis function ---------------------//

//--------------------- Code essencials --------------------//
const char* ssid = "JosePC";            //  Name of WIFI Network
const char* password = "esp32wish";     //  Password

#define mqtt_server "192.168.1.200"     //  IP of MQTT BROKER
WiFiClient MqttWAC;                        //  Name of the MQTT CLIENT
PubSubClient client(MqttWAC);

//  Trialing and debbuging
float num;
//--------------------- Code essencials --------------------//

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
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("CONECTANDO");
    wific++;
    if (wific > 5){
      ESP.restart();
    }
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() 
{
  // Loop until we're reconnected
  int counter = 0;
  while (!client.connected()) 
  {
    if (counter==2) { ESP.restart();}
    counter+=1;
    Serial.print("Attempting MQTT connection...\n");
    // Attempt to connect
  
    if (client.connect("rtu2"))
    {
        Serial.println("Connected!!!");
        Serial.println("**************************************\n");
    } 
    else 
    {
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
  client.publish("Cart Status", output);             //  MQTT publishing
}

void setup() {

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
}


/*

  StaticJsonDocument<80> doc;                   //  JSON static DOC
  char output[80];

  current = millis();
  if(current - start >= period1) {

    num = random(0, 25);
    doc["n"] = num;

    serializeJson(doc, output);                 //  Json serialization
    Serial.println(output);                     

    client.publish("Trial", output);            //  MQTT publishing

    start = millis();
  }

*/

