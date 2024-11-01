#include <Wire.h>
#include <Stepper.h>
// Flags n controls
bool update_traget_flag = false;
int motor1_direction = 1;  // 电机 1 的方向
int motor1_steps = 0;      // 电机 1 的步数
int motor2_direction = 1;  // 电机 2 的方向
int motor2_steps = 0;      // 电机 2 的步数


// Big motor

#define MOTOR_1   (6)   // IN1
#define MOTOR_2   (7)   // IN2
#define MOTOR_3   (8)   // IN3
#define MOTOR_4   (9)   // In4
#define MOTOR_STEPS (2048) // A full rebo 
// ライブラリが想定している配線が異なるので2番、3番を入れ替える
Stepper myStepper(MOTOR_STEPS, MOTOR_1, MOTOR_3, MOTOR_2, MOTOR_4);

void myStepper_revolution(bool dir, int steps) {
  // step one revolution  in one direction:
  if (dir == 0) {
    myStepper.step(steps);

  }else{
    myStepper.step(-steps);
  }
  disable_motor();
}

void disable_motor() {
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_3, LOW);
  digitalWrite(MOTOR_4, LOW);
}


// Small motor
#define ENABLE_PIN 10  // 可选的使能引脚
#define STEP_PIN 11  // A4988的STEP引脚
#define DIR_PIN 12   // A4988的DIR引脚
int stepsPerRevolution = 200;  // 假设电机每转一圈需要200步

void A4988_stepper_motor(boolean dir, int steps) {
  digitalWrite(ENABLE_PIN, LOW);  // Enable A4988

  digitalWrite(DIR_PIN, dir);
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);  // 发送脉冲
    delayMicroseconds(8000);  // 控制脉冲间隔，决定速度
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(8000);  // 控制脉冲间隔，决定速度
  }

  digitalWrite(ENABLE_PIN, HIGH);  // Disable A4988

}


// I2C stuff
int address;
static int led_pin = 13;
void blink_led() {
  digitalWrite(led_pin, 1);
  delay(100);
  digitalWrite(led_pin, 0);
  delay(100);
}


void receiveEvent(int howMany) {
  if (Wire.available() >= 4) {  // 确保接收到4个字节
    motor1_direction = Wire.read();   // 电机 1 的方向
    motor1_steps = Wire.read();       // 电机 1 的步数
    motor2_direction = Wire.read();   // 电机 2 的方向
    motor2_steps = Wire.read();       // 电机 2 的步数

    update_target_flag = true;  // 设置标志，指示有新指令
  }

}



// Main functions
void setup() {
  // Big motor init
  myStepper.setSpeed(10);

  // Small motor init
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);  // 使能A4988（低电平有效）
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(ENABLE_PIN, HIGH);

  // I2C init
  // 设置 D2-D5 为输入引脚，并启用内部上拉电阻
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  // 读取引脚状态，将其组合为地址，并加上8以避开系统保留地址
  address = ((digitalRead(5) << 3) | (digitalRead(4) << 2) | (digitalRead(3) << 1) | digitalRead(2));
  // 初始化 I2C 从设备
  Wire.begin(address + 8);
  pinMode(led_pin, OUTPUT);

  Serial.begin(9600);  // 用于调试
  Serial.print("I2C address: ");
  Serial.println(address);

  delay(1000);
  Wire.onReceive(receiveEvent);


}

void loop() {
  if (update_target_flag) {
    
    // Big motor rot
    myStepper_revolution(motor1_direction, motor1_steps);
    // Small motor rot
    A4988_stepper_motor(motor2_direction, motor2_steps);

    blink_led();
    update_target_flag = false;  // 重置标志
  }

}
