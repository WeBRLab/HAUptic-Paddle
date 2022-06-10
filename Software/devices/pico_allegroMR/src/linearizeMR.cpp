#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <math.h>


uint32_t linearize(uint32_t reading,uint32_t calibrationCenter,uint32_t calibrationFactor,uint32_t paddleRadius) {
        uint32_t result = abs((reading-calibrationCenter)*(3.30/4095.00)*calibrationFactor*paddleRadius);
        return result; 
}

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
        uint32_t sensorReading = adc_read();
        uint32_t calibrationFactor = 300;  //    [degree/Volt]
        uint32_t calibrationCenter = 3000.00;  //    []
        uint32_t paddleRadius = 1;  //    [m]
        uint32_t travelDistance = linearize(sensorReading,calibrationCenter,calibrationFactor,paddleRadius);
        printf("%.2f \n", (float)travelDistance);
    }
}