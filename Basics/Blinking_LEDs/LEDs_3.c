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
    */
    PORTA |= _BV(7);

    /*
    Zmienna wskazująca na kolejny bit którego wartość ma
    być ustawiona na wartość 1.
    */
    int8_t i = 6; 

    while(1) {
        _delay_ms(100);

        /*
        Ustawienie wartości 1 na pinie o numerze i portu A
        przy wykorzystaniu sumy bitowej.
        */
        PORTA |= _BV(i);

        /*
        Zmniejszenie wartości zmiennej i tak aby wskazywała na 
        pin na którym ma być ustawiona wartość 1 w następnej 
        iteracji pętli.
        */
        i--;

        /*
        Sprawdzenie czy wartość na pinie o numerze 0 portu A
        jest równa 1. Jeżeli warunek jest spełniony wartości 
        na pinach portu przywracane są do stanu poczatkowego
        (z stanem wysokim jedynie na pinie o numerze 7) oraz 
        odpowiednio zmieniana jest wartość zmiennej i.
        Warunek PORTA & _BV(0) wykorzystuje iloczyn bitowy 
        którego wynikiem jest wyzerowanie wszystkich bitów
        poza bitem o numerze 0 jeżeli ten ma ustawioną na
        porcie A wartość 1. 
        */
        if(PORTA & _BV(0)) {
            _delay_ms(100);

            /*
            Przywrócenie stanu początkowego portu A poprzez
            wyzerowanie wszystkich bitów oprócz bitu o nr.7.
            który na porcie A zawsze znajduje się w stanie 
            wysokim.
            */
            PORTA &= _BV(7);

            /*
            Ustawienie wartości zmiennej tak aby wskazywała
            na następny bit który ma być ustawiony w stan wysoki.
            Skoro port A wrócił do stanu początkowego z pinem nr.
            7 w stanie wysokim to kolejny pin który ma przejść w 
            stan wysoki ma nr.6
            */
            i = 6;
        }
    }

}
