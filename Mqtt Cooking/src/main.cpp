//--------------------- essencials Libs --------------------//
#include <SPI.h>
#include <Arduino.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
//--------------------- essencials Libs --------------------//

//--------------------- Code essencials --------------------//
String MqttInMsg;
IPAddress ip(192, 168, 1, 220);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

const char* server = "192.168.1.200";

#define TrialTopic "Trial"              //  Callback
#define mqtt_server "192.168.1.200"     //  IP of MQTT BROKER

// Ethernet and MQTT related objects
EthernetClient ethClient;
PubSubClient client(ethClient);
//--------------------- Code essencials --------------------//

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////  Created Funtions  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

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

void callback(char* topic, byte* message, unsigned int length)    //Mqtt Callback
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
} }

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////  Created Funtions  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void setup() {

  Serial.begin(9600);
  Ethernet.begin(mac, ip); 
  delay(3000);   

  client.setServer(server, 1883); 
  client.setCallback(callback);
}

void loop() {
}