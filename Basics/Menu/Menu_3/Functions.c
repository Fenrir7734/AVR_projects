#include "Functions.h"

/*
Zmienna globalna która będzie służyć
do zliczania liczby wykonań procedury
obsługi przerwania.od Timera2.
*/
volatile uint8_t counter2 = 0;

/*
Procedura obsługi przerwania która odpowiada
za przerwanie Compare Match w Timer2. Procedura ta
zostanie wykonana jeżeli w rejestrze TIMSK 
została ustawiona flaga OCIE2 oraz w rejestrze
TIFR flaga OCF2.
*/
ISR(TIMER2_COMP_vect) {
    
    /*
    Jeżeli wystąpiło przerwanie zmienna jest 
    inkrementowana
    */
    counter2++;
}

/*
Funkcja odczytująca klawisze z pierwszej kolumny 
klawiatury i wypisujące ich numery na wyświetlaczu
LCD.
Funkcja dostępna z poziomu:
Menu 1 -> M1 Program 1
*/
void key_read() {
    
    /*
    Zmienna która będzie przechowywać odczytany
    przycisk
    */
    uint8_t key;

    /*
    Pętla nieskończona w której będą oczytywane 
    klawisze a następnie wypisywane na wyświetlaczu
    */
    while(1) {
        
        /*
        Jeżeli zmienna state jest ustawiona oznacza
        to że nastąpiło przerwanie od Timer0 a więc
        minęło 10ms od poprzedniego przerwania. W 
        takim wypadku nastąpi odczytanie klawiatury.
        Jeżeli zmienna state nie jest ustawiona funkcja
        nie będzie robić nic poza oczekiwaniem w pętli
        na ustawienie tej zmiennej.
        */
        if(state) {

            /*
            Odczyt klawiatury.
            */
            key = keypad();

            /*
            Sprawdzenie czy któryś z przycisków z 1 kolumny jest
            wciśnięty. Jeżeli tak to nastąpi ich wypisanie.
            */
            if(key == 1 || key == 5 || key == 9 || key == 13) {

                /*
                Zadeklarowanie tablicy która będzie przechowywać
                numer wciśniętego klawisza w postaci znaków char.
                Następnie zostaje wywołana funkcja odpowiadająca
                za zmianę wartości zmiennej key na tablicę charów
                i zapisanie ich do przekazanej w wywołaniu tablicy.
                */
                char arr[3];
                toCharArr(arr, 3, key);

                /*
                Czyszczenie ekranu w celu usunięcia poprzednio wypisanych
                znaków a następnie wypisanie numeru wciśniętego klawisza
                */
                LCD_clear();
                LCD_write(arr);
                
                /*
                Wyzerowanie zmiennej odpowiadającej za informowanie o 
                przerwaniu
                */
                state = 0;

                /*
                Jeżeli nie został odczytany żaden klawisz z pierwszej kolumny
                następuje sprawdzenie czy wciśnięty klawisz (o ile jakiś klawisz
                był wciśnięty) nie był klawiszem odpowiadającym za wyjście z 
                funkcji. Jeżeli tak to funkcja menu_check_clear() wypisuje na 
                wyświetlaczu LCD menu i zwraca wartość true co skutkuje 
                przerwaniem wykonywania pętli nieskończonej oraz zakończeniem
                funkcji. Jeżeli klawisz CLEAR nie był naciśnięty program przechodzi
                do kolejnego obrotu pętli. 
                Funkcja menu_check_clear ponownie sprawdza wartość zmiennej state 
                oraz odczytuje klawiaturę. Jeżeli zmienna state była ustawiona to 
                zostaje wyzerowana wewnątrz tej funkcji.
                */
            } else if(menu_check_clear()) break;
        }
    }
}

/*
Funkcja odpowiadająca za cykliczne wyświetlanie
diod. Jest to zadanie 3 z ćwiczenia 1.
Funkcja jest dostępna z poziomu:
Menu 3 -> M3 Program 1
*/
void led_blink() {

    /*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu D jako portu wyjściowego
    0xFF = 0b11111111
    */
    DDRD = 0xFF;

    /*
    Ustawienie stanu początkowego portu D
    Ustawienie wartości 1 na pinie 7 portu D.
    */
    PORTD |= _BV(7);

    /*
    Zmienna wskazująca na kolejny bit którego wartość ma
    być ustawiona na wartość 1.
    */
    int8_t i = 6;
    
    while(1) {
        
        /*
        Opóźnienie 100ms
        */
        _delay_ms(100);

        /*
        Ustawienie wartości 1 na pinie o numerze i portu D
        przy wykorzystaniu sumy bitowej.
        */
        PORTD |= _BV(i);

        /*
        Zmniejszenie wartości zmiennej i tak aby wskazywała na 
        pin na którym ma być ustawiona wartość 1 w następnej 
        iteracji pętli.
        */
        i--;
    
        /*
        Sprawdzenie czy wartość na pinie o numerze 0 portu D
        jest równa 1. Jeżeli warunek jest spełniony wartości 
        na pinach portu przywracane są do stanu początkowego
        (z stanem wysokim jedynie na pinie o numerze 7) oraz 
        odpowiednio zmieniana jest wartość zmiennej i.
        */
        if(PORTD & _BV(0)) {
            _delay_ms(100);

            /*
            Przywrócenie stanu początkowego portu D poprzez
            wyzerowanie wszystkich bitów oprócz bitu o nr.7.
            który na porcie D zawsze znajduje się w stanie 
            wysokim.
            */
            PORTD &= _BV(7);

            /*
            Ustawienie wartości zmiennej tak aby wskazywała
            na następny bit który ma być ustawiony w stan wysoki.
            Skoro port D wrócił do stanu początkowego z pinem nr.
            7 w stanie wysokim to kolejny pin który ma przejść w 
            stan wysoki ma nr.6
            */
            i = 6;
        }

        /*
        Funkcja odczytuje klawiaturę jeżeli wcześniej nastąpiło 
        przerwanie od Timer1 i sprawdza czy został naciśnięty 
        klawisz odpowiadający za wyjście z funkcji. Jeżeli tak
        to nieskończona pętla zostanie przerwana a program wychodzi
        do menu z którego została wywołana funkcja.
        */
        if(menu_check_clear()) break;
    }

}

/*
Funkcja odpowiadająca za sterowanie diodą nr 0 dołączoną do portu C.
Dioda jest włączona i wyłączona co 0,5 sekundy. 
Funkcja jest dostępna z poziomu:
Menu 2 -> M2 Program 1
*/
void one_led_blink() {

    /*
    Ustawienie kierunku przesyłania danych. 
    Ustawienie całego portu C jako portu wyjściowego
    0xFF = 0b11111111
    */
    DDRC = 0xFF;

    /*
    Ustawienie stanu początkowego portu C
    Wyzerowanie całego portu C.
    */
    PORTC = 0x00;

    /*
    Ustawienie Timera 2 w tryb CTC oraz źródła sygnału 
    taktującego na sygnał wewnętrzny wraz z preskalerem
    1024.
    */
    TCCR2 |= _BV(WGM21) | _BV(CS20) | _BV(CS21) | _BV(CS22); 

    /*
    Uaktywnienie przerwań w timer2. Ustawienie generowania
    przerwania na Compare Match.
    */
    TIMSK |= _BV(OCIE2);

    /*
    Ustawienie wartości rejestru OCR2.
        
    1MHz/1024 = 976.6Hz
    1/976.6Hz = 1.024ms
    100ms/1.024ms = 98
    98 - 1 = 97 	<- ponieważ liczymy od 0
    */
    OCR2 = 97; 

    /*
    Wyzerowanie zmiennej globalnej
    */
    counter2 = 0;

    while(1) {

        /*
        Jeżeli wartość counter2 jest równa 5 
        oznacza to że nastąpiło 5 przerwań od
        Timera2 a więc minęło 500ms. W takim
        wypadku wartość bitu nr 0 w PORTC zostaje
        zanegowana a następnie zmienna counter2 
        wyzerowania w celu rozpoczęcia kolejnego
        odliczania 500ms.
        */
        if(counter2 == 5) {
            PORTC ^= _BV(0);
            counter2 = 0;
        }

        /*
        Sprawdzenie czy przycisk odpowiadający za cofanie 
        jest naciśnięty. Jeżeli tak następuje wyjście do menu.
        */
        if(menu_check_clear()) {

            /*
            Wyzerowanie ustawionych na początku tej funkcji bitów w
            rejestrze TCCR2 oraz TIMSK. Timer2 zostaje zatrzymany oraz
            flaga zezwalająca na przerwania Compare Match od Timer2 
            jest zerowana.
            */
            TCCR2 &= ~(_BV(WGM21) | _BV(CS20) | _BV(CS21) | _BV(CS22));
            TIMSK &= ~_BV(OCIE2);
            break;
        }
    }
}

/*
Funkcja pomocnicza stopera. Odpowiada za sterowanie stoperem.
*/
uint8_t stopwatch_controls(uint8_t current_mode) {

    /*
    Jeżeli zmienna state jest ustawiona następuje 
    odczyt klawiatury i ustalenie aktualnego trybu
    pracy stopera.
    */
    if(state) {
        state = 0;
        
        /*
        Odczyt klawiatury
        */
        uint8_t key = keypad();

        /*
        Oczekiwanie na puszczenie wciśniętego klawisza.
        */
        while(keypad() != 0)

        /*
        Jeżeli wartość key jest równa 0 oznacza to że żaden
        klawisz nie został wciśnięty a więc stan stopera się nie 
        zmieni.
        */
        if(key == 0) {
            return current_mode;
        }

        /*
        Naciśnięcie klawisza CLEAR powoduje wypisanie menu oraz 
        zwrócenie wartości 4 odpowiadającej trybowi EXIT. Funkcja
        stopwatch() zakończy działanie i program powróci do menu.
        */
        if(key == CLEAR) {
            menu_back();
            return 4;
        }

        /*
        Jeżeli naciśnięty jest klawisz start to następuje 
        sprawdzenie w jakim trybie aktualnie znajduje się stoper. 
        */
        if(key == START) {

            /*
            Ustawienie kursora w pozycji początkowej i usunięcie
            tekstu w całym pierwszym wierszu.
            */
            LCD_set_cursor(1, 1);
            LCD_clear_y(1);

            /*
            Zmiana tryby pracy na przeciwny i wypisanie odpowiedniego
            napisu w pierwszym wierszu.
            */
            if(current_mode == 2) {
                LCD_write("STOP");
                return 1;
            } else {
                LCD_write("START");
                return 2;
            }
        } 
        
        /*
        Jeżeli naciśnięty był kalwisz odpowiadający za reset stopera
        to następuje zwrócenie wartości odpowiadającej trybowi RESET.
        */
        if(key == RESET) {
            return 3;
        }
    }

    /*
    Jeżeli zmienna state nie była ustawiona tryb pracy stopera nie 
    zmienia się.
    */
    return current_mode;
}

/*
Funkcja pomocnicza stopera, wypisująca na 
wyświetlaczu sekund.
*/
void stopwatch_write_seconds(uint8_t seconds) {

    /*
    Ustawienie kursora w wierszu 2 kolumnie 4.
    */
    LCD_set_cursor(2, 4);

    /*
    Zmiana wartości zmiennej seconds na odpowiadający
    mu tablicę znaków.
    */
    char seconds_arr[3];
    toCharArr(seconds_arr, 3, seconds);
    
    /*
    Jeżeli upłynęło mniej niż 10 sekund dopisywane
    jest 0 przed znakiem oznaczającym cyfrę sekund.
    */
    if(seconds < 10) {

        /*
        Przesunięcie o jeden indeks wartości przedstawiającej
        cyfrę sekund a następnie wpisanie na pierwsze miejsce
        znaku '0'
        */
        seconds_arr[1] = seconds_arr[0];
        seconds_arr[0] = '0';
    }
    
    /*
    Wypisanie na ekranie sekund. Wartość minut nie ulega zmianie.
    */
    LCD_write(seconds_arr);
}

/*
Funkcja pomocnicza stopera, wypisująca na 
wyświetlaczu minuty. Funkcja analogiczna do poprzedniej.
*/
void stopwatch_write_minutes(uint8_t minutes) {
    
    /*
    Ustawienie kursora w wierszu 2 kolumnie 1.
    */
    LCD_set_cursor(2, 1);

    char minutes_arr[3];
    toCharArr(minutes_arr, 3, minutes);

    if(minutes < 10) {
        minutes_arr[1] = minutes_arr[0];
        minutes_arr[0] = '0';
    }
    
    /*
    Wypisanie na ekranie minut. Wartość sekund nie ulega zmianie.
    */
    LCD_write(minutes_arr);
}

/*
Główna funkcja odpowiadająca za obsługę stopera
Funkcja dostępna z poziomu:
Menu 2 -> M2 Program 2
*/
void stopwatch() {

    /*
    Ustawienie Timera 2 w tryb CTC oraz źródła sygnału 
    taktującego na sygnał wewnętrzny wraz z preskalerem
    1024.
    */
    TCCR2 |= _BV(WGM21) | _BV(CS20) | _BV(CS21) | _BV(CS22);

    /*
    Uaktywnienie przerwań w timer2. Ustawienie generowania
    przerwania na Compare Match.
    */
    TIMSK |= _BV(OCIE2);

    /*
    Ustawienie wartości rejestru OCR2.
        
    1MHz/1024 = 976.6Hz
    1/976.6Hz = 1.024ms
    100ms/1.024ms = 98
    98 - 1 = 97 	<- ponieważ liczymy od 0
    */
    OCR2 = 97;

    /*
    Deklaracja zmiennych które będą przechowywały
    odpowiednio minuty i sekundy jakie minęły od 
    uruchomienia stopera.
    */
    uint8_t minutes = 0;
    uint8_t seconds = 0;

    /*
    Zmienna określająca w jakim trybie aktualnie 
    znajduje się stoper. Tryby to:
    STOP = 1, START = 2, RESET = 3, EXIT = 4
    */
    uint8_t mode = 1;

    /*
    Wyzerowanie zmiennej globalnej zliczającej
    ilość przerwań
    */
    counter2 = 0;

    /*
    Wypisanie na wyświetlaczu tekstu:
    STOP
    00:00
    Pierwszy wiersz wyświetlacza będzie zawierał
    napis informujący czy stoper odmierza aktualnie 
    czas czy jest zatrzymany. Drugi wiersz to 
    odmierzony czas w formacie MM:SS
    */
    LCD_write("STOP");
    LCD_set_cursor(2, 1);
    LCD_write("00:00");

    while(1) {

        /*
        Kolejno następuje sprawdzenie czy stoper jest w trybie:
        STOP, START, RESET, EXIT
        */
        if(mode == 1) {
            /*
            Pętla która będzie wykonywana do momentu aż tryb pracy
            stopera nie zostanie zmieniony na inny niż STOP.
            */
            while(mode != 1) {
                
                /*
                Odczytywanie aktualnego trybu stopera.
                */
                mode = stopwatch_controls(mode);
            }
        } else if(mode == 2) {

            /*
            Tryb START
            Jeżeli wartość counter2 jest większa od 10 oznacza to
            że wystąpiło co najmniej 10 przerwań od timer2 a więć 
            minęła co najmniej 1 sekunda.
            */
            if(counter2 > 10) {

                /*
                Wartość licznika jest zerowana aby rozpocząć zliczanie
                kolejnej sekundy.
                */
                counter2 = 0;

                /*
                Zmienna przechowująca sekundy jest inkrementowana.
                */
                seconds++;

                /*
                Sprawdzenie czy minęła minuta. Jeżeli tak zmienna 
                odpowiadająca minutom jest inkrementowana, sekundy
                są zerowane oraz aktualizowane są minuty na wyświetlaczu.
                */
                if(seconds > 59) {
                    minutes++;
                    seconds = 0;
                    stopwatch_write_minutes(minutes);
                }
                
                /*
                Aktualizacja sekund na wyświetlaczu LCD.
                */
                stopwatch_write_seconds(seconds);
            }
        }else if(mode == 3) {

            /*
            Tryb RESET
            Zmiana napisu w pierwszym wierszu na STOP, wyzerowanie
            czasu na wyświetlaczu i wyzerowanie obu zmiennych 
            odpowiadających za zliczanie tego czasu, wyzerowanie
            zmiennej globalnej i zmiana trybu stopera na STOP.
            */
            LCD_set_cursor(1, 1);
            LCD_write("STOP");
            LCD_set_cursor(2, 1);
            LCD_clear_y(1);
            LCD_write("00:00");
            minutes = 0;
            seconds = 0;
            counter2 = 0;
            mode = 1;
        }else if(mode == 4) {

            /*
            Tryb EXIT
            Wyzerowanie ustawionych na początku tej funkcji bitów w
            rejestrze TCCR2 oraz TIMSK. Timer2 zostaje zatrzymany oraz
            flaga zezwalająca na przerwania Compare Match od Timer2 
            jest zerowana.
            */
            TCCR2 &= ~(_BV(WGM21) | _BV(CS20) | _BV(CS21) | _BV(CS22));
            TIMSK &= ~_BV(OCIE2);
            break;
        }

        /*
        Wywołanie funkcji sprawdzające czy zmienił się tryb pracy stopera
        i jeżeli tak to zwracającej kod tego trybu.
        */
        mode = stopwatch_controls(mode);
    }
}
