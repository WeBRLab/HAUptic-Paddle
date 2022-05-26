#include <stdio.h>
#include <string.h>
#include <string>
#include <iomanip>
#include <sstream>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "pico/binary_info.h"

#include "include/pico_lcd_i2c.hpp"


int main() {
    PicoLCDI2C lcd;
    // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)

    // Initializing i2c and associated gpios
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // Initializing adc and associated gpios
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);

    const float conversion_factor = 3.3f / (1 << 12);

    // Initialize LCD
    lcd.lcd_init();

        while (1) {
            uint16_t result = adc_read();
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << result*conversion_factor;
            std::string message = stream.str();
            lcd.print(message,1);
            sleep_ms(60);
            lcd.lcd_clear();
        }

    return 0;
}
