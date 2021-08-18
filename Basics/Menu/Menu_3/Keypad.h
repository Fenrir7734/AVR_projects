#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <avr/interrupt.h>
#include <util/delay.h>	


volatile uint8_t state;

void init_keypad();
void init_interrupts();
uint8_t keypad();

#endif
