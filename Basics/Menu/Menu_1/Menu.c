#include "Menu.h"

    struct MenuItem menu0_1 = {"Menu 1", 0, &menu1_1, &menu0_2, 0, 0, SUBMENU};
    struct MenuItem menu0_2 = {"Menu 2", 0, &menu2_1, &menu0_3, &menu0_1, 0, SUBMENU};
    struct MenuItem menu0_3 = {"Menu 3", 0, &menu3_1, 0, &menu0_2, 0, SUBMENU};
    struct MenuItem menu1_1 = {"M1 Program 1", &menu0_1, 0, &menu1_2, 0, function1, PROGRAM};
    struct MenuItem menu1_2 = {"M1 Program 2", &menu0_1, 0, 0, &menu1_1, function2, PROGRAM};
    struct MenuItem menu2_1 = {"M2 Program 1", &menu0_1, 0, &menu2_2, 0, 0, PROGRAM};
    struct MenuItem menu2_2 = {"M2 Program 2", &menu0_1, 0, 0, &menu2_1, 0, PROGRAM};
    struct MenuItem menu3_1 = {"M3 Program 1", &menu0_1, 0, &menu3_2, 0, 0, PROGRAM};
    struct MenuItem menu3_2 = {"M3 Menu Glowne", &menu0_1, &menu0_1, &menu3_3, &menu3_1, 0, SUBMENU};
    struct MenuItem menu3_3 = {"M3 Menu 1", &menu0_1, &menu1_1, 0, &menu3_2, 0, SUBMENU};

void write_menu() {
    LCD_clear();
    LCD_write_menu_item(currentItem->name, 1);

    if(currentItem->next != 0) {
        LCD_write_menu_item(currentItem->next->name, 2);
    }
}

void menu_cursor_up() {
    if(currentItem->prev != 0) {
        if(cursor_x == 2) {
            LCD_set_cursor(1, 1);
        } else {
            LCD_clear();
            LCD_write_menu_item(currentItem->prev->name, 1);
            LCD_write_menu_item(currentItem->name, 2);
        }
        currentItem = currentItem->prev;
    }
    _delay_ms(50);
}

void menu_cursor_down() {
    if(currentItem->next != 0) {
        if(cursor_x == 1) {
            LCD_set_cursor(2, 1);
        } else {
            LCD_clear();
            LCD_write_menu_item(currentItem->name, 1);
            LCD_write_menu_item(currentItem->next->name, 2);
            LCD_set_cursor(2, 1);
        }
        currentItem = currentItem->next;
    }
    _delay_ms(50);
}

void menu_select() {
    if(currentItem->type == SUBMENU && currentItem->child != 0) {
        currentItem = currentItem->child;
        write_menu();
    } else if(currentItem->menu_function != 0) {
        LCD_clear();
        currentType = PROGRAM;
        currentItem->menu_function();
    }
    _delay_ms(50);
}

void menu_back() {
    if(currentItem->parent != 0) {
        currentItem = currentItem->parent;
        write_menu();
    }
}

void menu_control(uint8_t key) {  			
    if(key != 0 && currentType == SUBMENU) {
        switch(key) {
            case UP: menu_cursor_up(); break;
            case DOWN: menu_cursor_down(); break;
            case CLEAR: menu_back(); break;
            case ENTER: menu_select(); break;
            default: break;
        }
    } else {
        if(key == CLEAR) {
            currentItem = (currentItem->parent)->child;
            currentType = SUBMENU;
            write_menu();
        }
    }	
}

void menu_init() {
    LCD_clear();
    LCD_write_menu_item(menu0_1.name, 1);
    LCD_write_menu_item(menu0_2.name, 2);
    LCD_set_cursor(1, 1);
    currentItem = &menu0_1;
}

