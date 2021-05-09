#include <avr/interrupt.h>

#include "Menu.h"

volatile uint8_t state = 0;

ISR(TIMER0_COMP_vect) {
	state = 1;
}

uint8_t keypad() {
	
	uint8_t key;

	for(uint8_t i = 4; i <= 7; i++) {
		PORTA = ~_BV(i);	
		
		_delay_ms(1);

		key = PINA ^ 0xFF;	

		if(key & 0x0F) {
			for(uint8_t j = 0; j <= 3; j++) {
				if(key & _BV(j)) {						
					key = (j * 4) + (i - 4) + 1;
					return key;			
				}
			}
		}
	}
	return 0;
}

int main()
{
	DDRA = 0xF0; 
	DDRC = 0xFF;  
	PORTC = 0x00;

	TCCR0 |= _BV(CS02) | _BV(WGM01); 
	TIMSK |= _BV(OCIE0);
	OCR0 = 38;
	sei();

	LCD_init();			
	LCD_clear();

    menu_init();

	while(1) {
        if(state) {
            menu_control(keypad());
        }
	}
}
