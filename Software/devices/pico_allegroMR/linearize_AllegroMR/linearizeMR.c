#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int main()
{
    // Init
    adc_init();
    adc_gpio_init(26);
    const uint PIN = 5;
    gpio_init(PIN);
    gpio_set_dir(PIN, GPIO_OUT);

    // Constants
    uint16_t position = 0;

    while (true)
    {
        uint16_t prevPosition = position;
        uint16_t position = adc_read();
        printf("Blinking!\r\n");
            // gpio_put(PIN, 0);
            // gpio_put(PIN, 1);
        // }
    }
}