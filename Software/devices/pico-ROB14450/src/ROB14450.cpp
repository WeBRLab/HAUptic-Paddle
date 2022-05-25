#include "include/ROB14450.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

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
    // Find the slice and channel that the chosen pwm pin is on
    slice_= pwm_gpio_to_slice_num (pwma_); 
    channel_= pwm_gpio_to_channel (pwma_);
    // Enable pwm on the selected slice
    pwm_set_enabled (slice_, true); 
}

void ROB14450::init()
{
    gpio_put(stdby_,true);
    // Find the wrap that will yield the desired PWM frequency. The base clock frequency
    //      is 125 MHz. The PWM frequency is the base clock frequency divided by the wrap.
    pwm_set_wrap (slice_, 12500);
    // Determine whether phase correction is to be used or not. When enabled, phase correction
    //      counts UP to the LEVEL (see .drive()) and then counts BACK DOWN to zero, before starting
    //      over. When disabled, phase correction counts UP to LEVEL, then resets to zero.
    pwm_set_phase_correct (slice_, false);
}

void ROB14450::drive(int speed)
{
    // Set directionality
    if(speed > 0)
    {
        gpio_put(a1_,false);
        gpio_put(a2_,true);
    }
    else
    {
        gpio_put(a1_,true);
        gpio_put(a2_,false);
    }

    // Sets the PWM pin to be the value of SPEED. Clamp assures that the value does not
    //      exceed the possible output value. The second input to the function is the LEVEL
    //      which determines the PWM duty cycle. Duty cycle can be calculated as the 
    //      LEVEL divided by the WRAP.
    pwm_set_gpio_level(pwma_,std::clamp(abs(speed),0,50000));
}

void ROB14450::shortBrake()
{
    gpio_put(a1_,false);
    gpio_put(a2_,true);
    pwm_set_gpio_level(pwma_,0);
}

void ROB14450::stop()
{
    gpio_put(a1_,false);
    gpio_put(a1_,false);
    pwm_set_gpio_level(pwma_,255);
}