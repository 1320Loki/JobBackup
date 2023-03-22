
/* https://patorjk.com/software/taag/#p=display&f=Big&t=Dual%20Core
  _____              _    _____               
 |  __ \            | |  / ____|              
 | |  | |_   _  __ _| | | |     ___  _ __ ___ 
 | |  | | | | |/ _` | | | |    / _ \| '__/ _ \
 | |__| | |_| | (_| | | | |___| (_) | | |  __/
 |_____/ \__,_|\__,_|_|  \_____\___/|_|  \___|

*/

//--------------------- essencials Libs --------------------//
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include "WiFi.h"
//--------------------- essencials Libs --------------------//

//--------------------- Code essencials --------------------//
#define WIFI_NETWORK "JosePC"           //    WIFI
#define WIFI_PASSWORD "esp32wish"
#define WIFI_TIME_MS 20000

#define mqtt_server "192.168.1.200"     //  IP of MQTT BROKER
WiFiClient rtu2;                        //  Name of the MQTT CLIENT
PubSubClient client(rtu2);

TaskHandle_t Task0;
TaskHandle_t Task1;

float num;                              //  Trialing and debbuging
String change;
//--------------------- Code essencials --------------------//


//////////////////////////////////////////////////////////////
//-------------------- Created Functions -------------------//
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
      delay(5000);
    } } }


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
//-------------------- Created Functions -------------------//
//////////////////////////////////////////////////////////////


//--------------------- TASK 1 >>> WIFI --------------------//
void WifiAlive (void * parameters) {

  for (;;) {

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Still Connected");
      vTaskDelay(10000 / portTICK_PERIOD_MS);
      continue;
    }

    Serial.println("**************************************");
    Serial.print("Connecting to ");       // begin Wifi connect
    Serial.println(WIFI_PASSWORD);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(2000);

    WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
    int wific = 0;
    
    while (WiFi.status() != WL_CONNECTED) {

      delay(1000);
      Serial.println("CONECTANDO");
      wific++;

      //if (wific > 5){ ESP.restart();  }

    }
    
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    reconnect();

  } }
//--------------------- TASK 1 >>> WIFI --------------------//

//--------------------- TASK 2 >>> MQTT --------------------//
void task2 (void * parameters)  {

  for (;;)  {
    
    
  if (!client.connected()) {  reconnect();}     //  mqtt server conex
  client.loop();
  delay(100);
    //vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}
//--------------------- Task 2 >>> MQTT --------------------//


void setup() {
  // 
  Serial.begin(115200);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //    task 1
  xTaskCreatePinnedToCore(
    WifiAlive,      //  Function Name
    "Wifi Alive",   //  Task Name
    5000,           //  Stack Size
    NULL,           //  Task Parameters
    1,              //  Task priority
    &Task0,         //  Task Handle
    0               //  Core 0 of esp32
  );            

  //    task 2
  xTaskCreatePinnedToCore(
    task2,          //  Function Name
    "Task 2",       //  Task Name
    5000,           //  Stack Size
    NULL,           //  Task Parameters
    1,              //  Task priority
    &Task1,         //  Task Handle
    1               //  Core 1 
  );               

}

void loop() {

}



/*
















*/
