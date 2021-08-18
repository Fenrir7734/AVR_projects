#include "Functions.h"

/*
Funkcja wypisująca napis na wyświetlaczu. 
Po wypisaniu kończy swoje działanie więc 
naciśnięcie przycisku CLEAR będzie obsłużone
przez funkcję menu_control().
Funkcja dostepna z poziomu:
Menu 1 -> M1 Program 2
*/
void function1() {
    LCD_write("Funkcja 1");
}

/*
Funkcja powodująca miganie diody LED
dołączonej do PORTD. Sterowanie PORTD 
odbywa się w pętli nieskończonej a więc 
wykrycie naciśnięcia i obsłużenie klawisza
CLEAR będzie wykonane przez funkcję 
menu_check_clear().
Funkcja dostępna z poziomu: 
Menu 3 -> M3 Program 1
*/
void function2() {
    DDRD = 0xFF;
    PORTD = 0x00;

    while(1) {
        PORTD ^= 0x01;
        _delay_ms(100);

        /**/
        if(menu_check_clear()) break;
    }
}

/*
Funkcja analogiczna do function1(), dostępna
z poziomu:
Menu 2 -> M2 Program 1
*/
void function3() {
    LCD_write("Funkcja 3");
}


