#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <math.h>


int main()
{
    // Init
    adc_init();
    adc_gpio_init(1);
    adc_select_input(1);
    
    const uint PIN = 5;
    gpio_init(PIN);
    gpio_set_dir(PIN, GPIO_OUT);

    // Constants
    uint16_t position = 0;
    stdio_init_all(); // NOTE:  Added by chandler 
    
    while (1)
    {
        uint32_t sensorReading = adc_read();
        printf("%.2f \n", (float)sensorReading);
    }
}