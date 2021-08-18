#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000L

/*
Zmienna globalna do której będzie 
zapisywany aktualny stan PORTA.
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
    Odczyt i zapis do zmiennej state
    aktualnego stanu PINA.
    */
    state = PINA;
}

int main(void) {

    /*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu A jako portu wejściowego
    0x00 = 0b00000000
    */
    DDRA = 0x00;

    /*
    Ustawienie całego portu C jako portu wyjściowego
    */
    DDRC = 0xFF;
    
    /*
    Ustawienie stanu początkowego portu C,
    wszystkie piny będą w stanie niskim.
    */  
    PORTC = 0x00;	

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
    5ms/256us = 20
    20 - 1 = 19 	<- ponieważ liczymy od 0
    */
    OCR0 = 19;
    
    /*
    Ustawienie globalnej flagi zezwolanie na przerwanie.
    */
    sei();

    while(1) {
        /*
        Przypisanie wartości zmiennej state do rejestru
        PORTC. 
        Timer będzie generował żądanie przerwania co 5ms.
        Za każdym razem zostanie wywołana procedura 
        obsługi przerwania która odczyta aktualny stan
        PORTA oraz zapisze go do zmiennej state. Po 
        przywróceniu normalnego przepływu sterowania 
        wartość tej zmiennej zostanie odczytana i 
        zapisana do rejestru PORTC.
        */
        PORTC = state;
    }
}
