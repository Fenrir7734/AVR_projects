#include <avr/io.h> 		//biblioteka do obsługi wejścia/wyjścia
#include <util/delay.h>		//biblioteka zawierająca funkcje opóźniające
#define F_CPU 1000000L		//Ustawienei częstotliwości taktowania mikrokontrolera na 1MHz


int main(void)
{   
	/*
	Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu A jako portu wyjściowego
	*/
	DDRA = 0xFF;
	
	/*
    Ustawienie stanu początkowego portu A,
	wszystkie piny będą w stanie niskim.
    */ 
	PORTA = 0x00;

	
	/*
	Ustawienie timera w tryb CTC oraz preskalera na 1024
	*/
	TCCR0 |= _BV(WGM01) | _BV(CS02) | _BV(CS00); 

    while(1) {

		/*
		Pętla ta powoduje generowanie fali o kwadratowym
		kształcie poprzez ustawianie stanu niskiego na 
		PA0, odczekaniu 50ms, następnie ustawieniu stanu
		wysokiego na PA0 i odczekaniu 150ms. Jedno wykonanie
		tej pętli odpowiada jednemu okresowi fali o czasie 
		200ms.
		*/
		
		/*
		Ustawienie wartości rejestru OCR0.
		
		1MHz/1024 = 976.6Hz
		1/976.6Hz = 1.024ms
		50ms/1.024ms = 48.828 = 48
		48 - 1 = 47 	<- ponieważ liczymy od 0
		*/
		OCR0 = 47;

		/*
		Reset flagi OCR0 w rejestrze TIFR0
		Rejestr TCNT0 w trybie CTC jest resetowany 
		automatycznie po tym jak wartości rejestrów
		TCNT0 oraz OCR0 zrównają się.
		*/
		TIFR |= _BV(OCF0);

		/*
		Ustawienie stanu 0 na PORTA który będzie trwać
		50ms.
		*/
		PORTA = 0;

		/*
		Pętla która będzie wykonywała się do czasu
		aż wartość rejestru TCNT0 będzie różna od 
		wartości ustawionej w rejestrze OCR0 a więc
		przez 50ms Gdy wartość w obu rejestrach będą
		takie same zostanie ustawiona flaga OCF0 w 
		rejestrze TIFR a program wyjdzie z tej pętli.
		*/
		while(!(TIFR & _BV(OCF0)));
		
		/*
		Ustawienie wartości rejestru OCR0.
		
		1MHz/1024 = 976.6Hz
		1/976.6Hz = 1.024ms
		50ms/1.024ms = 146.48 = 146
		146 - 1 = 145 	<- ponieważ liczymy od 0
		*/
		OCR0 = 145;
		TIFR |= _BV(OCF0);

		/*
		Ustawienie stanu 1 na PORTA który będzie trwać
		150ms.
		*/
		PORTA = 1;

		/*
		Pętla analogiczna do poprzedniej, pędzie 
		wykonywała się przez 150ms.
		*/
		while(!(TIFR & _BV(OCF0)));

	}
}
