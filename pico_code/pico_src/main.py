from includes.LLC_functions import *
# from includes.Subsystems import *
# from includes.Stepper_pin_def import *
# from includes.Subsystem_motion_sequences import *

'''
    1. System basic functionalitis, tests
    2. Full feedforward control, start to end, with only the control discription
    3. 

    99. A Serial communication control mode
'''


#############################################
###### Stage 1, System initializations ######
'''
    About boot.py, having boot.py makes deleting files on rp2040 really slow somehow.
'''

time.sleep(2) # Prevent the rshell from grabbing the serial port
pin_init() # Initialize all pins to 0

# Enable motor relay
led = Pin(25, Pin.OUT)
led.on()

# time.sleep(20)
# 扫描I2C总线，看看是否能找到两个Pro Mini
devices = i2c.scan()
print("I2C设备地址:", devices)


# Make sure data is less than 32 bytes
# 示例：发送给地址为 0x08 的设备
for _ in range(6):
    send_motor_commands(0x08, 0, 512, 0, 2000)
    time.sleep(10)
    send_motor_commands(0x08, 1, 512, 1, 2000)
