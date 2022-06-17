/*
    Author: Chandler Stubbs
    Date: May 16, 2022
    Description:
        Header file for ROB14450 motor driver.
*/
#include <vector>
#include "pico/stdlib.h"

#ifndef ROB14450_HPP
#define ROB14450_HPP

class ROB14450
{
public:
    ROB14450(std::vector<int> pins); 
    
    // Functions
    void init();
    void drive(int duty);
    void shortBrake();
    void stop();

private:
    // Variables
    int stdby_,pwma_,a1_,a2_;
    uint slice_, channel_;
};

#endif  // CUSTOM_PID_NODE_HPP