#include <avr/io.h> 		//biblioteka do obsługi wejścia/wyjścia
#include <util/delay.h>		//biblioteka zawierająca funkcje opóźniające
#define F_CPU 1000000L		//Ustawienei częstotliwości taktowania mikrokontrolera na 1MHz

/*
Zmimenna globalna która będzie służyć
do zliczania liczby wykonań funkcji timer10ms
*/
uint8_t t = 0;

void timer10ms() {
	/*
	Pętla która będzie wykonywała się do czasu
	aż wartość rejestru TCNT0 będzie różna od 
	wartości ustawionej w rejestrze OCR0 a więc
	przez 10ms. Gdy wartość w obu rejestrach będą
	takie same zostanie ustawiona flaga OCF0 w 
	rejestrze TIFR a program wyjdzie z tej pętli.
	*/
	while(!(TIFR & _BV(OCF0)));
							 
	/*
	Reset flagi OCR0 w rejestrze TIFR0
	Rejestr TCNT0 w tym trybie jest resetowany 
	automatycznie.
	*/			
	TIFR |= _BV(OCF0);
}

void czas_1s() {
	/*
	Jeżeli funkcja ta została wywołana znaczy to że
	właśnie wykonała się funkcja timer10ms() a więc
	mineło 10ms. W związku z tym zmienna t jest 
	inkrementowana.
	*/
	t++;

	/*
	Jeżeli zmienna t == 100 oznacza to że funkcja 
	timer10ms wykonała się 100 razy a więc mineła 
	1 sekunda. 10ms * 100 = 1s.
	*/
	if(t == 100) {

		/*
		Negacja PA7 aby zasygnalizować upłynięcie 1 sekundy.
		*/
		PORTA ^= _BV(7);

		/*
		Reset wartości zmiennej t aby rozpocząć odliczanie 
		kolejnej sekundy.
		*/
		t = 0;
	}
}

int main(void)
{   
	
	/*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu A jako portu wyjściowego
    0xFF = 0b11111111
    */
	DDRA = 0xFF;

	/*
    Ustawienie stanu początkowego portu A,
	wszystkie piny będą w stanie niskim.
    */ 
	PORTA = 0x00;

	/*
	TCCR0 jest rejestrem Timer0 który jest używany do ustawiania
	trybów timera. Aby Timer mógł działać należy ustawić mu 
	źródło sygnału taktującego. Timer domyślnie nie ma 
	ustawionego źródła zegara, jest zatrzymany, więc trzy 
	Clock Select Bits (CS02, CS01, CS00) są ustawione na 0.
	W tym przypadku zostanie wykorzystany wewnętrzny sygnał
	taktujący wraz z preskalerem (dzielnikiem) który umożliwia
	podzielenie tego sygnału poprzez jedną z wybranych wartości.
	Aby ustawić wartość preskalera na 256 wystarczy ustawić 
	CS02 na wartość jeden. Wartości bitów CS01 oraz CS00 nie 
	musimy zmieniać poniważ wiemy że mają ustawioną wartość 0.
	
	Ponadto tryb pracy Timera0 możemy zmieniać poprzez manipulacje
	bitami WGM01 i WGM00. Timer domyślnie znajduje się w normalnym
	trybie pracy (WGM01 i WGM00 są ustawione na 0). Aby zmienić tryb
	na CTC należy ustawić WGM01 na wartość 1, WGM00 jak wiemy ma już
	ostawioną wartość 0. 

	CTC - tryb pracy timera w którym licznik będzie zliczał impulsy 
	aż do momentu uzyskania wartości określonej w rejestrze OCR0. 
	Wartość z OCR0 będzie porównywana z wartością rejestru TCNT0
	(rejestr zliczający impulsy). Gdy zliczanie dojdzie do końca
	stan rejestru TCNT0 jest z powrotem ustawiany na 0.
	*/
	TCCR0 |= _BV(WGM01) | _BV(CS02); 
	
	/*
	Ustawienie wartości rejestru OCR0. Wartość w tym rejestrze
	jest porównywana z zawartością rejestru TCNT0. Gdy wartości
	te będą równe, flaga OCF0 w rejestrze TIFR zostanie ustawiona
	a TCNT0 zresetowany. 

	1MHz/256=3.906kHz  <- 256 to wartość preskalera
	1/3.906kHz=256us
	10ms/256us=39
	39 - 1 = 38 	<- ponieważ liczymy od 0

	38 jest to liczba impulsów jakie ma zliczyć timer0. Przy obecnym
	ustawieniu częstotliwości taktowania mikrokontrolera na 1MHz oraz
	preskalera na 256 wartość ta odpowiada czasowi równemu 10ms.

	TCNT0 jest domyślnie ustawiony na wartość 0.
	*/
	OCR0 = 38;

	/*
    Główna pętla programu, pętla nieskończona.
    */
    while(1) {
		
		/*
		Wywołanie funkcji odmierzającej 10ms. Program wyjdzie z tej 
		funkcji dopiero po minięciu tych 10ms.
		*/
		timer10ms();

		/*
		Wartość PA0 zmienia się co 10ms, po zakończeniu wykonywania
		funkcji timer10ms().
		*/
		PORTA ^= _BV(0);

		/*
		Funkcja odmierzająca 1s na podstawie zliczeń liczby wykonań funkcji
		timer10ms()
		*/
		czas_1s();
	}
}
