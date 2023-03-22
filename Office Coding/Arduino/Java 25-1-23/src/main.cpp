#include <Arduino.h>

/*
 * Wish & Cook
 * Hugo Mostardinha @ 2023 -01  
 * Tanks: https://randomnerdtutorials.com
*/

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// Ethernet Shild Pin Used: D10 ~ D13 

// Relay state and pin
String relay1State = "Off";
const int relay = 7;

// Motor and PWM
String motor1State = "Off";
const int led = 9;         // the PWM pin the LED is attached to
int brightness = 0;  // how bright the LED is
int fadeAmount = 10;  // how many points to fade the LED by

// Client variables 
char linebuf[80];
int charcount=0;

void setup() { 
  // Relay module prepared 
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  // declare pin 9 to be an PWM output:
  pinMode(led, OUTPUT);
    // set the brightness of pin 9:
  analogWrite(led, brightness);

  
  // Open serial communication at a baud rate of 9600
  Serial.begin(9600);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

// Display dashboard page with on/off button for relay
// It also print Temperature in C and F
void dashboardPage(EthernetClient &client) {
  client.println("<!DOCTYPE HTML><html><head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>");                                                             
  client.println("<h3>Wish & Cook Web Controller - <a href=\"/\">Refresh</a></h3>");
  // Generates buttons to control the relay
  client.println("<h4>Cook 1 - State: " + relay1State + "</h4>");
  // If relay is off, it shows the button to turn the output on          
  if(relay1State == "Off"){
    client.println("<a href=\"/relay1on\"><button>ON</button></a>");
  }
  // If relay is on, it shows the button to turn the output off         
  else if(relay1State == "On"){
    client.println("<a href=\"/relay1off\"><button>OFF</button></a>");                                                                    
  }
  
// Generates buttons to control the motor
  client.println("<h4>Motor 1 - State: " + motor1State + "</h4>");
// If Motor is off, it shows the button to turn the output on          
  if(motor1State == "Off"){
    client.println("<a href=\"/motor1on\"><button>ON</button></a>");
  }
  // If relay is on, it shows the button to turn the output off         
  else if(motor1State == "On"){
    client.println("<a href=\"/motor1off\"><button>OFF</button></a>");                                                                    
  }


  client.println("</body></html>"); 
}


void loop() {



  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
       char c = client.read();
       //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        Serial.println(c);
        if (c == '\n' && currentLineIsBlank) {
          dashboardPage(client);
          break;
        }
        
        if (c == '\n') {
          // Find Cook botton answer
          if (strstr(linebuf,"GET /relay1off") > 0){
            digitalWrite(relay, HIGH);
            relay1State = "Off";
          }
          else if (strstr(linebuf,"GET /relay1on") > 0){
            digitalWrite(relay, LOW);
            relay1State = "On";
          }
          // Find Motor botton answer
      
          if (strstr(linebuf,"GET /motor1off") > 0){
            //int bringtness= 500;
            digitalWrite(led, HIGH);
            motor1State = "Off";
          }
          else if (strstr(linebuf,"GET /motor1on") > 0){
            //int bringtness= 0; brightness
            digitalWrite(led,LOW);
            motor1State = "On";
          }

          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;          
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}