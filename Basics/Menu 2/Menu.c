#include "Menu.h"

    /*
    Przypisanie poszczególnym elementom menu wartości.
    menuX_Y = {name, parent, child, next, prev, menu_function, type}
    */
    struct MenuItem menu0_1 = {"Menu 1", 0, &menu1_1, &menu0_2, 0, 0, SUBMENU};
    struct MenuItem menu0_2 = {"Menu 2", 0, &menu2_1, &menu0_3, &menu0_1, 0, SUBMENU};
    struct MenuItem menu0_3 = {"Menu 3", 0, &menu3_1, 0, &menu0_2, 0, SUBMENU};
    struct MenuItem menu1_1 = {"M1 Program 1", &menu0_1, 0, &menu1_2, 0, 0, PROGRAM};
    struct MenuItem menu1_2 = {"M1 Program 2", &menu0_1, 0, 0, &menu1_1, function1, PROGRAM};
    struct MenuItem menu2_1 = {"M2 Program 1", &menu0_2, 0, &menu2_2, 0, function3, PROGRAM};
    struct MenuItem menu2_2 = {"M2 Program 2", &menu0_2, 0, 0, &menu2_1, 0, PROGRAM};
    struct MenuItem menu3_1 = {"M3 Program 1", &menu0_3, 0, &menu3_2, 0, function2, PROGRAM};
    struct MenuItem menu3_2 = {"M3 Menu 1", &menu0_3, &menu1_1, 0, &menu3_1, 0, SUBMENU};

/*
Funkcja która wypisuje dwa lub jedne element menu
na wyświetlacz LCD.
*/
void write_menu() {
    /*chil
    Wyczyszczenie zawartości wyświetlacza LCD np.
    po poprzednim menu lub wykonywanym programie
    */
    LCD_clear();

    /*
    Przekazanie do funkcji LCD_write_menu_item(...)
    wybranego elementu i wypisanie go w pierwszej linii
    wyświetlacza 
    */
    LCD_write_menu_item(currentItem->name, 1);

    /*
    Jeżeli wybrany element nie jest ostatnim elementem
    menu to zostanie wypisany kolejny element w drugiej
    linii wyświetlacza.
    */
    if(currentItem->next != 0) {
        LCD_write_menu_item(currentItem->next->name, 2);
    }
}

/*
Funkcja odpowiadająca za przesunięcie kursora o jeden 
element menu w górę oraz przewinięcie menu.
*/
void menu_cursor_up() {

    /*
    Jeżeli aktualnie wybrany element nie ma poprzednika 
    oznacza to że kursor wskazuje na pierwszy element menu
    i nie może zostać przesunięty wyżej. W przeciwnym razie
    zostaną podjęte odpowiednie działania.
    */
    if(currentItem->prev != 0) {
        
        /*
        Zmienna globalna cursor_x z pliku LCD.h przechowuje
        numer linii w którym aktualnie znajduje się kursor. 
        Jeżeli kursor znajduje się w linii drugiej wystarczy 
        przesunąć go o jedną linię wyżej, nie ma potrzeby 
        przewijania menu. W przeciwnym wypadku kursor pozostanie
        a swojej aktualnej pozycji a menu zostanie przewinięte. 
        */
        if(cursor_x == 2) {
            
            /*
            Ustawienie kursora w na pierwszej pozycji linii 1
            */
            LCD_set_cursor(1, 1);
        } else {
            
            /*
            Przewijanie menu. Przewijanie polega na wyczyszczeniu
            całego wyświetlacza LCD a następnie wypisaniu w pierwszej
            linii wyświetlacza elementu poprzedniego oraz elementu 
            aktualnego (który przedtem znajdował się w linii pierwszej) 
            w linii drugiej.
            */
            LCD_clear();
            LCD_write_menu_item(currentItem->prev->name, 1);
            LCD_write_menu_item(currentItem->name, 2);
        }

        /*
        Jeżeli nastąpiło przesunięci kursora w górę lub przewinięcie
        menu, aktualnie wskazywany przez kursor element zmienia się
        na element poprzedni elementu aktualnego.
        */
        currentItem = currentItem->prev;
    }
    _delay_ms(5);
}

/*
Funkcja analogiczna do poprzedniej, odpowiadająca za
ruch kursora w dół oraz przewinięcie menu.
*/
void menu_cursor_down() {

    /*
    Jeżeli aktualny element nie ma następnika oznacza to 
    że kursor wskazuje na ostatni element danego menu i 
    nie może być przesunięty niżej.
    */
    if(currentItem->next != 0) {

        /*
        Jeżeli kursor znajduje się w linii pierwszej 
        wystarczy go przesunąć o jedną linię w dół. W
        przeciwnym wypadku nastąpi przewinięcie menu.
        */
        if(cursor_x == 1) {
            /*
            Ustawienie kursora w na pierwszej pozycji linii 2
            */
            LCD_set_cursor(2, 1);
        } else {
            /*
            Przewinięcie menu. Wyczyszczenie akrualnej zawartości
            wyświetlacza a następnie wypisanie w linie pierwszej
            elementu aktualnego oraz w linii drugiej elementu następnego.
            */
            LCD_clear();
            LCD_write_menu_item(currentItem->name, 1);
            LCD_write_menu_item(currentItem->next->name, 2);
            
            /*
            Jako że funkcja LCD_write_menu_item(...) zawsze ustawia kursor
            na pozycji pierwszej linii 1 w tym wypadku musi nastąpić 
            "ręczne" przestawienie kursora do linii drugiej tak aby znajdował 
            się przy elemencie następnym.
            */ 
            LCD_set_cursor(2, 1);
        }
        
        /*
        Jeżeli nastąpiło przesunięci kursora w dół lub przewinięcie
        menu, aktualnie wskazywany przez kursor element zmienia się
        na element następny elementu aktualnego.
        */
        currentItem = currentItem->next;
    }
    _delay_ms(5);
}

/*
Funkcja odpowiadająca za obsługę klawisza wyboru ENTER
*/
void menu_select() {

    /*
    Jeżeli aktualny element z założenia ma pozwalać na przejście do
    kolejnego menu oraz jeżeli element ten wskazuje na jakiś element 
    typu MenuItem to nasatąpi przejście do tego elementu. 
    Jeżeli natomiast element ma pozwalać na wykonanie jakiegoś programu 
    i ma wskaźnik do funkcji tego programu to nastąpi wykonanie tego programu.
    */
    if(currentItem->type == SUBMENU && currentItem->child != 0) {
        
        /*
        Przejście do kolejnego menu. Aktualny element zmienia się na
        pierwszy element listy tworzącej menu niższe w hierarchi.
        */
        currentItem = currentItem->child;

        /*
        Funkcja odpowiadająca za wypisanie na wyświetlaczu nowego menu.
        */
        write_menu();
    } else if(currentItem->menu_function != 0) {

        /*
        Rozpoczęcie wykonywania programu do którego odwołuje się wskaźnik 
        na funkcję. Przed wywołaniem funkcji programu z wyświetlacza zostaje 
        usunięte menu oraz wartość currentType zmienia się na PROGRAM.
        */
        LCD_clear();
        currentType = PROGRAM;
        currentItem->menu_function();
    }
    _delay_ms(5);
}

/*
Funkcja odpowiadająca za obsługę klawisza powrotu CLEAR.
*/
void menu_back() {

    /*
    Cofnięcie jest możliwe tylko wtedy gdy istnieje jakiś
    rodzic aktualnego elementu tj. jeżeli element jest powiązany
    z jakimś innym elementem znajdującym się wyżej w hierarchi.
    */
    if(currentItem->parent != 0 && currentType == SUBMENU) {
        /*
        Cofnięcie do poprzedniego menu. Aktualny element zmienia się na
        odpowiadający mu element z menu wyższego w hierarchi.
        */
        currentItem = currentItem->parent;
        write_menu();
    } else if(currentItem->parent != 0) {

        /*
        Wyjście z programu. Gdy następuje wejście do programu wartość 
        zmiennej currentItem nie zmienia się. Aktualnie powrót działą w ten
        sposób że kursor pojawia się przy elemencie wskazującym na 
        program z którego właśnie wyszedł. Odkomendowanie poniższej linii
        pozwoliło by na ustawienie kursora przy pierwszym elemencie menu do 
        którego nastąpił powrót.
        */
        //currentItem = (currentItem->parent)->child;
        currentType = SUBMENU;
        write_menu();
    }
}

/*
Główna funkcja odpowiadająca za sterowanie menu.
uint8_t key - odczytany klawisz
*/
void menu_control(uint8_t key) {

    /*
    Ustalenie typu aktualnego elementu. Jeżeli wykonywany jest właśnie 
    program wybrany z menu jedynym działający klawiszem jest CLEAR pozwalający
    na wyjście z tego programu.
    */  			
    if(currentType == SUBMENU) {

        /*
        Wywołaniie odpowiedniej funkcji sterującej menu w zależności od tego
        który klawisz jest wciśnięty. Jeżeli nie jest wciśnięty żaden klawisz 
        odpowiadajacy za sterowanie funkcja nic nie robi.
        */
		switch(key) {
			case UP: menu_cursor_up(); break;
			case DOWN: menu_cursor_down(); break;
			case CLEAR: menu_back(); break;
			case ENTER: menu_select(); break;
			default: break;
		}
	} else {

        /*
        Jeżeli wykonywany jest akrualnie program z menu dostępny jest tylko klawisz
        CLEAR pozwalający na wyjście do menu.
        */
        if(key == CLEAR) {
            menu_back();
        }
    }	
}

/*
Funkcja odpowiadająca za wyjście z programu wykonującego się w pętli. W takich 
programach jest wywoływana zamiast funkcji menu_control(...) ponieważ ta funkcja 
informuje program poprzez zwrócenie odpowiedniej wartości że został przerwany.
*/
uint8_t menu_check_clear() {

    /*
    Sprawdzenie czy nastąpiło przerwanie odmierzające czas między kolejnymi 
    odczytami klawiatury.
    */
    if(state) {
        state = 0;

        /*
        Sprawdzenie czy klawisz CLEAR został wciśnięty, jeżeli tak to następuje
        wypisanie menu a funkcja zwraca wartość 1 w celu poinformowania funkcji 
        wywoływującej o jej przerwaniu.
        */
        if(keypad() == CLEAR) {
            menu_back();
            return 1;
        }
    }

    /*
    Jeżeli nie nastąpił odczyt lub nie był wciśnięty klawisz CLEAR funkcja
    zwraca wartość 0.
    */
    return 0;
}

/*
Funkcja wywoływana w programie raz po wszystkich innych inicjalizacjach 
(klawiatury, przerwań i LCD), odpowiadająca za inicjalizacje menu.
*/
void menu_init() {

    /*
    Wyczyszcenie wyświetlacza, wypisanie dwóch pierwszych elementów najwyższego
    w hierarchii menu, ustawienie kursora przy pierwszym elemencie
    oraz ustawienie odpowiednich wartości zmiennych currentItem oraz 
    currentType.
    */
    LCD_clear();
    LCD_write_menu_item(menu0_1.name, 1);
    LCD_write_menu_item(menu0_2.name, 2);
    LCD_set_cursor(1, 1);
    currentItem = &menu0_1;
    currentType = SUBMENU;
}

