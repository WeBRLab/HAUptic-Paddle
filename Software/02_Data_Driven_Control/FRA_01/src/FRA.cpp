#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include <math.h>
#include <hardware/timer.h>

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
float control(float kd, int desired_velocity, int velocity){
    float maxspeed = 5000.0;
    uint32_t amplification_factor = 1;          //      [degree/Volt]
    uint32_t paddleRadius = 1;                  //      [m]
    float velocity_error = velocity - desired_velocity;
    float control_signal = kd*(velocity_error);
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
    int Kd = 55.0;                          //      Proportional control constant
    float desired_velocity = 10.0;        //     0 degrees + 60 units offset;
    int current_position;
    int last_position;
    int last_last_position;
    int current_velocity;
    int s_t;
    int e_t;
    int dt;
    int control_signal;
    int count;

	    //  START CONTROL LOOP
    float freqs[46] ={0.1,0.12,0.14,0.16,0.18,0.2,0.22,0.24,0.26,0.28,0.3,0.32,0.34,0.36,0.38,0.4,0.42,0.44,0.46,0.48,0.5,0.52,0.54,0.56,0.58,0.6,0.62,0.64,0.66,0.68,0.7,0.72,0.74,0.76,0.78,0.8,0.82,0.84,0.86,0.88,0.9,0.92,0.94,0.96,0.98,1};
    int index;
    while (1){
        count = count + 1;
        s_t = time_us_32();
        last_last_position = last_position;
        last_position = current_position;
        // sleep_ms(2);
        current_position = adc_read();
        e_t = time_us_32();
        dt = e_t - s_t;
        // current_velocity = getVelocity(current_position, last_position,dt);
        index = (int)round(count/2000) - 26843*0  +35;
        printf("\n %.2d ...", (int)index);
        printf("%.2f...", (float)current_position);
        // printf("%.2f...", (float)current_velocity);
        printf("%.2f...", (float)control_signal);
        printf("%.2f...", (float)s_t);

        // control_signal = 5000*sin((int)freqs[index]);
        control_signal = 100000*sin((float)freqs[index]*(3.1415/32*count));
        drive(control_signal);
    }
}