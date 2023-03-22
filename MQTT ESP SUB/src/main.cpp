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


//--------------------- Code essencials --------------------//

unsigned long start;                    //  Estructura De millis
unsigned long current, current2;      
const unsigned long period1 = 1000;   
const unsigned long period2 = 5000;    


//  WIFI CONNECTIONS
const char* ssid = "JosePC";
const char* password = "esp32wish";

#define TrialTopic "Trial"              //  Callback
#define mqtt_server "192.168.1.200"     //  IP of MQTT BROKER
WiFiClient rtu2;                        //  Name of the MQTT CLIENT
PubSubClient client(rtu2);              //  CHANGE 



float num;                              //  Trialing and debbuging
String change;

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
      client.subscribe("Trial");
    }
    else  {

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

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

void setup()  {

  Serial.begin(115200);

  conex();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {  reconnect();}     //  mqtt server conex
  client.loop();
  delay(1000);
}