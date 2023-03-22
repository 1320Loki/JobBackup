/*
https://patorjk.com/software/taag/#p=display&f=Big&t=MEAL%20RUNNER
  __  __ ______          _      _____  _    _ _   _ _   _ ______ _____  
 |  \/  |  ____|   /\   | |    |  __ \| |  | | \ | | \ | |  ____|  __ \ 
 | \  / | |__     /  \  | |    | |__) | |  | |  \| |  \| | |__  | |__) |
 | |\/| |  __|   / /\ \ | |    |  _  /| |  | | . ` | . ` |  __| |  _  / 
 | |  | | |____ / ____ \| |____| | \ \| |__| | |\  | |\  | |____| | \ \ 
 |_|  |_|______/_/    \_\______|_|  \_\\____/|_| \_|_| \_|______|_|  \_\

*/

//--------------------- Code essencials --------------------//
#include <Arduino.h>
#include <Servo.h>
Servo myservo; 

int l1=4, l2=2, BPin = 9;

#define In1 6   //  MOTOR 1 PINS
#define In2 7   //  MOTOR 1 PINS
#define ENA 8   //  Speed Motor 1
#define ENB 10  //  Speed Motor 2
#define In3 11  //  MOTOR 2 PINS
#define In4 12  //  MOTOR 2 PINS

int Pos = 0, valid = 0, Side = 0;
bool Returning = false;

int BtCont = 0, BtState = 0, LastBtState = 0;   //  Button code
//--------------------- Code essencials --------------------//

//--------------------- Milis function ---------------------//
unsigned long start;                  // Estructura De millis
unsigned long current, current2;      //
const unsigned long period1 = 500;    //
const unsigned long period2 = 500;    //
//--------------------- Milis function ---------------------//

void setup() {

  pinMode(l1, OUTPUT);    //  Green LED
  pinMode(l2, OUTPUT);    //  Red LED
  pinMode(BPin, INPUT);   //  Input Button
  
  pinMode(In1, OUTPUT);   //  Motor 1
  pinMode(In2, OUTPUT);   //  Motor 1
  pinMode(In3, OUTPUT);   //  Motor 2
  pinMode(In4, OUTPUT);   //  Motor 2

  pinMode(ENA, OUTPUT);   //  PWM Motor 1
  pinMode(ENB, OUTPUT);   //  PWM Motor 2

  Serial.begin(9600);

  digitalWrite(l1, LOW);  //  Green LED OFF
  digitalWrite(l2, LOW);  //  Red LED OFF

}

//-------------------------------- CREATED FUNCTIONS --------------------------------//
void MovePositive() {
  //  Running LED
  digitalWrite(l1, HIGH);   //  Green ON, Red OFF, Positive Move
  digitalWrite(l2, LOW); 
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
  digitalWrite(l1, LOW);   //  Green OFF, Red ON, Positive Move
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

  digitalWrite(l1, HIGH);
  digitalWrite(l2, HIGH);
}

void MotorIdle() {
  digitalWrite(In1, LOW);   //  direction pin Motor 1
  digitalWrite(In2, LOW);   //  direction pin Motor 1
  digitalWrite(In3, LOW);   //  direction pin Motor 2
  digitalWrite(In4, LOW);   //  direction pin Motor 2 

  digitalWrite(l1, LOW);
  digitalWrite(l2, LOW);
}

void servoLeft() {
  myservo.write(180);
  delay(1500);
}

void servoRight() {
  myservo.write(0);
  delay(1500);
}

void servoReset() {
  myservo.write(90);
  delay(3000);
}

//------------------------------------ ARDU LOOP ------------------------------------//
void loop() {

  current = millis();
  if(current - start >= period1)
  {
    Serial.println("IDLE");
    digitalWrite(l1, LOW);
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

    digitalWrite(l1, HIGH);
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
 