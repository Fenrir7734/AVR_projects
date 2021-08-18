
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
    Wszystkie piny portu A będą w stanie niskim.
    0x00 = 0b00000000
    */
    PORTA = 0x00;


    while(1) {

        /*
        Pętla ustawiająca stan wysoki na pinach
        o numerach od 3 do 0 portu A. Stan wysoki
        na pinie o numerze 3 jest ustawiana bez 
        opóźnienia.
        */
        for(int8_t i = 3; i >= 0; i--) {
            /*
            Ustawienie wartości 1 na pinie o numerze i
            portu A z wykorzystaniem sumy bitowej.
            */
            PORTA |= _BV(i);
            _delay_ms(100);
        }
        
        /*
        Ustawienie stanu wyjściowego dla drugiej części cyklu.
        Ustawienie stanu niskiego na wszystkich pinach portu A
        z wykorzystaniem iloczynu bitowego.
        */
        PORTA &= 0x00;
        
        /*
        Pętla ustawiająca stan wysoki na pinach
        o numerach od 4 do 7 portu A. Stan wysoki
        na pinie o numerze 4 jest ustawiana bez 
        opóźnienia.
        */
        for(int8_t i = 4; i <= 7; i++) {
            /*
            Ustawienie wartości 1 na pinie o numerze i
            portu A z wykorzystaniem sumy bitowej.
            */
            PORTA |= _BV(i);
            _delay_ms(100);
        } 
        
        /*
        Ustawienie stanu wyjściowego dla kolejnej iteracji.
        Ustawienie stanu niskiego na wszystkich pinach portu A
        z wykorzystaniem iloczynu bitowego.
        */
        PORTA &= 0x00;
    }
}
