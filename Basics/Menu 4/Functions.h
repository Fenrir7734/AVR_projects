#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "Menu.h"

#define START 2
#define RESET 6

void key_read();
void led_blink();
void one_led_blink();
void stopwatch();

uint8_t stopwatch_controls(uint8_t mode);
void stopwatch_write_seconds(uint8_t seconds);
void stopwatch_write_minutes(uint8_t minutes);

#endif
