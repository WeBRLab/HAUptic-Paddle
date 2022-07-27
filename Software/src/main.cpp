#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include <math.h>
#include <pico_rob14450.h>
#include <pico_allegroHalleffect.h>
#include "pin_definitions.h"

const uint PIN = 5;

// FUNCIONS CALLED TO INITIALIZE SENSOR AND DRIVER //
void init_sensor(){
    adc_init();
    adc_gpio_init(27);
    adc_select_input(1);
    gpio_init(PIN);
    gpio_set_dir(PIN, GPIO_OUT);
    uint16_t position = 0;
}


//  CONTROLLER //
float control(float kp, int desired_position, int position){
    float maxspeed = 5000.0;
    uint32_t amplification_factor = 1;          //      [degree/Volt]
    uint32_t calibrationCenter = 3050.00;       //      unitless; based on the sensor location
    uint32_t paddleRadius = 1;                  //      [m]
    float travel = position - 3200.0;
    float position_error = (travel - desired_position);
    float control_signal = kp*(position_error);
    if (control_signal > maxspeed){
        control_signal = maxspeed;
    }
    if (control_signal < -1*maxspeed){
        control_signal = -1*maxspeed;
    }
    int control_output = round(control_signal);
    printf("%.2f \n", (float)control_output);    
    return control_output;
}


// MAIN LOOP //
int main()
{
        // Import variables
    const std::vector<int> motor_pins = {STDBY_PIN,PWMA_PIN,A1_PIN,A2_PIN};
        // Insantiate modules
    ROB14450 driver(motor_pins);
    driver.init();
    init_sensor();                          //      "turn on" sensor
    stdio_init_all();                       //      "turn on" logic board
    
    //  Declare CONSTANTS
    int Kp = 35.0;                           //      Proportional control constant
    float desired_position = 80.0;           //     0 degrees + 60 units offset;
    
    //  START CONTROL LOOP
    while (1){
        int current_position = adc_read();
        int control_signal = control(Kp,desired_position, current_position); 
        driver.drive(control_signal);
    }
}











// int main(){
//      gpio_init(stdby);
//     gpio_set_dir(stdby,GPIO_OUT);
//     gpio_put(stdby,true);
//     gpio_init(a1);
//     gpio_set_dir(a1,GPIO_OUT);
//     gpio_init(a2);
//     gpio_set_dir(a2,GPIO_OUT);
//     gpio_set_function(pwma,GPIO_FUNC_PWM);
//     int slice_= pwm_gpio_to_slice_num(pwma); 
//     int channel_= pwm_gpio_to_channel(pwma);
//     pwm_set_enabled(slice_, true); 
//     pwm_set_wrap(slice_, 12500);
//     pwm_set_phase_correct(slice_, false);
//     pwm_set_gpio_level(pwma,1000);
//     sleep_ms(1);

//     while (1){
//         gpio_put(a1,false);
//         gpio_put(a2,true);
//         sleep_ms(1000);
//         gpio_put(a1,true);
//         gpio_put(a2,false);
//         sleep_ms(1000);
//     }   
// }