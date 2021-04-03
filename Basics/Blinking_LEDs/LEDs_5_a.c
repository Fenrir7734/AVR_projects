#include <avr/io.h>
#include <util/delay.h>

int main(void){
	
	/*
	Ustawienie kierunku przesyłania danych. 
	Ustawienie całego portu A jako portu wyjściowego
	0xFF = 0b11111111
	*/
	DDRA = 0xFF;

	/*
	Ustawienie stanu początkowego portu A
	Ustawienie wartości 1 na pinie 7 portu A.
	z wykorzystaniem sumy bitowej.
	*/
	PORTA |= _BV(7);


	while(1) {
		_delay_ms(100);

		/*
		Pętla ustawiająca stan wysoki na pinach
		o numerach od 6 do 4 portu A
		*/
		for(int8_t i = 6; i >= 4; i--) {
			_delay_ms(100);

			/*
			Ustawienie wartości 1 na pinie o numerze i
			portu A z wykorzystaniem sumy bitowej.
			*/
			PORTA |= _BV(i);
		}

		_delay_ms(100);

		/*
		Ustawienie stanu wyjściowego dla drugiej części cyklu
		Wyzerowanie wszystkich bitów i ustawienie bitu nr 0 na wartość 1
		Na początku wykonywany jest iloczyn bitowy który powoduje
		wyzerowanie wszystkich wartości na porcie A (0x0 = 0b00000000),
		następnie na wyniku iloczynu bitowego wykonywana jest suma 
		logiczna powodująca ustawienie wartości 1 na bicie nr.0. 
		Uzyskany wynik ustawiany jest na pinach portu A.
		*/
		PORTA = (PORTA & 0x0) | _BV(0); 

		/*
		Pętla ustawiająca stan wysoki na pinach
		o numerach od 1 do 3 portu A
		*/
		for(int8_t i = 1; i <= 3; i++) {
			_delay_ms(100);

			/*
			Ustawienie wartości 1 na pinie o numerze i
			portu A z wykorzystaniem sumy bitowej.
			*/
			PORTA |= _BV(i);
		}
		
		_delay_ms(100);

		/*
		Ustawienie stanu wyjściowego dla kolejnej iteracji.
		Wyzerowanie wszystkich bitów i ustawienie bitu nr 7 na wartość 1
		Instrukcja jest analogiczna do poprzedniej.
		*/
		PORTA = (PORTA & 0x0) | _BV(7); 
	}

}

