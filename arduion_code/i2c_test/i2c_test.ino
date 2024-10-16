#include <Wire.h>
#include <Stepper.h>

boolean control_motor_flag = false; // This flag will be changed in received handle function,

// Small motor
#define ENABLE_PIN 10  // 可选的使能引脚
#define STEP_PIN 11  // A4988的STEP引脚
#define DIR_PIN 12   // A4988的DIR引脚
int stepsPerRevolution = 200;  // 假设电机每转一圈需要200步

// Big motor

#define MOTOR_1   (6)   // IN1
#define MOTOR_2   (7)   // IN2
#define MOTOR_3   (8)   // IN3
#define MOTOR_4   (9)   // In4

#define MOTOR_STEPS (2048) // A full rebo

// ライブラリが想定している配線が異なるので2番、3番を入れ替える
Stepper myStepper(MOTOR_STEPS, MOTOR_1, MOTOR_3, MOTOR_2, MOTOR_4);
int led = 13;





void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read();  // 读取I2C总线上的数据
    Serial.print(c);  // 打印接收到的数据
  }

  control_motor_flag = true;
//  myStepper.step(1024);
//  delay(1000);
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







void setup() {
  // Small motor init
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  digitalWrite(ENABLE_PIN, LOW);  // 使能A4988（低电平有效）




  // Big motor init
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
//  if(control_motor_flag == true){
//    myStepper.step(1024);
//    control_motor_flag = false;
//    }
  digitalWrite(DIR_PIN, LOW);  // 改变方向为逆时针

  stepMotor(stepsPerRevolution);  // 顺时针转一圈
  delay(1000);  // 等待1秒

  digitalWrite(DIR_PIN, HIGH);  // 改变方向为逆时针
  stepMotor(stepsPerRevolution);  // 逆时针转一圈
  delay(1000);  // 等待1秒
}



void stepMotor(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);  // 发送脉冲
    delayMicroseconds(2000);  // 控制脉冲间隔，决定速度
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(2000);  // 控制脉冲间隔，决定速度
  }
}
