#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 200);

// Make sure to leave out the http and slashes!
const char* server = "192.168.1.200";

// Ethernet and MQTT related objects
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

String MqttInMsg;


void reconnect() {
  
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (mqttClient.connect("Spices")) {
      Serial.println("connected");
      mqttClient.subscribe("Spices_Input");        //  topics subscribed to...
    }
    else  {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      }
  }
}

void Online()  {    //  FUNCTION created to send a mqtt msg letting know that the client is online

  StaticJsonDocument<80> doc2;     //  JSON static DOC
  char output[80];
  doc2["Spices Position:"] = "a";

  serializeJson(doc2, output);                 //  Json serialization
  Serial.println("Publiquei:");
  Serial.println(output);  
  mqttClient.publish("Spices_Outputs", output);       //  MQTT publishing, ("Topic",payload)

}

void setup() {

  Serial.begin(9600);
  Ethernet.begin(mac, ip); 

  delay(3000);   

  mqttClient.setServer(server, 1883); 
  delay(3000);
}

void loop() {

  if (!mqttClient.connected()) {  reconnect();}

}