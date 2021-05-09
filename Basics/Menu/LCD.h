#ifndef __LCD_H__
#define __LCD_H__

#include <avr/io.h>			
#include <util/delay.h>	

#define LCD_DDR DDRB			
#define LCD_PORT PORTB			
#define LCD_RS 1				
#define LCD_EN 0 

uint8_t cursor_x;
uint8_t cursor_y;

void pulse();
void LCD_write_cmd(uint8_t cmd);
void LCD_write_data(uint8_t cmd);
void LCD_init();
void LCD_clear();
void LCD_set_cursor(uint8_t x, uint8_t y);
void LCD_clear_y(uint8_t y);
void LCD_write(char *str);
void LCD_write_menu_item(char *str, uint8_t line);

char* toCharArr(char* arr, uint8_t n, uint8_t num);

#endif
