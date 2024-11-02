from machine import Pin, PWM, ADC, I2C
import time

# This function set is for direct hardware control, the most basic functions
i2c = I2C(0, scl=Pin(21), sda=Pin(20), freq=100000)

def pin_init():
    for i in range(26):
        pin = Pin(i, Pin.OUT)
        pin.off() 

def blink(count):
    led = Pin(25, Pin.OUT)
    for i in range(count):
        led.value(1)
        time.sleep(0.5)
        led.value(0)
        time.sleep(0.5)
        
def boot():
    pin_init()
    
# 函数：发送四字节数据包
def send_motor_commands(address, motor1_dir, motor1_steps, motor2_dir, motor2_steps):
    data = bytearray([motor1_dir, motor1_steps, motor2_dir, motor2_steps])
    i2c.writeto(address, data)


def move_all_big_motors(dir, steps):
    '''
        dir -> 0 or 1
        steps -> [0,255] !!!!!!!!!
    '''
    for i in range(12):
        send_motor_commands(0x08 + i, dir, steps, 0, 0)


def move_all_small_motors(dir, steps):
    '''
        dir -> 0 or 1
        steps -> [0,255] !!!!!!!!!
    '''
    for i in range(12):
        send_motor_commands(0x08 + i, 0, 0, dir, steps)