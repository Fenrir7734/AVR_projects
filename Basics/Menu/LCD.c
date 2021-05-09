#include "LCD.h"

uint8_t cursor_x = 0;
uint8_t cursor_y = 0;

void pulse() {
	LCD_PORT |= (1<<LCD_EN);		
	_delay_us(1);
	LCD_PORT &= ~ (1<<LCD_EN);
	_delay_us(100);
}

void LCD_write_cmd(uint8_t cmd) {
	LCD_PORT &= ~_BV(LCD_RS);

	LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0); 
	pulse();

	LCD_PORT = (LCD_PORT & 0x0F) | ((cmd & 0x0F) << 4);  
	pulse();	

	_delay_ms(2);
}

void LCD_write_data(uint8_t cmd) {
	LCD_PORT |= _BV(LCD_RS);

	LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0); 
	pulse();

	LCD_PORT = (LCD_PORT & 0x0F) | ((cmd & 0x0F) << 4);
	pulse();	

	_delay_ms(2);
}

void LCD_init() {
	LCD_DDR = 0xF0 | _BV(LCD_RS) | _BV(LCD_EN);	
    LCD_PORT = 0;		
	_delay_ms(10);			
	
	LCD_write_cmd(0b00000010);  //powrót kursora do pozycji 0
	LCD_write_cmd(0b00101000);   //4 bit, 2 linie, 5x7           
	LCD_write_cmd(0b00001111); 	//ekran włączony, kursor wałączony, kursor miga
	LCD_write_cmd(0b00000110);	//inkrementacja kursora, ekran nie zeminia polozenia
	LCD_write_cmd(0b00000001); 
	_delay_ms(2);
}

void LCD_clear() {
	LCD_write_cmd(0x01);
	cursor_x = 1;
	cursor_y = 1;

	_delay_ms(60);
}

void LCD_set_cursor(uint8_t x, uint8_t y) {
	cursor_x = x;
	cursor_y = y;

	LCD_write_cmd(((x - 1) * 0x40 + (y - 1)) | 0x80);
	_delay_ms(5);
}

void LCD_clear_y(uint8_t y) {
	
	LCD_set_cursor(cursor_x, y);

	for(int i = y; i <= 16; i++) {
		LCD_write_data(' ');
	}

	LCD_set_cursor(cursor_x, y);
}

void LCD_write(char *str) {

	while(*str != '\0') {
		cursor_y++;

		if(cursor_y == 16) {
			LCD_set_cursor(2, 1);
		}

		LCD_write_data(*str);
		str++;
	}
	
}

void LCD_write_menu_item(char *str, uint8_t line) {

	LCD_set_cursor(line, 1);
	LCD_write_data(' ');

	while(*str != '\0') {
		cursor_y++;

		LCD_write_data(*str);
		str++;
	}

	LCD_set_cursor(1, 1);
	
}

char* toCharArr(char* arr, uint8_t n, uint8_t num) {
	for(uint8_t i = 0; i < n; i++) {
		arr[i] = '\0';
	}


	if(num < 10) {
		arr[0] = num + 0x30;
	} else {
		uint8_t i = 1;	
		uint8_t r;
		
		while(num) {
			r = num % 10;
			num /= 10;

			arr[i] = r + 0x30;
			i--;
		}	
	}

	return arr;
}
