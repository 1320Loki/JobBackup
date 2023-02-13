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
#include <PubSubClient.h>
#include <Arduino.h>
#include "WiFi.h"
//--------------------- essencials Libs --------------------//

//--------------------- Code essencials --------------------//
//  WIFI CONNECTIONS
const char* ssid = "JosePC";
const char* password = "esp32wish";

// MILLIS FUNCTION
unsigned long currentTime = millis();   //  Current time
unsigned long previousTime = 0;         //  Previous time
const long timeoutTime = 2000;          //  Define timeout (example: 2000ms = 2s)

#define mqtt_server "192.168.1.200"     //  IP of MQTT BROKER
WiFiClient rtu2;                        //  Name of the MQTT CLIENT
PubSubClient client(rtu2);
//--------------------- Code essencials --------------------//

void conex()
{
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  conex();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  // put your main code here, to run repeatedly:

}


/*

void reconnect() 
{
  // Loop until we're reconnected
  int counter = 0;
  while (!client.connected()) 
  {
    if (counter==2)
    {
        ESP.restart();
    }
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
    }
  }
  topicsSubscribe();
}












*/