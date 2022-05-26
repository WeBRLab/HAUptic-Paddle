#include "include/pico_lcd_i2c.hpp"

/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

PicoLCDI2C::PicoLCDI2C() 
{
    // Constructor
}


/* Quick helper function for single byte transfers */
void PicoLCDI2C::i2c_write_byte(uint8_t val) {
#ifdef i2c_default
    i2c_write_blocking(i2c_default, addr, &val, 1, false);
#endif
}

void PicoLCDI2C::lcd_toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work
    int delay_us = 600;
    sleep_us(delay_us);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(delay_us);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(delay_us);
}

// The display is sent a byte as two separate nibble transfers
void PicoLCDI2C::lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void PicoLCDI2C::lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

// go to location on LCD
void PicoLCDI2C::lcd_set_cursor(int line, int position) {
    int val = (line == 1) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}

void PicoLCDI2C::lcd_char(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}

void PicoLCDI2C::lcd_string(const char *s) {
    while (*s) {
        lcd_char(*s++);
    }
}

void PicoLCDI2C::lcd_init() {
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}

// To faciliate end-user ability to input std::string type rather than c string methods.
void PicoLCDI2C::print(std::string val, int line) {
    const char *message  = val.c_str();
    lcd_set_cursor(line, 1);
    lcd_string(message);
}
// void PicoLCDI2C::print(const char *message[]) {
//     lcd_clear();
    
//     for (int m = 0; m < sizeof(message) / sizeof(message[0]); m += MAX_LINES) {
//         for (int line = 0; line < MAX_LINES; line++) {
//             lcd_set_cursor(line, (MAX_CHARS / 2) - strlen(message[m + line]) / 2);
//             lcd_string(message[m + line]);
//         }
//     }

// }