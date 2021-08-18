#include <avr/io.h>			//biblioteka do obsługi wejścia/wyjścia
#include <avr/interrupt.h> 	//biblioteka do obsługi przerwań


/*
Procedura obsługi przerwań z wektorem 
przerwania INT0_vect który odpowiada za
zewnętrzne żądanie przerwania dla linii
INT0. Jeżeli zajdzie przerwanie procedura
ta neguje wartość PA0. 

ISR jest specjalną makrodefinicją 
przyjmującą dwa parametry. Jednym z nich
jest wektor przerwania określający jakiego
typu przerwania dotyczy dana procedura, 
natomiast drugi parametr definjuje sposób
w jaki będzie zachowywac się ta procedura. 
Domyślnie, bez potrzeby podawania tego parametru
makro ISR przychodzi z parametrem ISR_BLOCK 
który blokuje przerwania do czasu wykoniania 
procedury. Jeżeli chcemy zmieninć to zachowanine
należy jawnie podać odpowiedni parametry np.
ISR_NOBLOCK który to powoduje, że procedura 
przerwania będzie mogła być przerwana przez
inne przerwanie.
Procedura obsługi przerwań powinna być jak
najkrótsza oraz nie powinna zawierać funkcji
opóźniających. Wynika to z tego że jeżeli 
obsługa przerwań w czasie wykonywania procedury
jest zablokowana (ISR_BLOCK) to możemy nie 
obsłużyć w porę przychodzących nowych przerwań.
*/
ISR(INT0_vect) {
    PORTA ^= _BV(PA0);
}


/*
Procedura obsługi przerwań z wektorem 
przerwania INT2_vect który odpowiada za
zewnętrzne żądanie przerwania dla linii
INT2 oraz domyślnym parametrem ISR_BLOCK.
Jeżeli zajdzie przerwanie przerwanie procedura
ta neguje wartość PA2.
*/
ISR(INT2_vect) {
    PORTA ^= _BV(PA2);
}


int main(void) {

    /*
    Przerwanie jest to zdarzenie które powoduje
    zmianę kolejności wykonywania programu. Jeżeli 
    nastąpi przerwanie program wstrzymuje aktualnie 
    wykonywane zadanie i przechodzi do odpowiedniej
    procedury obsługi przerwań. Aby móc obsłużyć 
    przerwanie należy:
    - napisać procedurę obsługi przerwania
    - ustawić flagę zezwolenia przerwania dla odpowiednich linii
    - ustawić globalną flagę zezwolenia na przerwanie
    */

    /*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu A jako portu wyjściowego
    0xFF = 0b11111111
    */	
    DDRA = 0xFF;
    
    /*
    Ustawienie wyzwalania przerwania na INT0 
    poprzez zbocze narastając. 

    Rejestr MCUCR (rejestr kontroli MCU) służy
    do ustawienia sposobu wyzwalania przerwania
    na INT0 oraz INT1. Flagi kontroli przerwań 
    ISC11 oraz ISC10 odpowiadają za INT1, natomiast
    ISC01 oraz ISC00 odpowiadają za INT0. Domyślnie
    flagi te są wyzerowane a więc INT0 oraz INT1 
    domyslnie są wyzwalane poziomem niskim.
    */
    MCUCR |= _BV(ISC01) | _BV(ISC00); 

    /*
    Ustawienei wyzwalania przerwania na INT2
    poprzez zbocze narastające

    Rejester MCUCSR (rejestr kontroli i statusu MCU)
    służy do ustawiania sposobu wyzwalania przerwania
    na INT2. Odpowiada za to tylko jedna flaga ISC2 a 
    więc na INT2 można ustawić tylko dwa tryby:
    - wyzwalanie zboczem opadającym. Ten tryb jest domyślny.
    - wyzwalanie zboczem narastającym.
    */
    MCUCSR |= _BV(ISC2); 
    
    /*
    Ustawienie wyzwalania przerwań na INT0 oraz INT2
    poprzez zbocze opadające.
    */
    /*
        MCUCR |= _BV(ISC01);
        MCUCSR &= ~_BV(ISC2); 			
    */


    /*
    Uaktywnienie przerwań INT0 oraz INT2

    Rejestr GICR służy do uaktywnienia zewnętrznych
    przerwań INT1, INT0 oraz INT2. Aby uaktywnić np.
    przerwanie INT1 wystarczy ustawić w tym rejestrze
    flagę INT0.

    INT1 nasłuchuje na PD3
    INT0 nasłuchuje na PD2
    INT2 nasłuchuje na PB2

    Żądanie przerwania może być zgłoszone niezależnie
    czy dany pin jest ustawiony jako wejście czy wyjście
    */		
    GICR |= _BV(INT0) | _BV(INT2);
            
    /*
    Ustawienie flag przerwania dla INT0 oraz INT2. 
    Poniższa operacja spowoduje natychmiastowe 
    przejście do procedury obsługi przerwania gdy
    obsługa przerwań zostanie już aktywowana.

    W rejestrze tym mogą być ustawiane flagi INTF0,
    INTF1 oraz INTF2 które oznaczają że wystąpiło 
    żądanie przerwania zewnętrznego na odpowiadającej 
    im linii. Gdy któraś z tych flag jest ustawiona 
    oraz istnieje procedura obsługi przerwań z 
    odpowiednim wektorem przerwania, program przechodzi 
    do jej wykonanina. Następuje wtedy wyzerowanie 
    znacznika INTFx a więc nie musimy robić tego manualnie
    */
    GIFR |= _BV(INTF0) | _BV(INTF2);
    
    /*
    Funkcja ta odblokowuje obsługę przerwań poprzez 
    ustawienie flagi I w rejestrze SREG. Funkcja ta
    powinna być wywołana tylko raz, na poczatku programu 
    */
    sei(); 								

    while(1) {}
}
