#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <math.h>
#include <pico_rob14450.h>
#include "pin_definitions.h"

int main(){
    // const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    // gpio_init(LED_PIN);
    // gpio_set_dir(LED_PIN, GPIO_OUT);
    const std::vector<int> motor_pins = {STDBY_PIN,PWMA_PIN,A1_PIN,A2_PIN};
    const std::vector<int> sensor_pins = {SENSOR_PIN};
    ROB14450 motorDriver(motor_pins);
    motorDriver.init();
    
    

    while(true){

        

        motorDriver.drive(6250);
        gpio_put(LED_PIN,true);
        sleep_ms(1000);
        motorDriver.stop();
        gpio_put(LED_PIN,false);
        sleep_ms(500);
        motorDriver.drive(-6250);
        gpio_put(LED_PIN,true);
        sleep_ms(1000);
        motorDriver.stop();
        gpio_put(LED_PIN,false);
        sleep_ms(500);
    }

}
