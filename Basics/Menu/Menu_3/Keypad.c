#include "Keypad.h"

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
    state jest ustawiana na 1
    */
    state = 1;
}

/*
Celem poniżej funkcji jest sprawdzenie  aktualnego 
stanu  klawiatury i zwrócenie numeru naciśniętego  
klawisza. Numery zawierają się w przedziale od
1 do 16. Jeżeli żaden klawisz nie jest naciśnięty
funkcja zwróci wartość 0. W celu odczytania stanu 
klawiatury uaktywniam kolejne kolumny a odczytuje 
stan poszczególnych rzędów.
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
        warunków.
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
                czy odpowiedni rząd jest aktywny.
                */
                if(key & _BV(j)) {					

                    /*
                    Obliczenie numeru naciśniętego przycisku.
                    Obliczenia:
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
                    rzędzie a następnie dodać do tego numer kolumny w której
                    znajduje się przycisk. Na koniec dodaję do wyniku 1 aby
                    uzyskać numer przycisku z przedziału od 1 do 16 zamiast 
                    od 1 do 15. Czyli::
                    (2 * 4) + (7 - 4) + 1 = 12
                    */
                    key = (j * 4) + (i - 4) + 1;
                    return key;			
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

/*
Inicjalizacja timera i przerwań.
*/
void init_interrupts() {
    /*
    Ustawienie timera w tryb CTC oraz źródła sygnału
    taktującego na sygnał wewnętrzny wraz z 
    preskalerem 256
    */
    TCCR0 |= _BV(CS02) | _BV(WGM01); 

    /*
    Uaktywnienie przerwań w timer0. Ustawienie generowania
    żądania przerwania na Compare Match.
    */
    TIMSK |= _BV(OCIE0);

    /*
    Ustawienie wartości rejestru OCR0.
        
    1MHz/256 = 3906Hz
    1/3906Hz = 256us
    10ms/256us = 39
    39 - 1 = 38     <- ponieważ liczymy od 0
    */
    OCR0 = 38;

    /*
    Ustawienie globalnej flagi zezwolenie na przerwanie.
    */
    sei();
}

/*
Inicjalizacja klawiatury.
*/
void init_keypad() {
    DDRA = 0xF0; 
    PORTA = 0x0F;
}
