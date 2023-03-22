//--------------------- Code essencials --------------------//
#include <Arduino.h>

//--------------------- Milis function ---------------------//
unsigned long start;                  // Estructura De millis
unsigned long current, current2;      //
const unsigned long period1 = 1000;   //
const unsigned long period2 = 500;    //
//--------------------- Milis function ---------------------//

int out[] = {2,3,4,5,6,7};
int a = 0;
//--------------------- Code essencials --------------------//

void setup() {

  Serial.begin(9600);
  Serial.println("");
  Serial.println("INICIO DE SISTEMA ");

  for (int i=0; i!=9; i++){

    delayMicroseconds(100);
    Serial.print("OUTPUT---> ");
    Serial.println(out[i]);
    pinMode(out[i], OUTPUT);
  }

}

void QuickButton(int i) {

  digitalWrite(out[i], HIGH);
  Serial.print("relay on  ---> ");
  Serial.println(out[i]);
  delay(100);

  digitalWrite(out[i], LOW);
  Serial.print("relay off ---> ");
  Serial.println(out[i]);
  delay(100);

}

void SlowButton(int i, int timer) {

  digitalWrite(out[i], HIGH);
  Serial.print("relay on  ---> ");
  Serial.println(out[i]);
  delay(timer);

  digitalWrite(out[i], LOW);
  Serial.print("relay off ---> ");
  Serial.println(out[i]);
  delayMicroseconds(10000);

}

void TeaRecipe(int tp, int sp, int tm, int st, int mn, int ps) {

  QuickButton(tp);
  //  Break IDLE

  QuickButton(tp);   // SELECT TEMP  
  delay(500);
  SlowButton(ps, 3500);    // PLUS
  delay(500);
  //  Set Temperature to MAX

  QuickButton(sp);   // SELECT TEMP  
  delay(500);
  SlowButton(mn, 2000);    // PLUS
  delay(500);
  QuickButton(ps);
  delay(500);
  //  Set Speed to MIN

  QuickButton(tm);   // SELECT TEMP  
  delay(500);
  SlowButton(ps, 2500);    // PLUS
  delay(500);
  //  Set Speed to MIN

  QuickButton(st);
  delayMicroseconds(10000);
  //  Start Cooking

}

void loop() {

  current = millis();

  if(current - start >= period1)  {
    a++;
    Serial.print("CONTADOR ");
    Serial.println(a);
  
    if( a == 5) { TeaRecipe(0,1,2,3,4,5);}

    start = millis();
  } }

/*

out[0] = 2  = tp  = Temperature
out[1] = 3  = sp  = Speed
out[2] = 4  = tm  = Time
out[3] = 5  = st  = Start
out[4] = 6  = mn  = Minus
out[5] = 7  = ps  = Plus

Speed Limits ----> 0 to 4
  Space 1
Time Limits ----> 0:00 to 99:00
  10 20 30 40 50 60
  30segd
Temp Limits ----> 0 to 120
  0 30 40 50 60 70 80 90 100 110 120

*/