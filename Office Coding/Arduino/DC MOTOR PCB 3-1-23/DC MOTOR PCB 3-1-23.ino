/* https://patorjk.com/software/taag/#p=display&f=Big&t=DC%20MOTOR
  _____   _____   __  __  ____ _______ ____  _____  
 |  __ \ / ____| |  \/  |/ __ \__   __/ __ \|  __ \ 
 | |  | | |      | \  / | |  | | | | | |  | | |__) |
 | |  | | |      | |\/| | |  | | | | | |  | |  _  / 
 | |__| | |____  | |  | | |__| | | | | |__| | | \ \ 
 |_____/ \_____| |_|  |_|\____/  |_|  \____/|_|  \_\
*/

//--------------------- Code essencials --------------------//
int l1=4, l2=2;

#define In1 6   //  MOTOR 1 PINS
#define In2 7
#define ENA 8
#define In3 10  //  MOTOR 1 PINS
#define In4 11
#define ENB 12

int Pos = 0;
//--------------------- Code essencials --------------------//

//--------------------- Milis function ---------------------//
unsigned long start;                  // Estructura De millis
unsigned long current, current2;      //
const unsigned long period1 = 1500;   //
const unsigned long period2 = 500;    //
//--------------------- Milis function ---------------------//

void setup() {

  pinMode(l1, OUTPUT);  //  Green LED
  pinMode(l2, OUTPUT);  //  Red LED
  digitalWrite(l1, LOW);
  digitalWrite(l2, LOW);
  
  pinMode(In1, OUTPUT); //  Motor 1
  pinMode(In2, OUTPUT); //  Motor 1
  pinMode(In3, OUTPUT); //  Motor 2
  pinMode(In4, OUTPUT); //  Motor 2

  pinMode(ENA, OUTPUT); //  PWM Motor 1
  pinMode(ENB, OUTPUT); //  PWM Motor 2

  Serial.begin(9600);

}

void loop() {

  current = millis();
  if(current - start >= period1)
  {
    Pos = random(1,2);
    Distance(Pos);
    start = millis();
  }

}

void SpeedUpP() {
  //  Running LED
  digitalWrite(l1, HIGH);   
  digitalWrite(l2, LOW); 
  //  Speed T0
  digitalWrite(In1, HIGH);  //  direction pin Motor 1
  digitalWrite(In2, LOW);   //  direction pin Motor 1
  digitalWrite(In3, HIGH);  //  direction pin Motor 2
  digitalWrite(In4, LOW);   //  direction pin Motor 2   
  //  Speed T0 - T1
  analogWrite(ENA, 50);
  analogWrite(ENB, 50);
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  delay(1000);
}

void SpeedUpN() {
  //  Running LED
  digitalWrite(l1, HIGH);
  digitalWrite(l2, LOW);
  //  Speed T0
  digitalWrite(In1, LOW);   //  direction pin Motor 1
  digitalWrite(In2, HIGH);  //  direction pin Motor 1
  digitalWrite(In3, LOW);   //  direction pin Motor 2
  digitalWrite(In4, HIGH);  //  direction pin Motor 2   
  //  Speed T0 - T1
  analogWrite(ENA, 50);
  analogWrite(ENB, 50);
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  delay(1000);
}

void SlowDown() {
  //  Running LED
  digitalWrite(l1, LOW);   
  digitalWrite(l2, HIGH);
  //  Speed T0 - T1
  analogWrite(ENA, 150);
  analogWrite(ENB, 150); 
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  delay(1000);
  //  Speed T0 - T1
  analogWrite(ENA, 50);
  analogWrite(ENB, 50);
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



