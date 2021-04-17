#include <avr/io.h>                //biblioteka do obsługi wejścia/wyjścia
#include <avr/interrupt.h>		   //biblioteka do obsługi przerwań
#include <util/delay.h>		//biblioteka zawierająca funkcje opóźniające

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
Klawiatura 4x4 podzielona jest na 4 kolumny (4 starsze
bity PORTA) i 4 rzędy (4 młodsze bity). Aby odczytać
stan klawiatury należy na kolejne kolumny podawać
stan niski tym samym "uaktywniając je". Jeżeli jakiś
przycisk znajdujący się w uaktywnionej kolumnie jest
wciśnięty wtedy stan rzędu w którym się znajduje 
zmieni się na niski. Kod przycisku składa się więc
z bitu kolumny i rzędu w którym znajduje się ten przycisk.
Jeżeli żaden klawisz nie jest wciśnięty i żadna kolumna
aktywna to cały port do którego podłączona jest klawiatura
znajduje sie w stanie wysokim. Sprawdzając stan
klawiatury można również aktywować kolejne rzędy
a odczytywać kolumny, rezultat będzie taki sam.

Celem poniżej funkcji jest sprawdzenie  aktualnego 
stanu  klawiatury i zwrócenie numeru naciściętego 
klawisza. Numery zawierają się w przedziale od
1 do 16. Jeżeli żaden klawisz nie jest naciśnięty
funkcja zwróci wartość 0. Dodatkowo funkcja zapisuje
kod wciśniętego klawisza do rejestru PORTC.
W celu odczytania stanu klawiatury uaktywniam 
kolejne kolumny a odczytuje stan poszczególnych
rzędów.
*/
uint8_t keypad() {
	
	/*
	Zmienna która docelowo będzie przechowywać
	kod naciśniętego klawisza.
	*/
	uint8_t key;

	/*
	Pętla w której odbywa się sprawdzanie
	stanu klawiatury
	*/
	for(uint8_t i = 4; i <= 7; i++) {

		/*
		Ustawienie na PORTA stanu niskiego
		na bitach od 4 do 7 co skutkuje 
		uaktywnieniem kolejnych kolumn.
		*/
		PORTA = ~_BV(i);	
		
		_delay_ms(1);

		/*
		Odczytanie stanu PINA i zanegowanie go.
		Jeżeli jakiś klawisz jest naciśnięty wtedy
		jedynie 2 bity w zmiennej key będą w stanie
		wysokim. Głównym powodem tej negacji jest
		ułatwienie mi napisania dwóch kolejnych
		warunków. Drugi powód to bardziej przejrzysty
		sposób wyświetlenia kodu przycisku na PORTC.
		*/
		key = PINA ^ 0xFF;	

		/*
		Warunek poprzez wyzerowanie bitów od 4 do 7
		odpowiadających za kolumny sprawdza czy któryś
		rząd jest aktywny. Jeżeli tak oznacza to że jakiś
		przycisk w aktualnie sprawdzanej kolumnie jest 
		naciśnięty a więc następnie wykona się pętla której
		celem jest ustalenie w którym rzędzie znajduje się 
		ten przycisk.
		Natomiast jeżeli maskowanie da wartość 0 oznacza to
		że żaden przycisk w tej kolumnie nie jest
		aktywny a więc główna pętla tej funkcji przejdzie
		od razu do sprawdzania kolejnej kolumny.
		Dzięki takiemu rozwiązaniu instrukcje w ciele warunku if
		wykonają się maksymalnie jeden raz jeżeli 
		jakiś przycisk jest rzeczywiście wciśnięty. Pozwala
		to zaszczędzić czas poprzez pozbycie się niepotrzebnych
		operacji.
		*/
		if(key & 0x0F) {
			
			/*
			Pętla której zadaniem jest ustalenie który
			rząd jest aktywny.
			*/
			for(uint8_t j = 0; j <= 3; j++) {

				/*
				Warunek sprawdza czy bit o numerze j
				znajduje się w stanie wysokim czyli
				czy odpowiedni rzad jest aktywny.
				*/
				if(key & _BV(j)) {
				
					/*
					Wpisuje kod naciśniętego przycisku do PORTC
					*/							
					PORTC = key;

					/*
					Poniższa instrukcja return zwraca numer 
					naciśniętego przycisku. Obliczenia:
					
					przesunięcie + numer_kolumny + 1

					Kolumny i rzędy numeruję od 0 do 3.

					numer_kolumny - numer kolumny w której wykryto 
					naciśnięty przycisk. Zmienna i ma wartość 
					odpowiadającą numerowi bitu przyporządkowanego 
					dla danej kolumny, od 4 do 7. Aby uzyskać numer 
					kolumny należy odjąć od zmiennej 4.
					
					przesunięcie - odpowiada ilości przycisków które
					należy "przeskoczyć" aby otrzymać numer pierwszego
					przycisku w interesującym nas rzędzie.

					Dla przykładu jeżeli przycisk znajduje się w rzędzie 
					nr 2 kolumnie nr 3 to należy przeskoczyć 8 poprzednich
					przycisków tak aby "wskazać" na pierwszy przycisk w 2
					rzędzie a nasępnie dodać do tego numer kolumny w której
					znajduje się przycisk. Na koniec dodaję do wyniku 1 aby
					uzyskać numer przycisku z przedziału od 1 do 16 zamiast 
					od 1 do 15. Czyli::
					(2 * 4) + (7 - 4) + 1 = 12
					*/
					return (j * 4) + (i - 4) + 1;			

				}
			}
		}
	}

	/*
	Jeżeli żadnej przycisk nie jest 
	wciśnięty funkcja zwraca wartość 0.
	*/
	return 0;
}


int main(void) {
    
	/*
	Ustawienie PORTA tak aby 4 kolumny były wyjściami 
	oraz 4 rzędy wejściami.
	*/
    DDRA = 0xF0;  
	
	/*
	Ustawienie całego portu C i B jako portu wyjściowego
	*/  
    DDRC = 0xFF;   
    DDRB = 0xFF;

	/*
	Ustawienie wejść w stan wysoki i wyjść w stan niski.
	*/
	PORTA = 0x0F;

	/*
	Ustawienie timera w tryb CTC oraz źródła sygnału
	taktuającego na sygnał wewnętrzny wraz z 
	preskalerem 256
	*/
	TCCR0 |= _BV(CS02) | _BV(WGM01); 

	/*
	Uaktywnienie przerwań w timer0. Ustawienei generowania
	żądania przerwania na Compare Match.
	*/
	TIMSK |= _BV(OCIE0);			 

	/*
	Ustawienie wartości rejestru OCR0.
		
	1MHz/256 = 3906Hz
	1/3906Hz = 256us
	10ms/256us = 39
	39 - 1 = 38 	<- ponieważ liczymy od 0
	*/
	OCR0 = 38;	

	/*
	Ustawienie globalnej flagi zezwolanie na przerwanie.
	*/
	sei();

    while(1) {
             
		/*
		Timer będzie generował żądanie przerwania co 10ms.
		Za każdym razem zostanie wywołana procedura 
		obsługi przerwania która ustawi wartość zmiennej
		state na 1.Po przywróceniu normalnego przepływu
		sterowania, program wejdzie w blok tej instrukcji
		warunkowej i rozpocznie wykonywanie funkcji keypad().
		Wartość zwrócona przez tą funkcje zostanie zapisana
		w rejestrze PORTB a zmienna state zostanie zresetowana
		i pozostanie w stanie false do czasu wystąpienia kolejnego
		przerwania. Skutkuje to tym że stan klawiatury jest 
		sprawdzany co 10ms.
		*/
 		if(state) {
			PORTB = keypad();
			state = 0;
		}
    }
}
