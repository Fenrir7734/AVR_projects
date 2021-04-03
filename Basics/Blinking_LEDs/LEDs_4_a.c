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
	Ustawienie wartości 1 na pinie 3 portu A.
	z wykorzystaniem sumy bitowej.
	*/
	PORTA |= _BV(3);


	while(1) {

		/*
		Pętla ustawiająca stan wysoki na pinach
		o numerach od 2 do 0 portu A
		*/
		for(int8_t i = 2; i >= 0; i--) {
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
		Wyzerowanie wszystkich bitów i ustawienie bitu nr 4 na wartość 1
		Na początku wykonywany jest iloczyn bitowy który powoduje
		wyzerowanie wszystkich wartości na porcie A (0x0 = 0b00000000),
		następnie na wyniku iloczynu bitowego wykonywana jest suma 
		logiczna powodująca ustawienie wartości 1 na bicie nr.4. 
		Uzyskany wynik ustawiany jest na pinach portu A.
		*/
		PORTA = (PORTA & 0x00) | _BV(4); 
		 
		/*
		Pętla ustawiająca stan wysoki na pinach
		o numerach od 5 do 7 portu A
		*/
		for(int8_t i = 5; i <= 7; i++) {
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
		Wyzerowanie wszystkich bitów i ustawienie bitu nr 3 na wartość 1
		Instrukcja jest analogiczna do poprzedniej.
		*/
		PORTA = (PORTA & 0x00) | _BV(3);
	}
}


