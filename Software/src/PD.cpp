#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <math.h>
#include <pico_rob14450.h>
#include "pin_definitions.h"

int main(){
    const std::vector<int> motor_pins = {STDBY_PIN,PWMA_PIN,A1_PIN,A2_PIN};
    const std::vector<int> sensor_pins = {SENSOR_PIN};
    ROB14450 motorDriver(motor_pins);
    motorDriver.init();
    
    while(true){
        motorDriver.drive(6250);
        sleep_ms(1000);
        motorDriver.stop();
        sleep_ms(500);
        motorDriver.drive(-6250);
        sleep_ms(1000);
        motorDriver.stop();
        sleep_ms(500);
    }
}