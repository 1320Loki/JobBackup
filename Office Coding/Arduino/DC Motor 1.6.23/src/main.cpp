#include <Arduino.h>

/* https://patorjk.com/software/taag/#p=display&f=Big&t=DC%20MOTOR%20-%201
  _____   _____   __  __  ____ _______ ____  _____             __ 
 |  __ \ / ____| |  \/  |/ __ \__   __/ __ \|  __ \           /_ |
 | |  | | |      | \  / | |  | | | | | |  | | |__) |  ______   | |
 | |  | | |      | |\/| | |  | | | | | |  | |  _  /  |______|  | |
 | |__| | |____  | |  | | |__| | | | | |__| | | \ \            | |
 |_____/ \_____| |_|  |_|\____/  |_|  \____/|_|  \_\           |_|
*/

//--------------------- Code essencials --------------------//

//  INDICATION LEDS
int l1=4, l2=2;

//  MOTOR 1 PINS
#define In1 6   
#define In2 7
#define ENA 8

//  MOTOR 2 PINS
#define ENB 10
#define In3 11  
#define In4 12

//  POSITIONING
#define SensPin 9
int Pos = 0;
int BtCont = 0, BtState = 0, LastBtState = 0;

// MILLIS FUNCTION
unsigned long start;                  
unsigned long current, current2;      
const unsigned long period1 = 1500;   //  Delay 1
const unsigned long period2 = 1500;   //  Delay 2

//--------------------- Code essencials --------------------//

void setup() {

  pinMode(l1, OUTPUT);  //  Green LED
  pinMode(l2, OUTPUT);  //  Red LED
  
  pinMode(In1, OUTPUT); //  Motor 1
  pinMode(In2, OUTPUT); //  Motor 1
  pinMode(In3, OUTPUT); //  Motor 2
  pinMode(In4, OUTPUT); //  Motor 2

  pinMode(ENA, OUTPUT); //  PWM Motor 1
  pinMode(ENB, OUTPUT); //  PWM Motor 2

  Serial.begin(9600);

}

//-----------------------------------------------------------------------------------//
//-------------------------------- CREATED FUNCTIONS --------------------------------//
//-----------------------------------------------------------------------------------//

void SpeedUpP() {
  //  Running LED
  digitalWrite(l1, HIGH);   
  digitalWrite(l2, LOW); 
  //  Speed T0
  digitalWrite(In1, HIGH);    //  MOTOR 1 Positive Direction
  digitalWrite(In2, LOW);       
  digitalWrite(In3, HIGH);    //  MOTOR 2 Positive Direction
  digitalWrite(In4, LOW);      
  //  Speed T0 - T1
  analogWrite(ENA, 10);
  analogWrite(ENB, 10);
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  delay(1000);
}

void SpeedUpN() {
  //  Running LED
  digitalWrite(l1, LOW);
  digitalWrite(l2, HIGH);
  //  Speed T0
  digitalWrite(In1, LOW);   //  MOTOR 1 Negative Direction
  digitalWrite(In2, HIGH);  
  digitalWrite(In3, LOW);   //  MOTOR 2 Negative Direction
  digitalWrite(In4, HIGH);     
  //  Speed T0 - T1
  analogWrite(ENA, 10);
  analogWrite(ENB, 10);
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  delay(1000);
}

void SlowDown() {
  //  Running LED
  digitalWrite(l1, LOW);   
  digitalWrite(l2, HIGH);
  //  Speed T0 - T1
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 10);
  analogWrite(ENB, 10);
  delay(1000);
}

void StopMotor(){
  digitalWrite(In1, LOW);   //  direction pin Motor 1
  digitalWrite(In2, LOW);   //  direction pin Motor 1
  digitalWrite(In3, LOW);   //  direction pin Motor 2
  digitalWrite(In4, LOW);   //  direction pin Motor 2 

  digitalWrite(l1, HIGH);
  digitalWrite(l2, HIGH);
  delay(1000);
}

void Distance(int P){
  // esta funcion se tiene que implementar en funcion de un boton el cual se accionara a medida que pase el cart
  // pero como en esta ocasion no se tiene este equipamiento.. se emplearan DELAYS para simular este funcionamiento
  if (P == 2){
    SpeedUpP();
    delay(3000);
    SlowDown();
    StopMotor();
    delay(3000);
    SpeedUpN();
    delay(3000);
    SlowDown();
    StopMotor();
  }
  else {
    SpeedUpP();
    delay(1000);
    SlowDown();
    StopMotor();
    delay(1000);
    SpeedUpN();
    delay(1000);
    SlowDown();
    StopMotor();
  }
    digitalWrite(l1, LOW);   digitalWrite(l2, LOW);  
    delay(5000);
}

//-----------------------------------------------------------------------------------//
//-------------------------------- CREATED FUNCTIONS --------------------------------//
//-----------------------------------------------------------------------------------//

void loop() {

  current = millis();
  if(current - start >= period1)
  {
    BtState = digitalRead(SensPin);   //  SensPin
    Serial.println("Fuera de while");

    if(BtState != LastBtState){

      BtCont++;
      if (BtCont > 0){

        Serial.print("Se recibio Mensaje. N cont = ");
        Serial.println(BtCont);
        digitalWrite(l2, HIGH);
        delayMicroseconds(20);
        digitalWrite(l2, LOW);

        while(BtCont == 1){
          SpeedUpP();
          BtState = digitalRead(SensPin);
          BtCont = BtCont + BtState;
          Serial.print("Se recibio Mensaje. N cont = ");
          Serial.println(BtCont);
          delayMicroseconds(100);
        }
        
        BtCont = 0;
        //while(Btcont == 2)
      }

    }

    LastBtState = BtState;  //  No Tocar
    start = millis();       //  No Tocar
  }
}

/*

Pos = random(1,2);
Distance(Pos);
digitalWrite(l1, HIGH);
digitalWrite(l2, HIGH);

*/