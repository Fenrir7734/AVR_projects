#include "Keypad.h"

/*
Zmienna globalna która będzie ustawiana
na wartość 1 jeżeli wystąpi przerwanie
*/
volatile uint8_t state = 0;

/*
Procedura obsługi przerwania która odpowiada
za przerwanie Compare Match. Procedura ta
zostanie wykonana jeżeli w rejestrze TIMSK 
została ustawiona flaga OCIE0 oraz w rejestrze
TIFR flaga OCF0.
*/
ISR(TIMER0_COMP_vect) {
	/*
	Jeżeli wystąpiło przerwanie wartość zmiennej
	state jest ustwiana na 1
	*/
	state = 1;
}

/*
Funkcja odpowiadająca za odczyt klawiszy. Zasadę jej 
działania opisałem w sprawozdaniu z ćwiczenia nr 2.
*/
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

/*
Inicjalizacja timera i przerwań.
*/
void init_interrupts() {
	/*
    Ustawienie timera w tryb CTC oraz źródła sygnału
    taktującego na sygnał wewnętrzny wraz z 
    preskalerem 256
    */
    TCCR0 |= _BV(CS02) | _BV(WGM01); 

	/*
    Uaktywnienie przerwań w timer0. Ustawienie generowania
    żądania przerwania na Compare Match.
    */
	TIMSK |= _BV(OCIE0);

	/*
    Ustawienie wartości rejestru OCR0.
        
    1MHz/256 = 3906Hz
    1/3906Hz = 256us
    10ms/256us = 39
    39 - 1 = 38     <- ponieważ liczymy od 0
    */
	OCR0 = 38;

	/*
    Ustawienie globalnej flagi zezwolenie na przerwanie.
    */
	sei();
}

/*
Inicjalizacja klawiatury.
*/
void init_keypad() {
    DDRA = 0xF0; 
	PORTA = 0x0F;
}
