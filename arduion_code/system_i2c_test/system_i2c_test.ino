#include <Wire.h>
#include <Stepper.h>


// Big motor

#define MOTOR_1   (6)   // IN1
#define MOTOR_2   (7)   // IN2
#define MOTOR_3   (8)   // IN3
#define MOTOR_4   (9)   // In4

#define MOTOR_STEPS (2048) // A full rebo 

// ライブラリが想定している配線が異なるので2番、3番を入れ替える
Stepper myStepper(MOTOR_STEPS, MOTOR_1, MOTOR_3, MOTOR_2, MOTOR_4);

void myStepper_demo_revolution() {
  // step one revolution  in one direction:
  myStepper.step(MOTOR_STEPS);
  delay(500);
  // step one revolution in the other direction:
  myStepper.step(-MOTOR_STEPS);
  delay(500);

  disable_motor();
}


void disable_motor() {
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_3, LOW);
  digitalWrite(MOTOR_4, LOW);
}



int address;
static int led_pin = 13;

void setup() {
  // 设置 D2-D5 为输入引脚，并启用内部上拉电阻
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  // 读取引脚状态，将其组合为地址，并加上8以避开系统保留地址
  address = ((digitalRead(5) << 3) | (digitalRead(4) << 2) | (digitalRead(3) << 1) | digitalRead(2));

  // 初始化 I2C 从设备
  Wire.begin(address + 8);



  Serial.begin(9600);  // 用于调试
  Serial.print("I2C address: ");
  Serial.println(address);


  // Big motor init
  myStepper.setSpeed(10);
  // 设置唯一的I2C地址 (根据你前面定义的地址方案)


  pinMode(led_pin, OUTPUT);
//  delay(2000);
  address_blink();
}

void loop() {
  blink_led();
  myStepper.step(1024);
  delay(1000);

}

void address_blink() {
  for (int i = 0; i < address; i++) {

    digitalWrite(led_pin, 1);
    delay(500);
    digitalWrite(led_pin, 0);
    delay(500);

  }

  delay(1000);
}

void blink_led() {
  digitalWrite(led_pin, 1);
  delay(200);
  digitalWrite(led_pin, 0);
  delay(200);
}
