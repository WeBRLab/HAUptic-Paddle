#ifndef PICO_LCD_I2C_H_
#define PICO_LCD_I2C_H_

/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include <string>

/* Example code to drive a 16x2 LCD panel via a I2C bridge chip (e.g. PCF8574)

   NOTE: The panel must be capable of being driven at 3.3v NOT 5v. The Pico
   GPIO (and therefor I2C) cannot be used at 5v.

   You will need to use a level shifter on the I2C lines if you want to run the
   board at 5v.

   Connections on Raspberry Pi Pico board, other boards may vary.

   GPIO 4 (pin 6)-> SDA on LCD bridge board
   GPIO 5 (pin 7)-> SCL on LCD bridge board
   3.3v (pin 36) -> VCC on LCD bridge board
   GND (pin 38)  -> GND on LCD bridge board
*/
// commands
class PicoLCDI2C
{
    public:
        PicoLCDI2C();
        void i2c_write_byte(uint8_t val);
        void lcd_toggle_enable(uint8_t val);
        void lcd_send_byte(uint8_t val, int mode);
        void lcd_clear();
        void lcd_set_cursor(int line, int position);
        void lcd_string(const char *s);
        void lcd_init();
        void lcd_char(char val);
        void print(std::string val, int line);

        const int MAX_LINES = 2;
        const int MAX_CHARS = 16;

    private:
        const int LCD_CLEARDISPLAY = 0x01;
        const int LCD_RETURNHOME = 0x02;
        const int LCD_ENTRYMODESET = 0x04;
        const int LCD_DISPLAYCONTROL = 0x08;
        const int LCD_CURSORSHIFT = 0x10;
        const int LCD_FUNCTIONSET = 0x20;
        const int LCD_SETCGRAMADDR = 0x40;
        const int LCD_SETDDRAMADDR = 0x80;

        // flags for display entry mode
        const int LCD_ENTRYSHIFTINCREMENT = 0x01;
        const int LCD_ENTRYLEFT = 0x02;

        // flags for display and cursor control
        const int LCD_BLINKON = 0x01;
        const int LCD_CURSORON = 0x02;
        const int LCD_DISPLAYON = 0x04;

        // flags for display and cursor shift
        const int LCD_MOVERIGHT = 0x04;
        const int LCD_DISPLAYMOVE = 0x08;

        // flags for function set
        const int LCD_5x10DOTS = 0x04;
        const int LCD_2LINE = 0x08;
        const int LCD_8BITMODE = 0x10;

        // flag for backlight control
        const int LCD_BACKLIGHT = 0x08;

        const int LCD_ENABLE_BIT = 0x04;

        // By default these LCD display drivers are on bus address 0x27
        const int addr = 0x27;

        // Modes for lcd_send_byte
        const int LCD_CHARACTER = 1;
        const int LCD_COMMAND = 0;



};

#endif