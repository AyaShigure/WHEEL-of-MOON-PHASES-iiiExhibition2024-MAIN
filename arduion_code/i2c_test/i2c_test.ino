#include <Wire.h>
int led = 13;

void setup() {
  // 设置唯一的I2C地址 (根据你前面定义的地址方案)
  Wire.begin(0x08);  // 这里假设第一个Pro Mini的地址是0x08
  pinMode(led, OUTPUT);

  // 注册接收数据的事件
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);  // 用于调试
}

void blink(){
  digitalWrite(led, 1);
  delay(500);
  digitalWrite(led, 0);
  delay(500);
}
void loop() {
  // 其他逻辑
}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read();  // 读取I2C总线上的数据
    Serial.print(c);  // 打印接收到的数据
  }
  blink();
}
