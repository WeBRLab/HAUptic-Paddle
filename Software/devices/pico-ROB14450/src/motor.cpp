#include "include/ROB14450.h"
#include "pico/stdlib.h"

int main(){
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    const int stdby = 19, a1 = 16, a2 = 17, pwma = 18;
    const std::vector<int> pins = {stdby,pwma,a1,a2};
    ROB14450 motorDriver(pins);
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
