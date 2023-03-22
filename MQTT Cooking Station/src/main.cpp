//--------------------- essencials Libs --------------------//
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include <Arduino.h>
//--------------------- essencials Libs --------------------//

//--------------------- Ethernet / MQTT --------------------//
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192, 168, 0, 115);
IPAddress ip(192, 168, 0, 164);
   
EthernetClient CookStation1;
PubSubClient mqttClient(CookStation1);

String MqttInMsg;
//--------------------- Ethernet / MQTT --------------------//

//--------------------- Milis function ---------------------//
unsigned long start;                  // Estructura De millis
unsigned long current, current2;      // Estructura De millis
const unsigned long period1 = 500;    // Estructura De millis
const unsigned long period2 = 500;    // Estructura De millis
//--------------------- Milis function ---------------------//

//--------------------- Buttons outputs --------------------//
const int relay0=9, relay1=8, relay2=7, relay3=6;
const int relay4=5, relay5=4, relay6=3;
//--------------------- Buttons outputs --------------------//

//--------------------- Code essencials --------------------//
int counter1 = 0;       //  Setting Up Counters
int BuzzerPin = A5;     //  Buzzer Analog ReadPin
int BuzzerValue;        //  Buzzer Analog Value

int InSignal = A1;      //  Signal to Start Cooking Process
int RobotSignal = A2;   //  After the cooking process ends
int invalue, outvalue;  //  Values

int temperature, speed, time;
bool mqttAction = false;

//--------------------- Code essencials --------------------//

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////  Created Funtions  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////

void reconnect() {

  while (!mqttClient.connected()) {

    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("")) {

      Serial.println("connected");
      //  topics subscribed to...
      mqttClient.subscribe("Trial");  
      mqttClient.subscribe("Kitchen.1/Cooking.Station.1/Set");       
    }

    else  {

      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
}  }  }

///////////////////////////////////////////////////////////////////////////

void callback(char* topic, byte* message, unsigned int length)  {

  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  MqttInMsg = messageTemp;
  Serial.println();

  if (String(topic) == "Kitchen.1/Cooking.Station.1/Set") {

    Serial.println("Kitchen.1/Cooking.Station.1/Set");

    StaticJsonDocument<256> mensaje;
    DeserializationError error = deserializeJson(mensaje, MqttInMsg);

    if (error) {
      
      Serial.println("Error al deserializar el mensaje JSON: ");
      Serial.println(error.c_str());
      return;
   }

    temperature = mensaje["temperature"];
    speed = mensaje["speed"];
    time = mensaje["time"];

    Serial.print("Temperature: ");
    Serial.println(temperature);

    Serial.print("Speed: ");
    Serial.println(speed);

    Serial.print("time: ");
    Serial.println(time);

    mqttAction = true;


    mqttClient.publish("Kitchen.1/Cooking.Station.1/QsO1", "message received", true);

  }

}

///////////////////////////////////////////////////////////////////////////

void QuickButton(int i) {

  digitalWrite(i, HIGH);
  delay(100);
  digitalWrite(i, LOW);
  delay(100);

}

///////////////////////////////////////////////////////////////////////////

void StartButton() {

  delay(500);
  QuickButton(relay0);
  delay(100);
  QuickButton(relay3);
  delay(100);
}

///////////////////////////////////////////////////////////////////////////

void ResetButton() {

  delay(500);
  counter1 = 0;
  
  QuickButton(relay0);
  delay(100);
  digitalWrite(relay6, HIGH);
  delay(100);

  digitalWrite(relay6, HIGH);
  delay(2000);
  digitalWrite(relay6, LOW);
  delay(500);
}

///////////////////////////////////////////////////////////////////////////

void SpeedLvl(int speed) {
  
  Serial.println("Speed selected");
  QuickButton(relay1);
  delay(500);

  QuickButton(1);                       //  Set speed to 0
  digitalWrite(relay4 , HIGH);          //  Set speed to 0
  delay(1200);                          //  Set speed to 0
  digitalWrite(relay4 , LOW);           //  Set speed to 0
  delay(250);                           //  Set speed to 0

  switch (speed)  {                     //  Speed levels ---> 0 1 2 3 4
 
  case 0:
    Serial.println("Set speed to 0");
    break;
  
  case 1:
    Serial.println("Set speed to 1");
    for (int i=0; i<1; i++) { 
      QuickButton(5);
      delay(150);
      }
    break;

  case 2:
    Serial.println("Set speed to 1");
    for (int i=0; i<2; i++) { 
      QuickButton(5); 
      delay(150);
      }
    break;

  case 3:
    Serial.println("Set speed to 1");
    for (int i=0; i<3; i++) { 
      QuickButton(5); 
      delay(150);
      }
    break;

  case 4:
    Serial.println("Set speed to 1");
    for (int i=0; i<4; i++) { 
      QuickButton(5); 
      delay(150);
      }
    break;

} }

///////////////////////////////////////////////////////////////////////////

void TempLvl(int temperature)  {
  Serial.println("Temperature selected");
  QuickButton(relay0);
  delay(500);

  QuickButton(relay0);                  //  Set temp to 0
  digitalWrite(relay4, HIGH);           //  Set temp to 0
  delay(2500);                          //  Set temp to 0
  digitalWrite(relay4, LOW);            //  Set temp to 0
  delay(150);                           //  Set temp to 0

  switch (temperature) {

  case 0:
    Serial.println("Set temperature to 0");
    break;
  
  case 60:
    Serial.println("Set temperature to 60");
    for (int i=0; i<4; i++) { 
      QuickButton(relay5);
      delay(150);
      }
    break;

  case 100:
    Serial.println("Set temperature to 100");
    for (int i=0; i<8; i++) { 
      QuickButton(relay5);
      delay(150);
      }
    break;
    
  case 120:
    Serial.println("Set temperature to 120");
    for (int i=0; i<10; i++) { 
      QuickButton(relay5);
      delay(150);
      }
    break;

} }

///////////////////////////////////////////////////////////////////////////

void TimeLvl(int time)  {
  Serial.println("Time selected");
  QuickButton(relay2);
  delay(500);

  switch (time) {

  case 1:     //  1 minutes
    Serial.println("Set temp to 0");
    QuickButton(relay2);                       
    digitalWrite(relay5, HIGH);           
    delay(1700);                       
    digitalWrite(relay5, LOW);            
    delay(150); 
    break;

  case 3:     //  3 minutes
    Serial.println("Set temp to 0");
    QuickButton(relay2);                       
    digitalWrite(relay5, HIGH);           
    delay(2500);                       
    digitalWrite(relay5, LOW);            
    delay(150); 
    break;

  case 5:     //  5 minutes
    Serial.println("Set temp to 0");
    QuickButton(relay2);                       
    digitalWrite(relay5, HIGH);           
    delay(3200);                       
    digitalWrite(relay5, LOW);            
    delay(150); 
    break;
  
  case 10:    //  10 Minutes
    QuickButton(relay2);                       
    digitalWrite(relay5, HIGH);           
    delay(5200);                       
    digitalWrite(relay5, LOW);            
    delay(150);  
    break;   

  case 30:    //  30 Minutes
    QuickButton(relay2);                       
    digitalWrite(relay5, HIGH);           
    delay(13200);                       
    digitalWrite(relay5, LOW);            
    delay(150);  
    break;    

  case 60:    //  60 Minutes
    QuickButton(relay2);                       
    digitalWrite(relay4, HIGH);           
    delay(16200);                       
    digitalWrite(relay4, LOW);            
    delay(150);  
    break;  
       
  case 99:    //  99 Minutes
    QuickButton(relay2);   
    delay(150);                     
    QuickButton(relay4);    
    delay(150);  
    break;      
} }

///////////////////////////////////////////////////////////////////////////

void TriggerBuzzer () {

  delay(1000);

  for (;;)  {

    BuzzerValue = analogRead(BuzzerPin);
    Serial.println(counter1);
    Serial.println(BuzzerValue);

    if (BuzzerValue < 800)  { 
      counter1++;

      if (counter1 >= 3){

        Serial.println("STOPING BUZZ DETECTED... RESTETING Soon");
        delay(1000);
        ResetButton();
        delay(100);
        break;
      } }

  delay(100); 
} }

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////  Created Funtions  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void setup() {

  pinMode(relay0, OUTPUT);  //  Temperature
  pinMode(relay1, OUTPUT);  //  Speed
  pinMode(relay2, OUTPUT);  //  Time
  pinMode(relay3, OUTPUT);  //  Play
  pinMode(relay4, OUTPUT);  //  Minus
  pinMode(relay5, OUTPUT);  //  Plus
  pinMode(relay6, OUTPUT);  //  Reset

  Ethernet.begin(mac, ip);
  mqttClient.setServer(server, 1883);
  mqttClient.setCallback(callback);

  delay(3000);

  Serial.begin(9600);
  Serial.println("skibidi dob dob dob");

  delay(1000);
}

void loop() {

  mqttClient.loop();
  current = millis();
  
  if (!mqttClient.connected()) {  reconnect(); }

  
  if(current - start >= period1) {

    Serial.println("yes");

    Serial.print("time: ");
    Serial.println(time);
    
    if(mqttAction == true){

      Serial.print("TEMP: ");
      Serial.println(temperature);

      Serial.print("SPEED: ");
      Serial.println(speed);

      Serial.print("TIME: ");
      Serial.println(time);

      ResetButton();
      mqttAction = false;

     
    }



    start = millis();
  }
  mqttClient.loop();

}

/*
relay0 = 9  = tp  = Temperature
relay1 = 8  = sp  = Speed
relay2 = 7  = tm  = Time
relay3 = 6  = st  = Play
relay4 = 5  = mn  = Minus
relay5 = 4  = ps  = Plus
relay6 = 3  = rs  = Reset
*/

/*

    if(mqttAction == true){

      ResetButton();

      TimeLvl(time);              //  Values (0 1 3 5 10 30 60 99)
      SpeedLvl(speed);            //  Values (0 1 2 3 4)
      TempLvl(temperature);       //  Values (0 60 100 120)
          
      StartButton();              //  Start cooking process

      mqttAction = false;
      TriggerBuzzer();            //  Waiting for buzzer
    }




StaticJsonDocument<200> MqttQsO1;

      MqttQsO1["temperature"] = temperature;
      MqttQsO1["speed"] = speed;
      MqttQsO1["time"] = time;

      char payload[200];
      serializeJson(MqttQsO1, payload);
      mqttClient.publish("Kitchen.1/Cooking.Station.1/Set", payload, true);
*/