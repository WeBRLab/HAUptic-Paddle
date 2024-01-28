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

int getVelocity(int current_position, int last_position, int dt){
    float velocity = 10*(current_position - last_position)/(dt/1000);
    return velocity;
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
    uint32_t amplification_factor = 1;          //      [degree/Volt]
    uint32_t paddleRadius = 1;                  //      [m]
    float position_error = position - desired_position;
    float control_signal = kp*(position_error);
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
int main() {
    //  INITIALIZE PADDLE
    init_sensor();                          //      "turn on" sensor
    init_driver();                          //      "turn on" driver
    stdio_init_all();                       //      "turn on" logic board
    
   //  Declare CONSTANTS
    int current_position = 0;
    int last_position;
    int last_last_position;
    int current_velocity;
    int s_t;
    int e_t;
    int dt = 0.01;
    int T_ = 0;
    int count = 0;
    int control_signal;

    //  START CONTROL LOOP
    while (1){
        int t = 0;
        count = count +1;
        printf("\n Commanding: %.2f \n", (float)control_signal);
        while (t<=100000) {
            s_t = time_us_32();
            last_last_position = last_position;
            last_position = current_position;
            sleep_ms(2);
            current_position = adc_read();
            e_t = time_us_32();
            dt = e_t - s_t;
            t = t + dt;
            T_ = T_ + dt;
            current_velocity = getVelocity(current_position, last_position,dt);
            printf("%.2f ...", (float)current_velocity);
            printf("%.2f ...", (float)current_position);
            int current_position = adc_read();
            printf("%.2f \n", (float)T_);    
            control_signal = 0+100*count; 
            drive(control_signal);
        }
    }
}