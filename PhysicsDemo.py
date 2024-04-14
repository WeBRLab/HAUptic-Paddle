from machine import Pin, I2C
from pico_i2c_lcd import I2cLcd
import utime
import math

# Global variables
stdby_pin = machine.Pin(1, machine.Pin.OUT)
a1_pin = machine.Pin(2, machine.Pin.OUT)
a2_pin = machine.Pin(4, machine.Pin.OUT)
pwma_pin = machine.Pin(26)

# Functions to initialize sensor and driver
def init_sensor():
    adc_pin = machine.ADC(machine.Pin(27))
    return adc_pin

def init_driver():
    pwm = machine.PWM(pwma_pin)
    pwm.freq(1000)  # Set PWM frequency
    stdby_pin.value(1)  # Set standby pin to high
    return pwm

# Function to drive motor
def drive(speed, pwm):
    if speed <= 0:
        a1_pin.value(0)
        a2_pin.value(1)
    else:
        a1_pin.value(1)
        a2_pin.value(0)
    speed = abs(speed)
    pwm.duty_u16(round(speed))  # Corrected line

# Controller function
def control(kp, desired_position, position):
    maxspeed = 5000.0
    position_error = position - desired_position
    control_signal = kp * position_error
    if control_signal > maxspeed:
        control_signal = maxspeed
    if control_signal < -maxspeed:
        control_signal = -maxspeed
    return round(control_signal)

def init_encoder():
    pin_clk = Pin(6, Pin.IN, Pin.PULL_DOWN)
    pin_dt = Pin(7, Pin.IN, Pin.PULL_DOWN)
    return pin_clk, pin_dt

pin_clk, pin_dt = init_encoder()
clk_last = pin_clk.value()
counter = 0


def printThis(text):
    printSuccessful = 0
    I2C_ADDR = 0x27
    I2C_NUM_ROWS = 2
    I2C_NUM_COLS = 16
    while (printSuccessful == 0):
        i2c = I2C(1, sda=Pin(6), scl=Pin(7), freq=400000)
        try:
            lcd = I2cLcd(i2c, I2C_ADDR, I2C_NUM_ROWS, I2C_NUM_COLS)
            lcd.clear()
            lcd.putstr(text)
            printSuccessful = 1
            utime.sleep(.1)
        except:
            print("Error: please wait")
            
printThis("hAUptic Paddle: Physics Demo")


def getTheta(pos):
    # Convert position to angle (theta)
    angle = pos-484000 #* (2 * math.pi / 65535)  # Adjust scale and offset if needed
    angle = angle/20000*30+20
    return angle

wall_loc = 20

m = 10
g = 9.81  # Acceleration due to gravity (m/s^2)
L = 1.0   # Length of the pendulum (m)
dt = 0.1 # Time step for numerical integration

Kspring = [
    0.00,
    1.00,   
    0.00,
    1.00
]

Kdamp = [
    0.00,
    0.00,
    1.00,
    1.00
]


mode = 0
debounce = 0
iteration_count = 0
last_change_time = utime.ticks_ms()
adc = init_sensor()
pwm = init_driver()
pos = [0.0]
prev_pos = [0]
vel = [0]
itr = 0

kp = 7
kd = 20
ui_dt = 0
des_pos = 0
control_signal = 1;
while True:
    ui_dt += 0.01*itr
    ui_dt = ui_dt % .5
    clk_state = pin_clk.value()
    dt_state = pin_dt.value()
    if (ui_dt>= 0.1):
        if (clk_state != clk_last ):
            if dt_state != clk_state:
                mode -= 1
            else:
                mode += 1
            mode = mode % len(Kspring)
            printThis("Spring/Damper: \n"+ str(Kspring[mode]) + "/"+ str(Kdamp[mode]))
        clk_last = pin_clk.value()
        dt_last = dt_state
    prev_pos.append(pos[itr])
    utime.sleep(.01)
    p1 = 10*adc.read_u16()
    utime.sleep(.01)
    p2 = 10*adc.read_u16()
    utime.sleep(.01)
    p3 = 10*adc.read_u16()
    utime.sleep(.01)
    p = (getTheta(p1)+getTheta(p2)+getTheta(p3))/3
    if p>= 2:
        p += (p*p/10)
    pos.append(p)
    vel.append((getTheta(p3)-getTheta(p1))/.1)
    
    vel[itr] = sum(vel[-3:])/3
    pos_err = 0
    
    control_signal =  -pos[itr]*300
    if control_signal >= 0:
        control_signal = control_signal*5.5
    control_signal = control_signal/3.5*Kspring[mode]*1.25 - vel[itr]*Kdamp[mode]*250
    drive(control_signal, pwm)
    utime.sleep_ms(1)
    itr = itr +1
    print(control_signal)
    if itr>= 100:
        pos = [pos[itr]]
        vel = [vel[itr]]
        prev_pos = [prev_pos[itr]]
        itr = 0
        