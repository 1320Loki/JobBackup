//--------------------- Code essencials --------------------//
#include <Arduino.h>

//--------------------- Milis function ---------------------//
unsigned long start;                  // Estructura De millis
unsigned long current, current2;      //
const unsigned long period1 = 1000;   //
const unsigned long period2 = 500;    //
//--------------------- Milis function ---------------------//

int out[] = {2,3,4,5,6,7,8};
int a = 0;              //  contador que cambiare por mqtt input

int counter1 = 0;       //  Setting Up Counters
int BuzzerPin = A5;     //  Buzzer Analog ReadPin
int BuzzerValue;        //  Buzzer Analog Value

int InSignal = A1;      //  Signal to Start Cooking Process
int RobotSignal = A2;   //  After the cooking process ends
int invalue, outvalue;  //  Values
//--------------------- Code essencials --------------------//

void QuickButton(int i) {

  digitalWrite(out[i], HIGH);
  delay(100);
  digitalWrite(out[i], LOW);
  delay(100);

}

void StartButton() {

  delay(500);
  QuickButton(0);
  delay(100);
  QuickButton(3);
  delay(100);
}

void ResetButton() {

  delay(500);
  counter1 = 0;
  
  QuickButton(0);
  delay(100);
  digitalWrite(out[6], HIGH);
  delay(100);

  digitalWrite(out[6], HIGH);
  delay(2000);
  digitalWrite(out[6], LOW);
  delay(500);
}

///////////////////////////////////////////////////////////////////////////
void SpeedLvl(int speed) {
  
  Serial.println("Speed selected");
  QuickButton(1);
  delay(500);

  QuickButton(1);                       //  Set speed to 0
  digitalWrite(out[4], HIGH);           //  Set speed to 0
  delay(1200);                          //  Set speed to 0
  digitalWrite(out[4], LOW);            //  Set speed to 0
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
void TempLvl(int temp)  {
  Serial.println("Temperature selected");
  QuickButton(0);
  delay(500);

  QuickButton(0);                       //  Set temp to 0
  digitalWrite(out[4], HIGH);           //  Set temp to 0
  delay(2500);                          //  Set temp to 0
  digitalWrite(out[4], LOW);            //  Set temp to 0
  delay(150);                           //  Set temp to 0

  switch (temp) {

  case 0:
    Serial.println("Set temp to 0");
    break;
  
  case 60:
    Serial.println("Set temp to 60");
    for (int i=0; i<4; i++) { 
      QuickButton(5);
      delay(150);
      }
    break;

  case 100:
    Serial.println("Set temp to 100");
    for (int i=0; i<8; i++) { 
      QuickButton(5);
      delay(150);
      }
    break;
    
  case 120:
    Serial.println("Set temp to 120");
    for (int i=0; i<10; i++) { 
      QuickButton(5);
      delay(150);
      }
    break;

} }
///////////////////////////////////////////////////////////////////////////
void TimeLvl(int time)  {
  Serial.println("Time selected");
  QuickButton(2);
  delay(500);

  switch (time) {

  case 1:     //  1 minutes
    Serial.println("Set temp to 0");
    QuickButton(2);                       
    digitalWrite(out[5], HIGH);           
    delay(1700);                       
    digitalWrite(out[5], LOW);            
    delay(150); 
    break;

  case 3:     //  1 minutes
    Serial.println("Set temp to 0");
    QuickButton(2);                       
    digitalWrite(out[5], HIGH);           
    delay(2500);                       
    digitalWrite(out[5], LOW);            
    delay(150); 
    break;

  case 5:     //  5 minutes
    Serial.println("Set temp to 0");
    QuickButton(2);                       
    digitalWrite(out[5], HIGH);           
    delay(3200);                       
    digitalWrite(out[5], LOW);            
    delay(150); 
    break;
  
  case 10:    //  10 Minutes
    QuickButton(2);                       
    digitalWrite(out[5], HIGH);           
    delay(5200);                       
    digitalWrite(out[5], LOW);            
    delay(150);  
    break;   

  case 30:    //  30 Minutes
    QuickButton(2);                       
    digitalWrite(out[5], HIGH);           
    delay(13200);                       
    digitalWrite(out[5], LOW);            
    delay(150);  
    break;    

  case 60:    //  60 Minutes
    QuickButton(2);                       
    digitalWrite(out[4], HIGH);           
    delay(16200);                       
    digitalWrite(out[4], LOW);            
    delay(150);  
    break;  
       
  case 99:    //  99 Minutes
    QuickButton(2);   
    delay(150);                     
    QuickButton(4);    
    delay(150);  
    break;      
} }

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
///////////////////////////////////////////////////////////////////////////

void setup() {

  Serial.begin(9600);
  Serial.println("");
  Serial.println("INICIO DE SISTEMA ");

  pinMode(InSignal, INPUT_PULLUP);
  pinMode(RobotSignal, OUTPUT);

  for (int i=0; i!=7; i++){

    delayMicroseconds(100);
    Serial.print("OUTPUT---> ");
    Serial.println(out[i]);
    pinMode(out[i], OUTPUT);
} }


void loop() {

  current = millis();

  if(current - start >= period1)  {

    digitalWrite(RobotSignal, HIGH);
    invalue = digitalRead(InSignal);
    Serial.println(invalue);

    if  (digitalRead(InSignal) == 1) {

      digitalWrite(RobotSignal, LOW);
      
      ResetButton();      //  Resets all parameters
      
      TimeLvl(3);         //  Values (0 1 3 5 10 30 60 99)
             
      SpeedLvl(0);        //  Values (0 1 2 3 4)
             
      TempLvl(100);       //  Values (0 60 100 120)
          
      StartButton();      //  Start cooking process

      TriggerBuzzer();    //  Waiting for buzzer

    }

    start = millis();

} }


/*

out[0] = 2  = tp  = Temperature
out[1] = 3  = sp  = Speed
out[2] = 4  = tm  = Time
out[3] = 5  = st  = Play
out[4] = 6  = mn  = Minus
out[5] = 7  = ps  = Plus
out[6] = 8  = rs  = Reset

*/