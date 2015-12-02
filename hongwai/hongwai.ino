#include <IRremote.h>
#include <HashMap.h>

int RECV_PIN = 11; // 红外一体化接收头连接到Arduino 11号引脚
int led = 12 ;

IRrecv irrecv(RECV_PIN);

decode_results results; // 用于存储编码结果的对象

const byte HASH_SIZE = 17;
String line ;
//storage
HashType<long, char*> hashRawArray[HASH_SIZE];
HashMap<long, char*> btnhash = HashMap<long, char*>(hashRawArray , HASH_SIZE );

HashType<long, char*> numberBtnArray[HASH_SIZE];
HashMap<long, char*> numberBtn = HashMap<long, char*>(numberBtnArray , HASH_SIZE );
void setup()
{
  Serial.begin(9600); // 初始化串口通信
  irrecv.enableIRIn(); // 初始化红外解码

  pinMode(led, OUTPUT);

  //数字区域
  numberBtn[0](16738455, "1");
  numberBtn[1](16750695, "2");
  numberBtn[2](16756815, "3");
  numberBtn[3](16724175, "4");
  numberBtn[4](16718055, "5");
  numberBtn[5](16743045, "6");
  numberBtn[6](16716015, "7");
  numberBtn[7](16726215, "8");
  numberBtn[8](16734885, "9");
  numberBtn[9](16730805, "0");
  //全键编码
  btnhash[0](16738455, "1");
  btnhash[1](16750695, "2");
  btnhash[2](16756815, "3");
  btnhash[3](16724175, "4");
  btnhash[4](16718055, "5");
  btnhash[5](16743045, "6");
  btnhash[6](16716015, "7");
  btnhash[7](16726215, "8");
  btnhash[8](16734885, "9");
  btnhash[9](16728765, "*");
  btnhash[10](16730805, "0");
  btnhash[11](16732845, "#");
  btnhash[12](16736925, "w");
  btnhash[13](16720605, "a");
  btnhash[14](16754775, "s");
  btnhash[15](16761405, "d");
  btnhash[16](16712445, "o");
  btnhash.debug();
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value == 0 || results.value == 0xFFFFFFFF) {
      irrecv.resume(); // 接收下一个编码
      return;
    }
    if (numberBtn.isExist(results.value)) {
      char * type =  btnhash.getValueOf(results.value);
      line = String(line + String(type));
      flashLed();
    }
    if (btnhash.getValueOf(results.value) == "#") {
      Serial.println(line);
      line = "";
      flashLed();
    }
    irrecv.resume(); // 接收下一个编码
  }
}


void flashLed() {
  digitalWrite(led, HIGH);
  delay(300);
  digitalWrite(led, LOW);
}

