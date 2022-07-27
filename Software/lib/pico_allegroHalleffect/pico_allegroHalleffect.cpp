#include "pico_allegroHalleffect.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <algorithm>
#include <cmath>
#include <math.h>

// uint32_t linearize(uint32_t reading,uint32_t calibrationCenter,uint32_t calibrationFactor,uint32_t paddleRadius) {
//         uint32_t result = abs((reading-calibrationCenter)*(3.30/4095.00)*calibrationFactor*paddleRadius);
//         return result; 
// }


ALLEGROMR::ALLEGROMR()
{
    adc_init();
    adc_gpio_init(27);
    adc_select_input(1);   
    const uint PIN = 5;
    gpio_init(PIN);
    gpio_set_dir(PIN, GPIO_OUT);
    stdio_init_all(); // NOTE:  Added by chandler 
}

float ALLEGROMR::get_pos()
{
        float sensorReading = adc_read();
        return sensorReading;
}