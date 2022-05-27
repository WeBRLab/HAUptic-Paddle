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
    uint16_t loopTime = 1;
    uint16_t endTime = 0;
    uint16_t startTime = 0;
    uint16_t reference = 0;
    uint16_t kp = 1;
    uint16_t kd = 0;

    while (true)
    {
        uint16_t prevPosition = position;
        uint16_t position = adc_read();
            gpio_put(PIN, 0);
            uint16_t ctrl = kp * position + kd * (prevPosition + position) / loopTime;  // Control Equation
            gpio_put(PIN, 1);
        // }
    }
}