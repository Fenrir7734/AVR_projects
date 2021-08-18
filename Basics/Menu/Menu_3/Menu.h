#ifndef __MENU_H__
#define __MENU_H__

#include "LCD.h"
#include "Functions.h"
#include "Keypad.h"

#define UP 4
#define DOWN 8
#define CLEAR 12
#define ENTER 16

/*
Zmienna będąca wskaźnikiem na
aktualnie wybrany element tj. ten
przy którym stoi kursor.
*/
struct MenuItem* currentItem;

/*
Zmienna która określa jakiego typu
jest aktualnie wybrany element. Na
jej podstawie funkcja menu_select()
określa czy po wciśnięciu klawisza
ENTER program przejdzie do wyświetlania
menu niższego w hierarchii czy rozpocznie
wykonywanie wybranego z menu programu.
*/
volatile enum itemType currentType;

/*
Definicja typu wyliczeniowego określająca
charakter elementu menu. Wartość SUBMENU jest
przyporządkowywana elementom które wskazują
na kolejne menu, natomiast wartość PROGRAM
informuje że dany element menu zawiera wskaźnik
na funkcję (program który zostanie wykonany po
wyborze tego elementu z menu).
*/
enum itemType {SUBMENU, PROGRAM};

/*
Struktura na podstawie której będą przedstawiane
kolejne elementy menu. Każde menu jest zorganizowane
w listę dwukierunkową której węzły stanowią kolejne 
elementy tego menu. Ponadto każdy element zawiera 
wskaźnik na element z menu poprzedniego, wskaźnik na 
pierwszy element z menu następnego, wskaźnik na funkcję
zawierającą program do wykonania oraz zmienną wyliczeniową
określającą charakter tego elementu.

name -  tekst wyświetlany na wyświetlaczu LCD opisujący
        dany element
parent -wskaźnik do elementu rodzica z menu wyższego w
        hierarchii. Dzięki niemu możliwy jest powrót do
        poprzedniego menu
child - wskaźnik do pierwszego elementu menu potomnego
        danego elementu. Dzięki niemu możliwe jest przejście
        do menu niższego w hierarchii. Jeżeli elementu 
        zawiera wskaźnik na funkcję i zmienna type
        ma wartość PROGRAM to pole powinno mieć wartość
        NULL
next -  wskaźnik na kolejny element tego samego menu.
prev -  wskaźnik na poprzedni element tego samego menu.
menu_function - wskaźnik na funkcję zawierającą program
        do wykonania. Wartość tego pola powinna być NULL
        jeżeli wartość zmiennej type nie jest PROGRAM
type -  Zmienna określająca czy dany element zawiera 
        wskaźnik na menu niższego poziomu czy wskaźnik
        na funkcję.
*/
struct MenuItem
{
    char* name;
    struct MenuItem* parent;
    struct MenuItem* child;
    struct MenuItem* next;
    struct MenuItem* prev;
    void (*menu_function)(void);
    enum itemType type;
};

    /*
    Deklaracja poszczególnych elementów menu.
    menuX_Y gdzie:
    X - numer menu
    Y - numer elementu menu X
    */
    struct MenuItem menu0_1;
    struct MenuItem menu0_2;
    struct MenuItem menu0_3;
    struct MenuItem menu1_1;
    struct MenuItem menu1_2;
    struct MenuItem menu2_1;
    struct MenuItem menu2_2;
    struct MenuItem menu3_1;
    struct MenuItem menu3_2;
        struct MenuItem menu3_3;

void write_menu();
void menu_cursor_up();
void menu_cursor_down();
void menu_select();
void menu_back();
void menu_control(uint8_t key);
uint8_t menu_check_clear();
void menu_init();

#endif
