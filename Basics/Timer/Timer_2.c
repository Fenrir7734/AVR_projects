#include <avr/io.h> 		//biblioteka do obsługi wejścia/wyjścia
#include <util/delay.h>		//biblioteka zawierająca funkcje opóźniające
#define F_CPU 1000000L		//Ustawienei częstotliwości taktowania mikrokontrolera na 1MHz


int main(void)
{   

    /*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu A jako portu wyjściowego
    0xFF = 0b11111111
    */
    DDRA = 0xFF;

    /*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu B jako portu wejściowego
    0x00 = 0b00000000
    */
    DDRB = 0x00;

    /*
    Ustawienie stanu początkowego portu A,
    wszystkie piny będą w stanie niskim.
    */  
    PORTA = 0x00;

    
    /*
    Ustawienie timera w tryb normalny i źródła sygnału taktującego
    na T0 (PB0).Zegar na zbocze opadające. Timer domyślnie
    nie ma ustawionego źródła zegara, jest zatrzymany, więc
    trzy Clock Select Bits (CS02, CS01, CS00) są ustawione na 0.
    W związku z tym wystarczy ustawić wartości 1 na CS02 oraz 
    CS01 aby ustawić źrodło sygnału na to wymienione wyżej 
    Skoro wiemy że CS00 ma domyślnie wartość 0 nie musimy 
    nic zmieniać. W tym trybie timer będzie zachowywał się jak
    licznik zliczający 
    */
    //TCCR0 |= _BV(CS01) | _BV(CS02); 
    
    /*
    Ustawienie timera w tryb normalny i źródła sygnału taktującego
    na T0 (PB0).Zegar na zbocze wzrastające. Jest to tryb wykorzystywany
    w tym programie. Jeżeli do PB0 podpięty jest przycisk wtedy:
    -zbocze opadające - stan licznika TCNT0 jest aktualizowany w 
                         momęcie gdy przycisk jest puszczony
    -zbocze wzrastające - stan licznika TCNT0 jest aktualizowany w 
                         momęcie gdy przycisk jest naciskany. Puszczenie
                         przycisku nie wywołuje akualizacji TCNT0.

    */
    TCCR0 |= _BV(CS01) | _BV(CS02) | _BV(CS00); 
    
    //TCNT0 jest domyślnie ustawiony na wartość 0
    TCNT0 = 254;

    while(1) {
         /*
            Opóźnienie wynoszące 200ms
         */
        _delay_ms(200);


        /*
        Jeżeli dojdzie do przepełnienia licznika (wartość maksymalna to 255)
        sygnalizowane jest to poprzez ustawienie bitu TOVO w rejestrze TIFR.
        Warunek sprawdza czy do takiego przepełnienia doszło.
        */
        if(TIFR & _BV(TOV0)) { 
            
            /*
            Domyślnie jeżeli doszło do przepełnienia licznika to wartość
            rejestru TCNT0 jest zerowana a liczenie rozpoczyna się od nowa.
            Jednak w tym wypadku jeżeli TOV0 zostanien ustawiony poniższa
            instrukcja spowoduje zatrzymanie timera0 poprzez ustawienie bitów 
            CS02, CS01, CS00 na wartość zero. Tym sposobem licznik zostanie 
            pozbawiony źródła sygnału taktującego.
            */
            TCCR0 &= 0b11111000;
            
            /*
            Reset flagi przepełnienia aby instrukcje w ciele warunku nie
            wykonywała się po usunięciu źródła sygnału taktuającego
            */
            TIFR |= _BV(TOV0);
        }
        
        /*
        Ustawienie aktualnej wartości rejestru TCNT0 na PORTA. W rejestrze
        PORTA znajdzie się liczba określająca ile razy zotał naciśnięty (lub
        puszczony w przypadku ustawienia zeraga na zbocze opadające) przycisk.

        Jeżeli dojedzie do przepełnienia licznik zostanie pozbawiony zegara
        a wartość TCNT0 zostanie zresetowana a więc również wartość PORTA będzie 
        równa zero po wykonaniu poniższej instrukcji.
        */
        PORTA = TCNT0;
    }
}
