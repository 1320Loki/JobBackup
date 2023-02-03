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

//--------------------- essencials Libs --------------------//
#include <Arduino.h>
#include "WiFi.h"
//--------------------- essencials Libs --------------------//


//--------------------- Code essencials --------------------//
//  WIFI CONNECTIONS
const char* ssid = "JosePC";
const char* password = "esp32wish";
WiFiServer server(80);
String header;



// MILLIS FUNCTION
unsigned long currentTime = millis();   // Current time
unsigned long previousTime = 0;         // Previous time
const long timeoutTime = 2000;          // Define timeout (example: 2000ms = 2s)
//--------------------- Code essencials --------------------//


void setup() {

  Serial.begin(115200);

  //  WIFI CONNECTION
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();   //  BEGIN THE WEB SERVER
//  WIFI CONNECTION
  
}

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
            if (header.indexOf("GET /d1") >= 0) {
              Serial.println("Distancia 1 selected");
            }

            else if (header.indexOf("GET /d2") >= 0) {
              Serial.println("Distancia 2 selected");
            }

            else if (header.indexOf("GET /d3") >= 0) {
              Serial.println("Distancia 3 selected");
            }
            
// --------------------------- Display the HTML web page --------------------------- //
            
            client.println("<!DOCTYPE html> <html>");
            client.println("<head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            //  CCS STYLE TO ON/OFF BUTTONS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #0c6306; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color : #321546;}</style></head>"); 
            
            // Web Page Heading
            client.println("<body><h1> ESP32 MEAL RUNER </h1>");
            
//  CREACION DE BOTON HTML DISTANCIA 1
            client.println("<p> Distancia de carrinho 1 </p>");
            client.println("<p><a href=\"/d1\"> <button class=\"button\">1</button></a> </p>");

//  CREACION DE BOTON HTML DISTANCIA 2
            client.println("<p> Distancia de carrinho 2 </p>");
            client.println("<p><a href=\"/d2\"> <button class=\"button\">2</button></a> </p>");

//  CREACION DE BOTON HTML DISTANCIA 3
            client.println("<p> Distancia de carrinho 3 </p>");
            client.println("<p><a href=\"/d3\"> <button class=\"button\">3</button></a> </p>");       
            
            client.println("</body> </html>");      //  END OF HTML BODY
            client.println();                       //  NEW LINE
            break;                                  //  END THE LOOP

// --------------------------- Display the HTML web page --------------------------- //
          
          } 
          
          else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
          
        } 

        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

*/