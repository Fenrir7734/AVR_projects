#include <avr/io.h> //biblioteka do obsługi wejścia/wyjścia
#include <avr/interrupt.h>	//biblioteka do obsługi przerwań
//#define F_CPU 1000000L //Ustawienei częstotliwości taktowania mikrokontrolera na 1MHz
                         //Definicja ta nie jest wykożystywana ponieważ częstotliwość
                         //została usatwiona na 1MHz w śodowisku AVR Studio w zakładce
                         //Debug -> AVR Simulator Options

/*
Zmimenna globalna która będzie służyć
do zliczania liczby wykonań procedury
obsługi przerwania.
volatile jest słowem kluczowym które 
informuje kompilator jak ma traktować
daną zmienna. Skutki korzystanie z tego
modyfikatora opiszę w następnym zadaniu.
*/
volatile uint8_t counter = 0;

/*
Procedura obsługi przerwania która odpowiada
za przerwanie Compare Match. Procedura ta
zostanie wykonana jeżeli w rejestrze TIMSK 
została ustawiona flaga OCIE0 oraz w rejestrze
TIFR flaga OCF0.
*/
ISR(TIMER0_COMP_vect) {
    /*
    Jeżeli wystąpiło 10 przerwań co odpowiada
    1s zostanie zanegowana wartość PA7 oraz 
    wartość zmiennej counter zostanie wyzerowana,
    rozpoczynając odliczanie kolejnej sekundy.
    */
    if(counter == 10) {
        PORTA ^= _BV(PA7);
        counter = 0;
    } 
    
    /*
    PA0 jest negowane a zmienna counter 
    inkrementowana. Wynikiem tej i powyższych
    operacji jest negowanie PA0 co 100ms oraz 
    PA7 co 1s.Wartość rejestru TCNT0 oraz flaga 
    OCF0 są zerowane automatycznie.
    */
    PORTA ^= _BV(PA0);
    counter++;
    
}

int main(void) {

    /*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu A jako portu wyjściowego
    0xFF = 0b11111111
    */	
    DDRA = 0xFF;
    
    /*
    Ustawienie timera w tryb CTC oraz źródła sygnału
    taktuającego na sygnał wewnętrzny wraz z 
    preskalerem 1024.
    */
    TCCR0 |= _BV(CS00) | _BV(CS02) | _BV(WGM01);
    
    /*
    Uaktywnienie przerwań w timer0. Ustawienei generowania
    żądania przerwania na Compare Match.

    TIMSK jest rejestrem w którym możemy uaktywnić 
    przerwania dla timerów oraz zdecydowac kiedy te 
    przerwania mają być generowane. Do tego celu służą
    dwie flagi: TOIE0 oraz OCIE0. Pierwsza generuje
    żądanie przerwania jeżeli dojdzie do przepełnienia
    licznika a więc gdy flaga TOV0 w rejestrze TIFR 
    zostanie ustawiona. Gdy flaga OCIE0 jest ustawiona
    żądanie przerwania jest generowane gdy w rejestrze TIFR
    zostanie ustawiona flaga OCF0 sygnalizująca wystąpienie
    dopasowania wartości z OCR0 z wartością z rejestru 
    licznika TCNT0.
    Domyslnie flagi TOIE0 i OCIE0.są wyzerowane.
    */
    TIMSK |= _BV(OCIE0);
    
    /*
    Ustawienie wartości rejestru OCR0.
        
    1MHz/1024 = 976.6Hz
    1/976.6Hz = 1.024ms
    100ms/1.024ms = 98
    98 - 1 = 97 	<- ponieważ liczymy od 0
    */
    OCR0 = 97;

    /*
    Ustawienie globalnej flagi zezwolanie na przerwanie.
    */
    sei();

    while(1) {}
}
