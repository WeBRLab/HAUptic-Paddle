#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <math.h>
#include <pico_rob14450.h>
#include "pin_definitions.h"

// int main()
// {
//     // Init
//     adc_init();
//     adc_gpio_init(SENSOR_PIN);
//     adc_select_input(1);
    
//     gpio_init(PIN);
//     gpio_set_dir(PIN, GPIO_OUT);

//     // Constants
//     uint16_t position = 0;
//     stdio_init_all(); // NOTE:  Added by chandler 
    
//     while (1)
//     {
//         uint32_t sensorReading = adc_read();
//         printf("%.2f \n", (float)sensorReading);
//     }
// }

int main(){
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    const std::vector<int> motor_pins = {STDBY_PIN,PWMA_PIN,A1_PIN,A2_PIN};
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
