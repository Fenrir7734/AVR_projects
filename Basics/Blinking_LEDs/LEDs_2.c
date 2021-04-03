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
	Ustawienie wartości 1 na pinie 7 portu A
	przy użyciu makra _BV(x), które jest równoważne 
	przesunięciu bitowemu (1<<x), oraz sumy bitowej.
	*/
	PORTA |= _BV(7);

	while(1) {
		
		/*
		Pętla negująca bity na które wskazuje zmienna i. 
		Blok instrukcji pętli wykonywany jest dla bitów o 
		numerach od 6 do 4.
		*/
		for(uint8_t i = 6; i >= 4; i--) {
			
			/*
			Instrukcja powodująca zanegowanie wartości bitu i portu A
			i pozostawiająca pozostałe wartości pinów tego portu bez zmian.
			Jest ona równoważna instrukcji:
			PORTA = PORTA ^ _BV(i); 
			*/
			PORTA ^= _BV(i);
			
			/* 
			Opóźnienie wynoszące 100ms
			*/
			_delay_ms(100);
		}

		/*
		Pętla negująca bity na które wskazuje zmienna i. 
		Blok instrukcji pętli wykonywany jest dla bitów o 
		numerach od 4 do 6. Blok instrukcji pętli jest 
		analogiczny do bloku instrukcji pętli poprzedniej.
		*/
		for(uint8_t i = 4; i <= 6; i++) {
			PORTA ^= _BV(i);
			_delay_ms(100);
		}
	}
}



