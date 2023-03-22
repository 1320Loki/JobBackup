
//--------------------- essencials Libs --------------------//

#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <Servo.h>
#include "WiFi.h"

//--------------------- essencials Libs --------------------//

const char* ssid = "JosePC";            //  Only needed if you are using a ESP32 / Wifi connection
const char* password = "esp32wish";     //  Only needed if you are using a ESP32 / Wifi connection

#define TrialTopic "Trial"              //  Callback
#define mqtt_server "192.168.1.200"     //  IP of MQTT BROKER

WiFiClient esp1;                        //  MQTT CLIENT or name of device. MUST change for each controler
PubSubClient client(esp1);              //  Must CHANGE 4 every device
String MqttInMsg;                       //  String needed to the MQTT Callback function (mqtt msg inputs)

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////  Created Funtions  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void conex()  {                         //  Function created for wifi connection for the ESP32

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

void reconnect() {                    //  Connection of the MQTT CLient

  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");
    if (client.connect("")) {

      Serial.println("connected");
      client.subscribe("Trial");      //  Subscribe to MQTT topics needed to MQTT CALLBACK
    }

    else  {

      Serial.print("failed, rc=");    //  Wait 5 seconds before retrying
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
}  }  }

void callback(char* topic, byte* message, unsigned int length)  {   //  Function created to read Topics and messages
  Serial.print("Message arrived on topic: ");                       //  You need to be subscribed to MQTT Topics in order to recive messages
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {    //  set the MQTT IN MSG one character at a time
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  MqttInMsg = messageTemp;              //  Sets the string created as a globar variable into the MQTT IN MSG
  Serial.println();

  if (String(topic) == "Trial") {       //  String() is used to convert the in to a string needed for comparations

    Serial.println("Trial arrived");

    if (MqttInMsg == "a" || MqttInMsg == "A" )  { Serial.println("MSG Recived"); }

}   }

void Online()  {                               //  FUNCTION created to send a mqtt msg letting know that the client is online

  StaticJsonDocument<80> doc2;                 //  JSON static DOC
  char output[80];
  doc2["Status"] = "Online";

  serializeJson(doc2, output);                 //  Json serialization
  Serial.println(output); 
  client.publish("Cart Status", output);       //  MQTT publishing
}

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////  Created Funtions  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////


void setup() {

  Serial.begin(115200);
  conex();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {  reconnect();}     //  Needed to start MQTT CONECNTION with server

  Online();
  client.loop();    // Needed to mqtt callback function DONT ERASE >:C
  
  delay(1000);
}