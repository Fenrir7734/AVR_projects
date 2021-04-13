#include <avr/io.h>                //Biblioteka Wej/Wyj
#include <avr/interrupt.h>
#include <util/delay.h>		//biblioteka zawierająca funkcje opóźniające

volatile uint8_t state = 0;

ISR(TIMER0_COMP_vect) {
	state = 1;
}

uint8_t keypad() {

	uint8_t col;
	uint8_t row;

	for(uint8_t i = 4; i <= 7; i++) {
		PORTA = ~_BV(i);	//uaktywnienie kolumny i;

		col = i - 4;				//numer aktywnej kolumny
		row = (PINA | 0xF0) ^ 0xFF;	//Jezeli jakiś przycisk w wierszu jest naciśnięty
									//to tylko odpowiadający za niego bit będzie miał wartość 1
									//dotyczy to tylko bitów od 3 do 0.

		for(uint8_t j = 0; j <= 3; j++) {
			if(row & _BV(j)) {							//sprawdza czy któryś z bitów od 3 do 0 jest ustawiony
														//jeżeli tak, oznacza to wciśnięty przycisk
				
				return j + (j * 3) + col + 1;			//numer_wiersza + (waga_wiersza) + numer_kolumny + 1
														//kolejne wiersze mają wagi odpowiednio 0, 3, 6, 9
														//waga wiersza razem z numerem wiersza wskazuje na pierwszy przycisk w tym wierszu (kolumna 1)
														//dodając do tego numer kolumny otrzymujemy numer przycisku który jest wciśnięty
														//przyciski mają numery od 1 - 16. dlatego dodaje na koniej 1.

			}
		}
	}

	return 0;	//Jeżeli żadnej przycisk nie jest wciśnięty funkcja zwraca wartość 0.
}


int main(void) {
    
    DDRA = 0xF0;     
    DDRC = 0xFF;    
    DDRB = 0xFF;

	TCCR0 |= _BV(CS01) | _BV(WGM01);
	TIMSK |= _BV(OCIE0);
	OCR0 = 19;	// 5ms 

	sei();

    while(1) {
             
 		while(state) {
			PORTB = keypad();
			state = 0;
		}
    }
}
