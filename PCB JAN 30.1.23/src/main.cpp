/* https://patorjk.com/software/taag/#p=display&f=Big&t=DC%20MOTOR%20-%201
  _____   _____ ____    ______  _____ _____  
 |  __ \ / ____|  _ \  |  ____|/ ____|  __ \ 
 | |__) | |    | |_) | | |__  | (___ | |__) |
 |  ___/| |    |  _ <  |  __|  \___ \|  ___/ 
 | |    | |____| |_) | | |____ ____) | |     
 |_|     \_____|____/  |______|_____/|_|     
*/

//--------------------- essencials Libs --------------------//
#include <Arduino.h>
#include <Servo.h>
#include "WiFi.h"
//--------------------- essencials Libs --------------------//


//--------------------- Code essencials --------------------//
//  INDICATION LEDS
int l2=2;

//  MOTOR 1 PINS
#define In1 19   
#define In2 5
#define ENA 18

//  MOTOR 2 PINS
#define ENB 16
#define In3 17  
#define In4 4

//  POSITIONING
#define BPin 9
bool Returning = false;
int Pos = 0, valid = 0, Side = 0;
int BtCont = 0, BtState = 0, LastBtState = 0;

// MILLIS FUNCTION
unsigned long start;                  
unsigned long current, current2;      
const unsigned long period1 = 1500;   //  Delay 1
const unsigned long period2 = 1500;   //  Delay 2

//  SERVO MOTORS
const int servoPin1 = 12, servoPin2 = 14;
Servo servo1, servo2;

//  WIFI CONNECTIONS
const char* ssid = "JosePC";
const char* password = "esp32wish";

//--------------------- Code essencials --------------------//


//--------------------- CREATED FUNCTIONS ------------------//

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
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void MovePositive() {
  //  Running LED
  digitalWrite(l2, HIGH);   //  Green ON, Red OFF, Positive Move
  //  Speed T0
  digitalWrite(In1, HIGH);  //  direction pin Motor 1
  digitalWrite(In2, LOW);   //  direction pin Motor 1
  digitalWrite(In3, HIGH);  //  direction pin Motor 2
  digitalWrite(In4, LOW);   //  direction pin Motor 2   
  //  Speed T0 - T1
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  delay(100);
  //  Speed T0 - T1
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void MoveNegative() {
  //  Running LED
  digitalWrite(l2, HIGH); 
  //  Speed T0
  digitalWrite(In1, LOW);   //  direction pin Motor 1
  digitalWrite(In2, HIGH);  //  direction pin Motor 1
  digitalWrite(In3, LOW);   //  direction pin Motor 2
  digitalWrite(In4, HIGH);  //  direction pin Motor 2   
  //  Speed T0 - T1
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  delay(100);
  //  Speed T0 - T1
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void MotorStop() {
  digitalWrite(In1, LOW);   //  direction pin Motor 1
  digitalWrite(In2, LOW);   //  direction pin Motor 1
  digitalWrite(In3, LOW);   //  direction pin Motor 2
  digitalWrite(In4, LOW);   //  direction pin Motor 2 

  digitalWrite(l2, HIGH);
}

void MotorIdle() {
  digitalWrite(In1, LOW);   //  direction pin Motor 1
  digitalWrite(In2, LOW);   //  direction pin Motor 1
  digitalWrite(In3, LOW);   //  direction pin Motor 2
  digitalWrite(In4, LOW);   //  direction pin Motor 2 

  digitalWrite(l2, LOW);
}

void servoLeft() {
  servo1.write(180);
  delay(1500);
}

void servoRight() {
  servo1.write(0);
  delay(1500);
}

void servoReset() {
  servo1.write(90);
  delay(3000);
}
//-------------------------------- CREATED FUNCTIONS --------------------------------//
void setup() {

    Serial.begin(115200);

    pinMode(l2, OUTPUT);    //  Red LED

    pinMode(In1, OUTPUT);   //  Motor 1
    pinMode(In2, OUTPUT);   //  Motor 1
    pinMode(In3, OUTPUT);   //  Motor 2
    pinMode(In4, OUTPUT);   //  Motor 2

    pinMode(ENA, OUTPUT);   //  PWM Motor 1
    pinMode(ENB, OUTPUT);   //  PWM Motor 2

    servo1.attach(servoPin1);   //  Servo motor 1
    servo2.attach(servoPin2);   //  Servo motor 2

    conex();
}

void loop() {

  current = millis();
  if(current - start >= period1)
  {
    Serial.println("IDLE");
    digitalWrite(l2, LOW);
    MotorIdle();
    servoReset();

    BtState = digitalRead(BPin);
    if(BtState != LastBtState && BtState == 1){
      BtCont++;
      valid = BtCont;
    }
    LastBtState = BtState; 
    start = millis();
  }

  while(valid > 0){     // Movimiento
    
    delayMicroseconds(10000);
    Pos = 1;
    Side = 1;

    LastBtState = 1;      //  necesario para filtrar ruido
    // Pos = random(1, 3);
    // Side = ramdom(2, 3);
    Serial.print("Valor de la Pocision P = ");
    Serial.println(Pos);
    Serial.print("Valor Contador = ");
    Serial.println(BtCont);   

    digitalWrite(l2, HIGH);

    switch (Pos)
    {

      case 1: {

        Serial.print("Motor Running, Case 1, Pocision P = ");
        Serial.println(Pos);
        BtCont = 0;

        while(Pos != BtCont){

          if(Returning == false){ MovePositive();}
          else if(Returning == true){ MoveNegative();}
          
          BtState = digitalRead(BPin);
          if(BtState != LastBtState && BtState ==1){
            BtCont++;
            Serial.print("Counter Value = ");
            Serial.println(BtCont);
          }
          LastBtState = BtState;

          if(BtCont == Pos && Returning == false){    //  Motor se tiene que detener
            BtCont = 0;                               //  Para poder dropear alimentos
            Returning = true;
            MotorStop();
            delay(1000);
            if(Side == 1) { servoLeft(); }
            else if(Side == 2) { servoRight(); }

            Serial.println("Returning begin");
          }
          
          if(BtCont == Pos && Returning == true){
            valid = 0;
            BtCont = 0;
            Returning = false;
            delay(1000);
            servoReset();
            delay(1000);
            Serial.println("P inicial");
            break;
          }
          delay(500);
        }
        break;
      }

      case 2: {

        Serial.print("Motor Running, Case 2, Pocision P = ");
        Serial.println(Pos);
        BtCont = 0;

        while(Pos != BtCont){

          if(Returning == false){ MovePositive();}
          else if(Returning == true){ MoveNegative();}
          
          BtState = digitalRead(BPin);
          if(BtState != LastBtState && BtState ==1){
            BtCont++;
            Serial.print("Counter Value = ");
            Serial.println(BtCont);
          }
          LastBtState = BtState;

          if(BtCont == Pos && Returning == false){    //  Motor se tiene que detener
            BtCont = 0;                               //  Para poder dropear alimentos
            Returning = true;
            MotorStop();
            delay(1000);
            if(Side == 1) { servoLeft(); }
            else if(Side == 2) { servoRight(); }

            Serial.println("Returning begin");
          }
          
          if(BtCont == Pos && Returning == true){
            valid = 0;
            BtCont = 0;
            Returning = false;
            delay(1000);
            servoReset();
            delay(1000);
            Serial.println("P inicial");
            break;
          }
          delay(500);
        }
        break;
      }

      case 3: {

        Serial.print("Motor Running, Case 3, Pocision P = ");
        Serial.println(Pos);
        BtCont = 0;

        while(Pos != BtCont){

          if(Returning == false){ MovePositive();}
          else if(Returning == true){ MoveNegative();}
          
          BtState = digitalRead(BPin);
          if(BtState != LastBtState && BtState ==1){
            BtCont++;
            Serial.print("Counter Value = ");
            Serial.println(BtCont);
          }
          LastBtState = BtState;

          if(BtCont == Pos && Returning == false){    //  Motor se tiene que detener
            BtCont = 0;                               //  Para poder dropear alimentos
            Returning = true;
            MotorStop();
            delay(1000);
            if(Side == 1) { servoLeft(); }
            else if(Side == 2) { servoRight(); }

            Serial.println("Returning begin");
          }
          
          if(BtCont == Pos && Returning == true){
            valid = 0;
            BtCont = 0;
            Returning = false;
            delay(1000);
            servoReset();
            delay(1000);
            Serial.println("P inicial");
            break;
          }

          delay(500);
        }
        break;
      }
    }

    delay(500);
  }
}

/*



*/