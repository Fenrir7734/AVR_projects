#include <avr/io.h>			//biblioteka do obsługi wejścia/wyjścia
#include <util/delay.h>		//biblioteka zawierająca funkcje opóźniające

//Główna funkcja programu
int main(void){
	
	/*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu A jako portu wejściowego
    0x00 = 0b00000000
    */
	DDRA = 0x00;

	/*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu B oraz C jako portu wyjściowego
    0xFF = 0b11111111;
    */
	DDRB = 0xFF;
	DDRC = 0xFF;

	/*
    Ustawienie stanu początkowego portu B oraz C,
	wszystkie piny obu rejestrów będą w stanie niskim.
    */  
	PORTB = 0x00;
	PORTC = 0x00;

	/*
	Licznik naciśniętych przycisków. Za każdym 
	razem gdy jakiś przycisk zostanie naciśnięty
	wartość tej zmiennej będzie inkrementowana
	*/
	uint8_t counter = 0;

	/*
	Zmienna przechowująca sumę wartości zwracanych
	poprzez przyciski Button. Za każdym 
	razem gdy jakiś przycisk zostanie naciśnięty
	wartość jaka została mu przypisana zostanie
	dodana do tej zmiennej.
	*/
	uint8_t sum = 0;
	
	/*
	Zmienna która posłuży do przechowywania 
	aktualnego stanu PINA. Domyślnie żaden
	przycisk nie jest wciśnięty na początku
	działania programu.
	*/
	uint8_t pinStatusCurrent = 0;

	/*
	Zmienna która posłuży do przechowywania 
	stanu PINA z poprzedniej iteracji głównej
	pętli programu. Domyślnie żaden przycisk 
	nie jest wciśnięty na początku działania 
	programu.
	*/
	uint8_t pinStatusPrev = 0;

	/*
    Główna pętla programu, pętla nieskończona.
    */
	while(1) {
		
		/*
		Odczyt i zapamiętanie aktualnego stanu portu A 
		przy czym 4 młodsze bity są zerowane z wykorzystaniem
		operacji logicznej AND i maski 0xF0 = 0b11110000
		Te 4 bity są zerowane ponieważ nie powinny być
		wykorzystywane w programie. Przyciski
		są podłączone tylko do pinów PORTA o numerach od 
		7 do 4.  
		*/
		pinStatusCurrent = PINA & 0xF0;

		/*
		Sprawdza czy stan portu A uległ zmienie. Jeżeli tak
		oznacza to że przycisk został właśnie albo wciśnięty
		albo puszczony. W pierwszej iteracji pętli zakładam że
		wartość pinStatusPrev jest równa 0 a więc żaden przycisk
		nie był wciśnięty.
		*/
		if(pinStatusCurrent != pinStatusPrev) {

			/*
			Jeżeli który kolwiek z 4 starszych bitów rejestru
			PORTA ma wartość jeden znaczy to że przycisk jest 
			wciśnięty. Blok instrukcji funkcji if wykonuje się
			tylko wtedy gdy którykolwiek z przycisków jest 
			wciśnięty. Jeżeli przycisk zostaje puszczony (wtedy
			wartosć pinStatusCurreny jest równa 0) to nic się 
			nie dzieje.
			*/
			if(pinStatusCurrent) { 

				/*
				Zwiększa wartość zmiennej sum w zależności
				od tego który przycisk został wciśnięty.
				*/
				switch(pinStatusCurrent) {

				/*
				Jeżeli przycisk nr. 1 jest wciśnięty 
				to dodaj do zmiennej sum wartość 1
				*/
				case 0b00010000: sum += 1; break; 	

				/*
				Jeżeli przycisk nr. 2 jest wciśnięty to 
				dodaj do zmiennej sum wartość 2
				*/
				case 0b00100000: sum += 2; break;

				/*
				Jeżeli przycisk nr. 3 jest wciśnięty to 
				dodaj do zmiennej sum wartość 4
				*/	
				case 0b01000000: sum += 4; break;

				/*
				Jeżeli przycisk nr. 4 jest wciśnięty to 
				dodaj do zmiennej sum wartość 8
				*/
				case 0b10000000: sum += 8; break;	
				default: break;
				}
				
				/*
				Nie zależnie od tego jaki przycisk został 
				wciśnięty, wartość licznika naciśniętych
				przycisków jest inkrementowana.
				*/
				counter++;

				/*
				Register PORTA składa się z 8 bitów więc
				maksymalna wartość jaką można do niego 
				zapisać to 255. Jeżeli któraś ze zmiennych
				sum lub counter przykroczy tą wartość zostaje
				ona wyzerowana.
				*/
				if(sum > 255) sum = 0;
				if(counter > 255) counter = 0;

				/*
				Zapisanie do rejestru portu B sumy
				wartości zwracanych przez przyciski.
				*/
				PORTB = sum;

				/*
				Zapisanie do rejestru portu C liczby 
				wciśniętych przycisków od początku działania
				programu. Wartosć rejestrów portu B i C jest
				aktualizowana tylko wtedy gdy któryś przycisk
				zostanie naciśnięty. 
				*/
				PORTC = counter;	
			} 
		}	
		/*
		Zapisanie aktualnej wartości zmiennej pinStatusCurrent
		(aktualny stan rejestru portu A) do zmienenj pinStatusPrev 
		(poprzedni stan rejestru porta A). Posłuży to w następnej 
		iteracji pętli do sprawdzenia czy stan tego portu uległ 
		zmianie.
		*/
		pinStatusPrev = pinStatusCurrent; 
	}
}

