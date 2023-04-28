/*
https://patorjk.com/software/taag/#p=display&f=Big&t=MQTT%20COOKING
  __  __  ____ _______ _______    _____ ____   ____  _  _______ _   _  _____ 
 |  \/  |/ __ \__   __|__   __|  / ____/ __ \ / __ \| |/ /_   _| \ | |/ ____|
 | \  / | |  | | | |     | |    | |   | |  | | |  | | ' /  | | |  \| | |  __ 
 | |\/| | |  | | | |     | |    | |   | |  | | |  | |  <   | | | . ` | | |_ |
 | |  | | |__| | | |     | |    | |___| |__| | |__| | . \ _| |_| |\  | |__| |
 |_|  |_|\___\_\ |_|     |_|     \_____\____/ \____/|_|\_\_____|_| \_|\_____|                         
*/

//--------------------- essencials Libs --------------------//
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include <Arduino.h>
//--------------------- essencials Libs --------------------//

//--------------------- Ethernet / MQTT --------------------//
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192, 168, 0, 50);
IPAddress ip(192, 168, 0, 25);                 //  change
   
EthernetClient CookStation1;                  //  change
PubSubClient mqttClient(CookStation1);        //  change

String MqttInMsg;
//--------------------- Ethernet / MQTT --------------------//

//--------------------- Milis function ---------------------//
unsigned long start;                  // Estructura De millis
unsigned long current, current2;      // Estructura De millis
const unsigned long period1 = 250;    // Estructura De millis
const unsigned long period2 = 500;    // Estructura De millis
//--------------------- Milis function ---------------------//

//--------------------- Buttons outputs --------------------//
const int relay0=9, relay1=8, relay2=7, relay3=6;
const int relay4=5, relay5=3, relay6=2;
//--------------------- Buttons outputs --------------------//

//--------------------- Code essencials --------------------//
int counter1 = 0;       //  Setting Up Counters
int BuzzerCont = 0;     //  Buzzer Counter
int BuzzerValue;        //  Buzzer Analog Value

int InSignal = A1;      //  Signal to Start Cooking Process
int RobotSignal = A2;   //  After the cooking process ends
int invalue, outvalue;  //  Values

int temperature, speed, time;
bool mqttAction = false;
bool cooking = false;

int cont = 0;
//--------------------- Code essencials --------------------//


/*
relay0 = 9  = tp  = Temperature
relay1 = 8  = sp  = Speed
relay2 = 7  = tm  = Time
relay3 = 6  = st  = Play
relay4 = 5  = mn  = Minus
relay5 = 3  = ps  = Plus
relay6 = 2  = rs  = Reset

A5 = Buzzer Pin

TimeLvl(time);              //  Values (0 1 3 5 10 30 60 99)
SpeedLvl(speed);            //  Values (0 1 2 3 4)
TempLvl(temperature);       //  Values (0 60 100 120)      
*/

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////  Created Funtions  /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

void QuickButton(int i) {

  digitalWrite(i, HIGH);
  delay(100);
  digitalWrite(i, LOW);
  delay(100);

}

//////////////////////////////////////////////////////////////////////////////

void StartButton() {

  delay(500);
  Serial.println("Stat button");
  delay(500);
  QuickButton(relay0);
  delay(100);
  QuickButton(relay3);
  delay(100);
  Serial.println("Stat button end");
  cooking = true;
}

//////////////////////////////////////////////////////////////////////////////

void ResetButton() {

  Serial.println("Statting of reset");
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
  Serial.println("Ending of reset");
}

//////////////////////////////////////////////////////////////////////////////

void cookSet(int speed, int temperature, int time) {

  Serial.print("Speed selected: ");
  Serial.println(speed);

  QuickButton(relay1);
  delay(150);

  switch (speed)  {     //  Speed levels ---> 0 1 2 3 4
 
    case 0:
      Serial.println("Set speed to 0");
      break;
    
    case 1:
      Serial.println("Set speed to 1");
      for (int i=0; i<1; i++) { 
        QuickButton(relay5);
        delay(150);
        }
      break;

    case 2:
      Serial.println("Set speed to 1");
      for (int i=0; i<2; i++) { 
        QuickButton(relay5); 
        delay(150);
        }
      break;

    case 3:
      Serial.println("Set speed to 1");
      for (int i=0; i<3; i++) { 
        QuickButton(relay5); 
        delay(150);
        }
      break;

    case 4:
      Serial.println("Set speed to 1");
      for (int i=0; i<4; i++) { 
        QuickButton(relay5); 
        delay(150);
        }
      break;
  }

  Serial.print("Temperature selected: ");
  Serial.println(temperature);

  QuickButton(relay0);
  delay(100);

  switch (temperature) {     //  Temperature levels ---> 0 60 100 120

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
  }

  Serial.print("Time selected: ");
  Serial.println(time);

  QuickButton(relay2);
  delay(150);

  switch (time) {

  case 1:     //  1 minutes
    Serial.println("Set temp to 1");
    QuickButton(relay2);                       
    digitalWrite(relay5, HIGH);           
    delay(1700);                       
    digitalWrite(relay5, LOW);            
    delay(150); 
    break;

  case 3:     //  3 minutes
    Serial.println("Set temp to 3");
    QuickButton(relay2);                       
    digitalWrite(relay5, HIGH);           
    delay(2500);                       
    digitalWrite(relay5, LOW);            
    delay(150); 
    break;

  case 5:     //  5 minutes
    Serial.println("Set temp to 5");
    QuickButton(relay2);                       
    digitalWrite(relay5, HIGH);           
    delay(3200);                       
    digitalWrite(relay5, LOW);            
    delay(150); 
    break;
  
  case 10:    //  10 Minutes
    Serial.println("Set temp to 10");
    QuickButton(relay2);                       
    digitalWrite(relay5, HIGH);           
    delay(5200);                       
    digitalWrite(relay5, LOW);            
    delay(150);  
    break;   

  case 30:    //  30 Minutes
    Serial.println("Set temp to 30");
    QuickButton(relay2);                       
    digitalWrite(relay5, HIGH);           
    delay(13200);                       
    digitalWrite(relay5, LOW);            
    delay(150);  
    break;    

  case 60:    //  60 Minutes
    Serial.println("Set temp to 60");
    QuickButton(relay2);                       
    digitalWrite(relay4, HIGH);           
    delay(16200);                       
    digitalWrite(relay4, LOW);            
    delay(150);  
    break;  
       
  case 99:    //  99 Minutes
    Serial.println("Set temp to 99");
    QuickButton(relay2);   
    delay(150);                     
    QuickButton(relay4);    
    delay(150);  
    break;  

}   }

//////////////////////////////////////////////////////////////////////////////

void reconnect() {

  while (!mqttClient.connected()) {

    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("")) {

      Serial.println("connected");
      //  topics subscribed to...
      mqttClient.subscribe("Trial");  
      mqttClient.subscribe("In/Cooking_1/Set");       
    }

    else  {

      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
}  }  }

//////////////////////////////////////////////////////////////////////////////

void callback(char* topic, byte* message, unsigned int length)  {

  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  Serial.print(". Message: ");

  if (String(topic).equalsIgnoreCase("In/Cooking_1/Set")) {    //  change
    Serial.println("In/Cooking_1/Set");                        //  change

    StaticJsonDocument<256> doc;
    deserializeJson(doc, message, length);  // Deserializa  JSON

    temperature = doc["Temperature"];
    speed = doc["Speed"];
    time = doc["Time"];

    Serial.print("Temperature: ");
    Serial.println(temperature);

    Serial.print("Speed: ");
    Serial.println(speed);

    Serial.print("time: ");
    Serial.println(time);

    mqttAction = true;

}   }

//////////////////////////////////////////////////////////////////////////////

void Online()  {

  StaticJsonDocument<80> msgout;                 //  JSON static DOC
  char output[80];
  msgout["Status"] = "Online";
  msgout["temperature"] = temperature;
  msgout["speed"] = speed;
  msgout["time"] = time;
  msgout["Cooking"] = cooking;

  //speed, temperature, time)

  String jsonStr;

  serializeJson(msgout, jsonStr);                 //  Json serialization
  Serial.println(output); 
  mqttClient.publish("Out/Cooking_1/Status", jsonStr.c_str());       //  MQTT publishing

}

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

  pinMode(A5, INPUT);
  delay(500);

  Ethernet.begin(mac, ip);
  mqttClient.setServer(server, 1883);
  mqttClient.setCallback(callback);

  delay(1500);

  Serial.begin(9600);

}

void loop() {

  current = millis();
  
  if (!mqttClient.connected()) {  reconnect(); }
  //cookSet(speed, temperature, time);

  if(current - start >= period1) {

    cont++;

    BuzzerValue = analogRead(A5);
    Serial.println(BuzzerValue);
    if(BuzzerValue < 700) { BuzzerCont++;}
    if(BuzzerCont >= 3) { cooking = false;}

    if(cont == 10){
      Online();
      cont = 0;
    }
    
    if(mqttAction == true){

      ResetButton();

      cookSet(speed, temperature, time);

      StartButton();
      delay(100);

      mqttAction = false;
      Online();
      BuzzerCont = 0;
    }

    start = millis();
  }
  mqttClient.loop();

}