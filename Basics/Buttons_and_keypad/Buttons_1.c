#include <avr/io.h> 		//biblioteka do obsługi wejścia/wyjścia
#include <util/delay.h>		//biblioteka zawierająca funkcje opóźniające

//Główna funkcja programu
int main(void)
{   
    /*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu A jako portu wejściowego
    0x00 = 0b00000000
    */
    DDRA = 0x00;

    /*
    Ustawienie całego portu B jako portu wyjściowego
    */
    DDRB = 0xFF;

    /*
    Ustawienie stanu początkowego portu B,
    wszystkie piny będą w stanie niskim.
    */   
    PORTB = 0x00;

    /*
    Zmienna która posłuży do przechowywania 
    aktualnego stanu PINA
    */
    uint8_t pinCurrentState;
    
    /*
    Główna pętla programu, pętla nieskończona.
    */
    while(1)
    {
        /*
        Odczyt i zapamiętanie aktualnego stanu portu A 
        przy czym 4 młodsze bity są zerowane z wykorzystaniem
        operacji logicznej AND i maski 0xF0 = 0b11110000
        Te 4 bity są zerowane ponieważ nie powinny być
        wykorzystywane w programie. Przyciski
        są podłączone tylko do pinów PORTA o numerach od 
        7 do 4.  
        */
        pinCurrentState = PINA & 0xF0;

        /*
        Ustawia odpowiednią wartość w rejestrze PORTB 
        w zależności od wciśniętego przycisku. Jeżeli 
        żaden przycisk nie jest wciśnięty to wartość
        rejestru PORTB jest zerowany.
        */
        switch(pinCurrentState) {

            /*
            Jeżeli przycisk nr. 1 jest wciśnięty to 
            wpisz do rejestru PORTB wartość 1
            */
            case 0b00010000: PORTB = 1; break;

            /*
            Jeżeli przycisk nr. 2 jest wciśnięty to 
            wpisz do rejestru PORTB wartość 2
            */	
            case 0b00100000: PORTB = 2; break;

            /*
            Jeżeli przycisk nr. 3 jest wciśnięty to 
            wpisz do rejestru PORTB wartość 4
            */
            case 0b01000000: PORTB = 4; break; 

            /*
            Jeżeli przycisk nr. 4 jest wciśnięty to 
            wpisz do rejestru PORTB wartość 8
            */
            case 0b10000000: PORTB = 8; break;	
            
            /*
            Jeżeli żaden przycisk nie jest 
            wciśnięty wyzeruj rejestr PORTB
            */
            default: PORTB = 0;					
        }
    }
}
