#ifndef __MENU_H__
#define __MENU_H__

#include "LCD.h"
#include "Functions.h"

#define UP 4
#define DOWN 8
#define CLEAR 12
#define ENTER 16

struct MenuItem* currentItem;
enum itemType currentType;

enum itemType {SUBMENU, PROGRAM};

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
void menu_init();

#endif
