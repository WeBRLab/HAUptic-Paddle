#include "pico_rob14450.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

//#include <unistd.h>
#include <algorithm>
#include <cmath>

ROB14450::ROB14450(std::vector<int> pins)
    : stdby_(pins.at(0)), pwma_(pins.at(1)), a1_(pins.at(2)),a2_(pins.at(3))
{
    gpio_init(stdby_);
    gpio_set_dir(stdby_,GPIO_OUT);
    gpio_init(a1_);
    gpio_set_dir(a1_,GPIO_OUT);
    gpio_init(a2_);
    gpio_set_dir(a2_,GPIO_OUT);
    gpio_set_function(pwma_,GPIO_FUNC_PWM);
    slice_= pwm_gpio_to_slice_num (pwma_); 
    channel_= pwm_gpio_to_channel (pwma_);
    pwm_set_enabled (slice_, true); 
}

void ROB14450::init()
{
    gpio_put(stdby_,true);
    pwm_set_wrap (slice_, 12500);
    pwm_set_phase_correct (slice_, false);
}

void ROB14450::drive(int speed)
{
    if(speed <= 0){
        gpio_put(a1_,false);
        gpio_put(a2_,true);
    }
    else{
        gpio_put(a1_,true);
        gpio_put(a2_,false);
    }
    speed = abs(speed);
    pwm_set_gpio_level(pwma_,speed);
}

void ROB14450::shortBrake()
{    gpio_put(a1_,false);
    gpio_put(a2_,true);
    pwm_set_gpio_level(pwma_,0);
}

void ROB14450::stop()
{
    gpio_put(a1_,false);
    gpio_put(a1_,false);
    pwm_set_gpio_level(pwma_,255);
}