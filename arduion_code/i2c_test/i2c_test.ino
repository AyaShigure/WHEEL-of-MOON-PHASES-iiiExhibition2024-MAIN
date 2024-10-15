#include <Wire.h>
#include <Stepper.h>

#define MOTOR_1   (6)   // IN1
#define MOTOR_2   (7)   // IN2
#define MOTOR_3   (8)   // IN3
#define MOTOR_4   (9)   // In4

#define MOTOR_STEPS (2048) // A full rebo

// ライブラリが想定している配線が異なるので2番、3番を入れ替える
Stepper myStepper(MOTOR_STEPS, MOTOR_1, MOTOR_3, MOTOR_2, MOTOR_4);

int led = 13;

void setup() {

  myStepper.setSpeed(10);
  // 设置唯一的I2C地址 (根据你前面定义的地址方案)
  Wire.begin(0x08); 
  pinMode(led, OUTPUT);

  // 注册接收数据的事件
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);  // 用于调试
}

void loop() {
  // Waiting to receive i2c commands
//  myStepper.step(1024);
//  delay(1000);
  }



void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read();  // 读取I2C总线上的数据
    Serial.print(c);  // 打印接收到的数据
  }

  myStepper.step(1024);
  delay(1000);
//  myStepper_pulse(256);
  blink(1);
}


void myStepper_demo_revolution(){
  // step one revolution  in one direction:
  myStepper.step(MOTOR_STEPS);
  delay(500);
  // step one revolution in the other direction:
  myStepper.step(-MOTOR_STEPS);
  delay(500);

  disable_motor();
}

void myStepper_pulse(int steps){
  myStepper.step(steps);
  delay(500);

  disable_motor();
}

void disable_motor() {
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_3, LOW);
  digitalWrite(MOTOR_4, LOW);
}


void blink(int n){
  for(int i = 0; i < n; i++) {
    digitalWrite(led, 1);
    delay(500);
    digitalWrite(led, 0);
    delay(500);
  }

}
