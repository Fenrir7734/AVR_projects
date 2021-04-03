#include <avr/io.h>     //biblioteka do obsługi wejścia/wyjścia
#include <util/delay.h> //biblioteka zawierająca funkcje opóźniające
 
//Główna funkcja programu
int main(void){
 
    /*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu A jako portu wyjściowego
    0xFF = 0b11111111
    */
    DDRA = 0xFF;
    
    /*
    Ustawienie stanu początkowego portu A z 2 ostatnimi 
    bitami ustawionymi na wartość 1
    */          
    PORTA = 0b11000000;                     
    
    /*
    Główna pętla programu, pętla nieskończona.
    */
    while(1) { 
        
        /*
        Pętla zmieniająca wartość bitów w kierunku od bitu nr.7 do bitu nr.0 (malejąco).
        Zmienna i zawsze wskazuje na kolejny bit który ma być ustawiony na wartość 1;
        */
        for(int8_t i = 5; i >= 0; i--) {
            
            /*
            Opóźnienie wynoszące 50ms
            */
            _delay_ms(50);
 
            /*
            Ustawienie wartości 1 na pinie i oraz wartości 0 na pinie i+2 portu A
            Za pomocą sumy bitowej bity o numerach i oraz i+2 ustawiane są na wartość 1,
            następnie na wyniku sumy bitowej i wartości z portu A wykonywana jest operacja 
            alternatywy wykluczającej (XOR). Operacja ta prowadzi do zanegowania bitów 
            portu A o numerach i oraz i+2 jednocześnie pozostawiając reszte bitów tego 
            portu bez zmian.
            Zapis ten jest równoważny zpisowi:
            PORTA = PORTA ^ (_BV(i) | _BV(i + 2));
            */
            PORTA ^= _BV(i) | _BV(i + 2);
        }
        
        /*
        Pętla zmieniająca wartość bitów w kierunku od bitu nr.0 do bitu nr.7 (rosnąco).
        Zmienna i zawsze wskazuje na kolejny bit który ma być ustawiony na wartość 1;
        */
        for(int8_t i = 2; i <= 7; i++) {
            
            /*
            Opóźnienie wynoszące 100ms
            */
            _delay_ms(100);
 
            /*
            Operacja analogiczna do tej z poprzedniej pętli lecz wykonywana dla bitów
            i oraz i-2.
            */
            PORTA ^= _BV(i) | _BV(i - 2);
        }
    }
 
}
