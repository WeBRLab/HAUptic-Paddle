#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include <math.h>

// GLOBAL VARIABLES:
const uint stdby = 1;
const uint a1 = 2;
const uint a2 = 4;
const uint pwma = 26;
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
void init_driver(){
    gpio_init(stdby);
    gpio_set_dir(stdby,GPIO_OUT);
    gpio_put(stdby,true);
    gpio_init(a1);
    gpio_set_dir(a1,GPIO_OUT);
    gpio_init(a2);
    gpio_set_dir(a2,GPIO_OUT);
    gpio_set_function(pwma,GPIO_FUNC_PWM);
    int slice_= pwm_gpio_to_slice_num(pwma); 
    int channel_= pwm_gpio_to_channel(pwma);
    pwm_set_enabled(slice_, true); 
    pwm_set_wrap(slice_, 12500);
    pwm_set_phase_correct(slice_, false);
 }

// FUNCION CALLED TO DRIVE MOTOR //
void drive(int speed){
    if(speed <= 0) {
        gpio_put(a1,false);
        gpio_put(a2,true);
    }
    else {
        gpio_put(a1,true);
        gpio_put(a2,false);
    }
    speed = abs(speed);
    pwm_set_gpio_level(pwma,speed);
    sleep_ms(1);
}

//  CONTROLLER //
float control(float kp, int desired_position, int position){
    float maxspeed = 5000.0;
    int frequency = 5; 
    float control_signal = kp*sin(position*frequency*3.14/180);
    if (control_signal > maxspeed){
        control_signal = maxspeed;
    }
    if (control_signal < -1*maxspeed){
        control_signal = -1*maxspeed;
    }
    // control_signal = 0.0;
    int control_output = round(control_signal);
    return control_output;
}


// MAIN LOOP //
int main()
{
    //  INITIALIZE PADDLE
    init_sensor();                          //      "turn on" sensor
    init_driver();                          //      "turn on" driver
    stdio_init_all();                       //      "turn on" logic board
    
    //  Declare CONSTANTS
    int Kp = 1000;                           //      Proportional control constant
    float desired_position = 3300.0;           //     0 degrees + 60 units offset;
    
    //  START CONTROL LOOP
    while (1){
        int current_position = adc_read();
        int control_signal = control(Kp,desired_position, current_position); 
        printf("%.2f \n", (float)control_signal);    
        drive(control_signal);
    }
}