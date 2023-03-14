/*
https://patorjk.com/software/taag/#p=display&f=Big&t=MQTT%20COOKING
  __  __  ____ _______ _______    _____ ____   ____  _  _______ _   _  _____ 
 |  \/  |/ __ \__   __|__   __|  / ____/ __ \ / __ \| |/ /_   _| \ | |/ ____|
 | \  / | |  | | | |     | |    | |   | |  | | |  | | ' /  | | |  \| | |  __ 
 | |\/| | |  | | | |     | |    | |   | |  | | |  | |  <   | | | . ` | | |_ |
 | |  | | |__| | | |     | |    | |___| |__| | |__| | . \ _| |_| |\  | |__| |
 |_|  |_|\___\_\ |_|     |_|     \_____\____/ \____/|_|\_\_____|_| \_|\_____|                         
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#include <Arduino.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 254, 141, 164);
IPAddress server(192, 168, 1, 200);

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);


void reconnect() {

  while (!mqttClient.connected()) {

    Serial.print("Attempting MQTT connection...");

    if (mqttClient.connect("Spices")) {
      Serial.println("connected");
      mqttClient.subscribe("Trial");        //  topics subscribed to...
    }

    else  {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
}  }  }



void setup() {

  Serial.begin(9600);
  Serial.println("1");

  Ethernet.begin(mac, ip);
  mqttClient.setServer(server, 1883);

  Serial.println("2");
  delay(1500);
}

void loop() {
  Serial.println("yes");

  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.publish("test-topic", "Hello, world!");
  delay(1000);
}
