#include <SPI.h>
#include <Ethernet.h>
//网络配置
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "192.168.2.151";  
IPAddress ip(192, 168, 2, 217);
EthernetClient client;
//切换等控制
int sensorPin1 = A0;    
int sensorPin2 = A1;    
int btnRight = 2;    
int btnLeft = 3;    
int sensorValue1 = 0;  
int sensorValue2 = 0; 
const int buttonPin = 7;   
int buttonState = 0;     
//快进快退控制
int CLK = 2;//CLK->D2
int DT = 3;//DT->D3
int SW = 4;//SW->D4
const int interrupt0 = 0;// Interrupt 0 在 pin 2 上
int count = 0;//计数值
int lastCLK = 0;//CLK历史值
int sta = 1;
//end

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  attachInterrupt(interrupt0, ClockChanged, CHANGE);//设置中断0的处理函数，电平变化触发

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
  controller();
}

void controller(){
   sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);
  buttonState = digitalRead(buttonPin);
  if(sensorValue1<50){
      postreq("/db/?cmd=addreplaceplay","path=USB/all");
      Serial.println("play all");
      count = 0;
      delay(400);
     
      return;
  }
  if(sensorValue1>1022){
      req("GET /command/?cmd=play&_=1448445158791 HTTP/1.1");
      Serial.println("prev");
      count = 0;
      delay(400);
      return;
  }
  if(sensorValue2>1022){
      req("GET /command/?cmd=
      next&_=1448445158791 HTTP/1.1");
      Serial.println("next");
      count = 0;
      delay(400);
      return;
  }
  if(sensorValue2<50){
      req("GET /command/?cmd=previous&_=1448445158791 HTTP/1.1");
      Serial.println("prev");
      count = 0;
      delay(400);
      return;
  }
  if (buttonState == LOW) {
      req("GET /command/?cmd=pause&_=1448445158791 HTTP/1.1");
      Serial.println("pause");
      count = 0;
      delay(400);
       return;

  } 
  if (!digitalRead(SW) && count != 0) //读取到按钮按下并且计数值不为0时把计数器清零
  {
    count = 0;
    Serial.print("count:");
    Serial.println(count);

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
    
}

void postreq(String uri,String data){
   if(client.connected()){
    Serial.println("disconnecting.");
    client.stop();
   }
   if (client.connect(server, 80)) {
        Serial.println("Sending to Server: ");                    
        client.println(String("POST "+uri+" HTTP/1.1"));           
        Serial.print(String("POST "+uri+" HTTP/1.1"));           
        client.println("Host: 192.168.2.151");
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.println("Connection: close");
        client.println("User-Agent: Arduino/1.0");
        client.print("Content-Length: ");
        client.println(data.length());
        client.println();
        client.print(data);
        client.println();        
      } else {
        // if you didn't get a connection to the server:
        Serial.println("connection failed");
      }
    
      while (client.available()) {
        client.read();
        
      }
}
//中断处理函数
void ClockChanged()
{
  int clkValue = digitalRead(CLK);//读取CLK引脚的电平
  int dtValue = digitalRead(DT);//读取DT引脚的电平
  unsigned long nowtime=millis(); //获取当前的系统运行时间长度

  if (lastCLK != clkValue)
  {
      lastCLK = clkValue;
      count += (clkValue != dtValue ? 1 : -1);//CLK和DT不一致时+1，否则-1
      if(count<=0){
        count=0;
      }
      if(count>=100){
        count=100;
      }
      
      if(sta){
        Serial.println(count);
        if(clkValue != dtValue){
          req("GET /seekfix.php?type=forward HTTP/1.1");
        }else{
          req("GET /seekfix.php?type=back HTTP/1.1");
        }

        sta=0;
      }else{
        if(clkValue != dtValue){
          count--;
        }else
        {
          count++;
        }
        sta=1;
      }
     
   
  }
}
