#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "192.168.2.151";  
IPAddress ip(192, 168, 2, 217);
EthernetClient client;
int sensorPin1 = A0;    
int sensorPin2 = A1;    
int ledPin = 13;      
int sensorValue1 = 0;  
int sensorValue2 = 0; 
const int buttonPin = 2;   
int buttonState = 0;      


void setup() {
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  Serial.println("connecting...");
}

void loop() {
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);
  buttonState = digitalRead(buttonPin);
  if(sensorValue2>1000){
      req("GET /command/?cmd=next&_=1448445158791 HTTP/1.1");
      Serial.println("next");
      delay(400);
      return;
  }
  if(sensorValue2<100){
      req("GET /command/?cmd=previous&_=1448445158791 HTTP/1.1");
      Serial.println("prev");
      delay(400);
      return;

  }
  if (buttonState == LOW) {
      req("GET /command/?cmd=pause&_=1448445158791 HTTP/1.1");
      Serial.println("pause");
      delay(400);
       return;

  } 
}

void req(String uri){
   if(client.connected()){
    Serial.println("disconnecting.");
          client.stop();
   }
   if (client.connect(server, 80)) {
        Serial.println("connected");
        // Make a HTTP request:
        client.println(uri);
        client.println("Host: 192.168.2.151");
        client.println("Connection: close");
        client.println();
      } else {
        // if you didn't get a connection to the server:
        Serial.println("connection failed");
      }
    
      while (client.available()) {
        client.read();
        
      }
}

