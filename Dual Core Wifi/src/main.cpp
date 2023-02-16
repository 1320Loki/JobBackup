
/*
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <PubSubClient.h>
*/
#include <Arduino.h>
#include "WiFi.h"

#define WIFI_NETWORK "JosePC"
#define WIFI_PASSWORD "esp32wish"
#define WIFI_TIME_MS 20000

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

      if (wific > 5){ ESP.restart();  }

    }
    
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void setup() {
  // 
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    WifiAlive,
    "Wifi Alive",
    5000,
    NULL,
    1,
    NULL,
    0            //CONFIG_ARDUINO_UDP_RUNNING_CORE
  );

}

void loop() {
  // 
}

/*

void conex()  {

}




    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Still Connected");
      vTaskDelay(10000 / portTICK_PERIOD_MS);
      continue;
    }

    Serial.println("Connecting!!");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

    unsigned long StartTime = millis();
  
    while (WiFi.status()!=WL_CONNECTED && millis()-StartTime< WIFI_TIME_MS) {

      if (WiFi.status() != WL_CONNECTED){
        Serial.println("WIFI fallo");
        vTaskDelay(2000 / portTICK_RATE_MS);
        continue;
      }
    }

    Serial.println("[WIFI] Connected: " + WiFi.localIP());

*/