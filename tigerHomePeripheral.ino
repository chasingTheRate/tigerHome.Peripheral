#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
 
// Replace with your network credentials
const char* ssid = "Southbound 35";
const char* password = "aggiesss";
 
ESP8266WebServer server(80);
Servo servoPortOne;
Servo servoPortTwo;
Servo servoPortThree;
 
String page = "";

int portOnePinTrans = D0;
int portOnePinSignal = D3;
int portTwoPinTrans = D1;
int portTwoPinSignal = D4;
int portThreePinTrans = D2;
int portThreePinSignal = D5;

void setup(void){

  servoPortOne.attach(portOnePinSignal);
  servoPortTwo.attach(portTwoPinSignal);
  servoPortThree.attach(portThreePinSignal);
  
  page = "<h1>You have reached a Tiger Blind Peripheral</h1>";

  pinMode(portOnePinTrans, OUTPUT);
  pinMode(portOnePinSignal, OUTPUT);
  pinMode(portTwoPinTrans, OUTPUT);
  pinMode(portTwoPinSignal, OUTPUT);
  pinMode(portThreePinTrans, OUTPUT);
  pinMode(portThreePinSignal, OUTPUT);
 
  digitalWrite(portOnePinTrans, LOW);
  digitalWrite(portTwoPinTrans, LOW);
  digitalWrite(portThreePinTrans, LOW);
  
  delay(500);
  
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  server.on("/", [](){
    server.send(200, "text/html", page);
  });

  server.on("/setPositionForServoAtPort", HTTP_POST, setPositionForServoAtPort);
  
  server.begin();
  
  Serial.println("Web server started!");
}
 
void loop(void){
  server.handleClient();
}

void setPositionForServoAtPort() {

  Serial.println("Setting Servo Positions...");
  
  delay(100);

  int pos = 0;
  int currentPosition = server.arg("currentPosition").toInt();
  int targetPosition = server.arg("targetPosition").toInt();
  int servoDelay = server.arg("delay").toInt();
  int port = server.arg("port").toInt();

//  Serial.println(currentPosition);
//  Serial.println(targetPosition);
//  Serial.println(servoDelay);
  
  server.send(200);

  Serial.println("Turn on Servos...");

  turnOnServo(port);
  
  if (currentPosition < targetPosition) {
    Serial.println("Less than");
    for(pos = currentPosition; pos <= targetPosition; pos += 1) {
      switch (port) {
        case 1:
          servoPortOne.write(pos);
          break;
        case 2:
          servoPortTwo.write(pos);
          break;
        case 3:
          servoPortThree.write(pos);
          break;
        default:
          break;
      }                                             
      delay(servoDelay);                       
    } 
  } else if (currentPosition > targetPosition) {
    for(pos = currentPosition; pos >= targetPosition; pos -= 1) {                                 
      switch (port) {
        case 1:
          servoPortOne.write(pos);
          break;
        case 2:
          servoPortTwo.write(pos);
          break;
        case 3:
          servoPortThree.write(pos);
          break;
        default:
          break;
      }              
      delay(servoDelay);                       
    }
  }

  Serial.println("Turn off Servos...");
  turnOffServos();   
}

void turnOnServo(int port) {
  switch (port) {
    case 1:
      digitalWrite(portOnePinTrans, HIGH);
      break;
    case 2:
      digitalWrite(portTwoPinTrans, HIGH);
      break;
    case 3:
      digitalWrite(portThreePinTrans, HIGH);
      break;
    default:
      break;
  }
  delay(100);
}

void turnOffServos() {
  digitalWrite(portOnePinTrans, LOW);
  digitalWrite(portTwoPinTrans, LOW);
  digitalWrite(portThreePinTrans, LOW);
  delay(100);
}
