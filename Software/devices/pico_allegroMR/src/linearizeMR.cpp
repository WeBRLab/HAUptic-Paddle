#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int main()
{
    // Init
    adc_init();
    adc_gpio_init(28);
    adc_select_input(2);
    
    const uint PIN = 5;
    gpio_init(PIN);
    gpio_set_dir(PIN, GPIO_OUT);

    // Constants
    uint16_t position = 0;
    stdio_init_all(); // NOTE:  Added by chandler 
    
    while (1)
    {
        uint16_t prevPosition = position;
        uint16_t position = adc_read();
        printf("%.2f \n", (float)position*3.3/4095.0);
        // sleep_ms(500);
    }
}


