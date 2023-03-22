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
#define In1 0   
#define In2 4
#define ENA 16

//  MOTOR 2 PINS
#define ENB 5
#define In3 18  
#define In4 19

//  POSITIONING

int BPin = 34;                      //  Positioning Button
int Side = 0;                       //  Side of tilting. left = 1. Right = 2.
int Pos = 0;                        //  Cart Positions.  Values are 1-3 
     
bool Returning = false;             //  Returning state
int BtCont = 0, valid = 0;          //  Validation for the Buton algorithm
int BtState = 0, LastBtState = 0;   //  State pf the buttons

// MILLIS FUNCTION
unsigned long start;                  
unsigned long current, current2;      
const unsigned long period1 = 1500;   //  Delay 1
const unsigned long period2 = 1500;   //  Delay 2

unsigned long currentTime = millis();   // Current time
unsigned long previousTime = 0;         // Previous time
const long timeoutTime = 2000;          // Define timeout (example: 2000ms = 2s)

//  SERVO MOTORS
const int servoPin1 = 12, servoPin2 = 14;
Servo servo1, servo2;

//  WIFI CONNECTIONS
String header;
WiFiServer server(80);
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

    digitalWrite(l2, HIGH);
    if (wific > 5){
      ESP.restart();
    }
  }
  
  digitalWrite(l2, LOW);
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

    pinMode(BPin, INPUT);   //  Input Button

    servo1.attach(servoPin1);   //  Servo motor 1
    servo2.attach(servoPin2);   //  Servo motor 2

    conex();
    server.begin();
}

//------------------------------------ ARDU LOOP ------------------------------------//
void loop() {

// NEW CLIENT CONNECTION
  WiFiClient client = server.available(); 

  if (client) {                     //  NEW CLIENT   
    currentTime = millis();         //  MILLIS TIME STRUCTURE
    previousTime = currentTime;     //  SET MILLIS TIME FOR DELAYING
    Serial.println("New Client.");  //  NEW CLIENT BY SERIAL
    String currentLine = "";        //  STRING TO HOLD INCOMING DATA

    // LOOP WHILE THE CLIENT IS CONNECTED
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();

      if (client.available()) {       //  BYTES FOR CLIENTS
        char c = client.read();       //  READ BYTE
        Serial.write(c);              //  SERIAL PRINT BYTE
        header += c;
        
        if (c == '\n') {              

          if (currentLine.length() == 0) {
           
            //  COMS WITH THE CLIENT
            client.println("HTTP/1.1 200 OK");          //  RSP CODE (HTTP/1.1 200 OK)
            client.println("Content-type:text/html");   //  TYPE OF MSG
            client.println("Connection: close");        //  CLOSE THE CONNECTION
            client.println();
            
            //  AVISO HTML
            if (header.indexOf("GET /1r") >= 0) {
              Serial.println("Distance 1, right side ---> selected");
              Side = 2;
              Pos = 1;
              valid++;
            }

            else if (header.indexOf("GET /2r") >= 0) {
              Serial.println("Distance 2, right side ---> selected");
              Side = 2;
              Pos = 2;
              valid++;
            }

            else if (header.indexOf("GET /3r") >= 0) {
              Serial.println("Distance 3, right side ---> selected");
              Side = 2;
              Pos = 3;
              valid++;
            }

            else if (header.indexOf("GET /1l") >= 0) {
              Serial.println("Distance 1, left side ---> selected");
              Side = 1;
              Pos = 1;
              valid++;
            }

            else if (header.indexOf("GET /2l") >= 0) {
              Serial.println("Distance 1, left side ---> selected");
              Side = 1;
              Pos = 2;
              valid++;
            }

            else if (header.indexOf("GET /3l") >= 0) {
              Serial.println("Distance 1, left side ---> selected");
              Side = 1;
              Pos = 3;
              valid++;
            }

// --------------------------- Display the HTML web page --------------------------- //
            client.println("<!DOCTYPE html> <html>");
            client.println("<head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            //  CCS STYLE TO ON/OFF BUTTONS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #0c6306; border: none; color: white; padding: 10px 20px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color : #321546;}</style></head>"); 
            
            // Web Page Heading
            client.println("<body><h1> ESP32 MEAL RUNER </h1>");
            
            //  CREACION DE BOTON HTML DISTANCIA 1 right 
            client.println("<p> Distance 1 right </p>");
            client.println("<p><a href=\"/1r\"> <button class=\"button\">1r</button></a> </p>");

            //  CREACION DE BOTON HTML DISTANCIA 2 right
            client.println("<p> Distance 2 right </p>");
            client.println("<p><a href=\"/2r\"> <button class=\"button\">2r</button></a> </p>");

            //  CREACION DE BOTON HTML DISTANCIA 3 right
            client.println("<p> Distance 3 right </p>");
            client.println("<p><a href=\"/3r\"> <button class=\"button\">3r</button></a> </p>");       

            //  CREACION DE BOTON HTML DISTANCIA 1 left
            client.println("<p> Distance 1 left </p>");
            client.println("<p><a href=\"/1l\"> <button class=\"button\">1l</button></a> </p>");

            //  CREACION DE BOTON HTML DISTANCIA 2 left
            client.println("<p> Distance 2 left </p>");
            client.println("<p><a href=\"/2l\"> <button class=\"button\">2l</button></a> </p>");

            //  CREACION DE BOTON HTML DISTANCIA 3 left
            client.println("<p> Distance 3 left </p>");
            client.println("<p><a href=\"/3l\"> <button class=\"button\">3l</button></a> </p>");       
            
            client.println("</body> </html>");      //  END OF HTML BODY
            client.println();                       //  NEW LINE
            break;                                  //  END THE LOOP
// --------------------------- Display the HTML web page --------------------------- //
          
          } 
          
          else { currentLine = "";}                 // CLEAR CURRENT LINE
          
        } 

        else if (c != '\r') { currentLine += c; }
      }
    }
    
    header = "";                                  //  CLEAR HEADER VARIABLE
    client.stop();                                //  CLOSE CONNECTION
    Serial.println("Client disconnected.");
    Serial.println("");
  
  }



    Serial.println("IDLE");
    MotorIdle();
    servoReset();
    BtState = digitalRead(BPin);
    Serial.print("Boton ");
    Serial.println(BtState);

    while(valid > 0){     // Movimiento
    
      delayMicroseconds(10000);
      Serial.print("Valor de la Pocision P = ");
      Serial.println(Pos);
      Serial.print("Valor del lado = ");
      Serial.println(Side);   

          switch (Pos) {
            
            // -----------------------------  Case 1 ----------------------------- //
            case 1: {

              Serial.print("Motor Running, Case 1, Pocision P = ");
              Serial.println(Pos);
              Serial.print("Side = ");
              Serial.println(Side);
              BtCont = 0;

              while(Pos != BtCont){

                if(Returning == false){ MovePositive();}
                else if(Returning == true){ MoveNegative();}
                
                BtState = digitalRead(BPin);
                Serial.print("Boton ");
                Serial.println(BtState);

                if(BtState != LastBtState && BtState == 1){
                  BtCont++;
                  Serial.print("Counter Value = ");
                  Serial.println(BtCont);
                }
                LastBtState = BtState;

                if(BtCont == Pos && Returning == false){    //  Motor se tiene que detener
                  BtCont = 0;                               //  Para poder dropear alimentos
                  Returning = true;
                  MotorStop();
                  delay(500);

                  if(Side == 1) { servoLeft(); }
                  else if(Side == 2) { servoRight(); }

                  delay(1000);
                  servoReset();

                  Serial.println("Returning begin");
                }
                
                if(BtCont == Pos && Returning == true){
                  valid = 0;
                  BtCont = 0;
                  Returning = false;

                  Serial.println("P inicial");
                  break;
                }
                delay(500);
                Serial.println("While final part");
              }
              break;
            }

            // -----------------------------  Case 2 ----------------------------- //
            case 2: {

              Serial.print("Motor Running, Case 1, Pocision P = ");
              Serial.println(Pos);
              Serial.print("Side = ");
              Serial.println(Side);
              BtCont = 0;

              while(Pos != BtCont){

                if(Returning == false){ MovePositive();}        //  Move Positive
                else if(Returning == true){ MoveNegative();}    //  Move Negative
                
                BtState = digitalRead(BPin);
                Serial.print("Boton ");
                Serial.println(BtState);

                if(BtState != LastBtState && BtState == 1){
                  BtCont++;
                  Serial.print("Counter Value = ");
                  Serial.println(BtCont);
                }
                LastBtState = BtState;

                if(BtCont == Pos && Returning == false){    //  Motor se tiene que detener
                  BtCont = 0;                               //  Para poder dropear alimentos
                  Returning = true;
                  MotorStop();
                  delay(500);

                  if(Side == 1) { servoLeft(); }
                  else if(Side == 2) { servoRight(); }

                  delay(1000);
                  servoReset();

                  Serial.println("Returning begin");
                }
                
                if(BtCont == Pos && Returning == true){
                  valid = 0;
                  BtCont = 0;
                  Returning = false;

                  Serial.println("P inicial");
                  break;
                }
                delay(500);
                Serial.println("While final part");
              }
              break;
            }

            // -----------------------------  Case 3 ----------------------------- //
            case 3: {

              Serial.print("Motor Running, Case 1, Pocision P = ");
              Serial.println(Pos);
              Serial.print("Side = ");
              Serial.println(Side);
              BtCont = 0;

              while(Pos != BtCont){

                if(Returning == false){ MovePositive();}
                else if(Returning == true){ MoveNegative();}
                
                BtState = digitalRead(BPin);
                Serial.print("Boton ");
                Serial.println(BtState);

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
                  delay(500);

                  if(Side == 1) { servoLeft(); }
                  else if(Side == 2) { servoRight(); }

                  delay(1000);
                  servoReset();

                  Serial.println("Returning begin");
                }
                
                if(BtCont == Pos && Returning == true){
                  valid = 0;
                  BtCont = 0;
                  Returning = false;

                  Serial.println("P inicial");
                  break;
                }
                delay(500);
                Serial.println("While final part");
              }
              break;
            }
            // -----------------------------  Case 3 ----------------------------- //
          }     // Switch Case

    }           //  While Valid

}               //  Valid