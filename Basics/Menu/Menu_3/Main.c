#include "Menu.h"
#include "keypad.h"

int main()
{
    /*
    Inicjalizacja portów do których jest podłączony
    keypad. Keypad będzie służył do sterowania menu
    oraz programami
    */
    init_keypad();

    /*
    Inicjalizacja timera oraz przerwań wyznaczających
    moment odczytu keypada
    */
    init_interrupts();

    /*
    Inicjalizacja wyświetlacza LCD i wyczyszczenie jego
    poprzedniej zawartości. Wyświetlacz będzie służył 
    do wyświetlania menu.
    */
    LCD_init();			
    LCD_clear();

    /*
    Inicjalizacja menu. Powoduje wyświetlenie na 
    wyświetlaczu 2 pierwszych pozycji najwyższego
    w hierarchii menu.
    */
    menu_init();

    while(1) {

        /*
        Jeżeli zmienna globalna state z pliku keypad.h
        jest ustawiona nastąpi odczyt klawiatury i 
        sprawdzenie czy był naciśnięty jakiś klawisz oraz
        czy wciśnięty klawisz służy do sterowania menu.
        Zmienna state jest ustawiana w przerwaniu.
        */
        if(state) {
            state = 0;

            /*
            Odczyt klawiatury.
            */
            uint8_t key = keypad();
            
            /*
            Sprawdzenie czy został naciśnięty jakiś klawisz. 
            Jeżeli tak, nastąpi przekazanie tej wartości do
            funkcji odpowiadającej za sterowanie menu.
            */
            if(key != 0) {

                /*
                Oczekiwanie aż żaden klawisz nie będzie wciśnięty,
                tak aby zapobiec wielokrotnemu wykonaniu tej samej
                akcji w przypadku zbyt długiego przytrzymania klawisza.
                */
                while(keypad() != 0);

                /*
                Funkcja ta sprawdzi czy wciśnięty klawisz służy 
                do sterowania menu i podejmie odpowiednie działania.
                */
                menu_control(key);
            }
        }
    }
}
