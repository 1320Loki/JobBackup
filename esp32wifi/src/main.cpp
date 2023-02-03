/* https://patorjk.com/software/taag/#p=display&f=Big&t=ESP%20WIFI
  ______  _____ _____   __          _______ ______ _____ 
 |  ____|/ ____|  __ \  \ \        / /_   _|  ____|_   _|
 | |__  | (___ | |__) |  \ \  /\  / /  | | | |__    | |  
 |  __|  \___ \|  ___/    \ \/  \/ /   | | |  __|   | |  
 | |____ ____) | |         \  /\  /   _| |_| |     _| |_ 
 |______|_____/|_|          \/  \/   |_____|_|    |_____|  


  WITH HELP BY:  https://randomnerdtutorials.com/esp32-web-server-arduino-ide/
*/


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

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

// MILLIS FUNCTION
unsigned long currentTime = millis();   // Current time
unsigned long previousTime = 0;         // Previous time
const long timeoutTime = 2000;          // Define timeout (example: 2000ms = 2s)
//--------------------- Code essencials --------------------//


void setup() {

  Serial.begin(115200);
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

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
            
            // GPIO CONTROL     MUST CHANGE THIS PART
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "1";
              digitalWrite(output26, HIGH);
              Serial.println(output26State);
            } 
            
            else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } 
            
            else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } 
            
            else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            
// --------------------------- Display the HTML web page --------------------------- //
            
            client.println("<!DOCTYPE html> <html>");
            client.println("<head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            //  CCS STYLE TO ON/OFF BUTTONS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #321546;}</style></head>"); 
            
            // Web Page Heading
            client.println("<body><h1> ESP32 MEAL RUNER </h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p> Distancia de carrinho " + output26State + "</p>");           //    NOMBRE DEL BOTON 1
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"> <button class=\"button\">ON</button></a> </p>");
            } 
            
            else {
              client.println("<p><a href=\"/26/off\"> <button class=\"button button2\">1</button></a> </p>");
            } 
                
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"> <button class=\"button\">ON</button></a> </p>");
            } 
            
            else {
              client.println("<p><a href=\"/27/off\"> <button class=\"button button2\">OFF</button></a> </p>");
            }
            client.println("</body> </html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;

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
