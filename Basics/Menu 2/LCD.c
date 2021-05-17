#include "LCD.h"

uint8_t cursor_x = 0;
uint8_t cursor_y = 0;

/*
Funkcja odpowiadająca za wygenerowanie impulsu prostokątnego
na linii ENABLE
*/
void pulse() {
	LCD_PORT |= (1<<LCD_EN);		
	_delay_us(1);
	LCD_PORT &= ~ (1<<LCD_EN);
	_delay_us(100);
}

/*
Funkcja wysyłająca komendę do wyświetlacza.
*/
void LCD_write_cmd(uint8_t cmd) {
	/*
	Przestawienie linii RS na wartość 0 w celu wysłania 
	komendy.
	*/
	LCD_PORT &= ~_BV(LCD_RS);

	/*
	Wysłanie 4 starczych bitów i wygenerowanie impulsu prostokątnego
	w celu potwierdzenia przesłania danych.
	*/
	LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0); 
	pulse();

	/*
	Wysłanie 4 młodszych bitów i wygenerowanie impulsu prostokątnego
	w celu potwierdzenia przesłania danych.
	*/
	LCD_PORT = (LCD_PORT & 0x0F) | ((cmd & 0x0F) << 4);  
	pulse();	

	_delay_ms(2);
}

/*
Funkcja wysyłająca dane do wyświetlacza.
*/
void LCD_write_data(uint8_t cmd) {

	/*
	Przestawienie linii RS na wartość 1 w celu wysłania 
	danych.
	*/
	LCD_PORT |= _BV(LCD_RS);

	LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0); 
	pulse();

	LCD_PORT = (LCD_PORT & 0x0F) | ((cmd & 0x0F) << 4);
	pulse();	

	_delay_ms(2);
}

/*
Funkcja odpowiadająca za inicjalizację wyświetlacza. Powinna
zostać wywołana tylko raz i musi się to odbyć przed inicjalizacją
menu.
*/
void LCD_init() {
	
	/*
	Ustawienie kierunku wyjściowego dla wszystkich linii.
	*/
	LCD_DDR = 0xF0 | _BV(LCD_RS) | _BV(LCD_EN);	
    LCD_PORT = 0;		
	_delay_ms(10);			
	
	/*
	Wysłanei poszczególnych komend do wyświetlacza.
	*/
	LCD_write_cmd(0b00000010);  //powrót kursora do pozycji 0
	LCD_write_cmd(0b00101000);  //4 bit, 2 linie, 5x7           
	LCD_write_cmd(0b00001111); 	//ekran włączony, kursor włączony, kursor miga
	LCD_write_cmd(0b00000110);	//inkrementacja kursora, ekran nie zeminia polozenia
	LCD_write_cmd(0b00000001); 	//wyczyszczenie ekranu
	_delay_ms(2);
}

/*
Funkcja odpowiadająca za ustawienie kursora na pozycji określonej
przez argumenty przekazane do tej funkcji.
*/
void LCD_set_cursor(uint8_t x, uint8_t y) {
	
	/*
	Aktualizacja położenia kursora
	*/
	cursor_x = x;
	cursor_y = y;

	/*
	Przesunięcie kursora. 
	*/
	LCD_write_cmd(((x - 1) * 0x40 + (y - 1)) | 0x80);
	_delay_ms(5);
}

/*
Funkcja czyszcząca wyświetlacz. W wyniku czyszczenia kursor
ustawiany jest na pozycji początkowej a więc wartość zmiennych
cursor_x i cursor_y również muszą zostać odpowiednio zmienione.
*/
void LCD_clear() {
	LCD_write_cmd(0x01);
	cursor_x = 1;
	cursor_y = 1;

	_delay_ms(60);
}

/*
Funkcja odpowiadająca za wyczyszczenei linii na której znajduje
się kursor od pozycji y.
*/
void LCD_clear_y(uint8_t y) {
	
	/*
	Ustawienie kursora na pozycji y aktualnej linii
	*/
	LCD_set_cursor(cursor_x, y);

	/*
	Wypełnienie spacjami linii od aktualnej pozycji do 
	końca tej linii.
	*/
	for(int i = y; i <= 16; i++) {
		LCD_write_data(' ');
	}

	/*
	Powrót kursora do pozycji od której rozpoczeło się
	czyszczenie linii.
	*/
	LCD_set_cursor(cursor_x, y);
}

/*
Funkcja odpowiadająca za wypisanie napisu na wyświetlaczu.
*/
void LCD_write(char *str) {

	/*
	Wypisywanie trwa do czasu napotkania znaku pustego '\0'
	*/
	while(*str != '\0') {

		/*
		Inkrementacja położenia kursora.
		*/
		cursor_y++;

		/*
		Jeżeli pozycja kursora w linii jest równa 16 następuje
		przeniesienie do drugiej linii.
		*/
		if(cursor_y == 16) {
			LCD_set_cursor(2, 1);
		}

		/*
		Wypisanie jednego znaku na wyświetlacz i przesunięcie
		wskaźnika na kolejny znak
		*/
		LCD_write_data(*str);
		str++;
	}
	
}

/*
Wypisanie jednego elementu menu. Funkcja ta istnieje ponieważ
jest trochę wygodniejsza niż używanie LCD_write(char *str) do
wypisywanie elementów menu.
*/
void LCD_write_menu_item(char *str, uint8_t line) {

	/*
	Ustawienie kursora na odpowiedniej linii wyświetlacza oraz
	wypisanie znaku spacji w celu pozostawienia miejsca na kursor
	wskazujący na aktualny element menu.
	*/
	LCD_set_cursor(line, 1);
	LCD_write_data(' ');

	while(*str != '\0') {
		cursor_y++;

		LCD_write_data(*str);
		str++;
	}

	LCD_set_cursor(1, 1);
}

/*
Funkcja zamieniająca wartość typu int na znak char. W tym programie
służy głównie do zamiany numeru klawisza na tablicę znaków w celu
późniejszego wypisania na ekranie. Funkcja ta działa poprawnie tylko 
dla liczb dwucyfrowych.
*/
char* toCharArr(char* arr, uint8_t n, uint8_t num) {

	/*
	Wypełnienie tablicy znakowej znakami pustymi.
	*/
	for(uint8_t i = 0; i < n; i++) {
		arr[i] = '\0';
	}

	/*
	Jeżeli liczba jest mniejsza od 10 to wystarczy 
	jedynie dodać do niej kod ASCII pierwszej cyfry
	z tego systemu kodowania. 
	*/
	if(num < 10) {
		arr[0] = num + 0x30;
	} else {
		uint8_t i = 1;	//index tablicy
		uint8_t r;		//cyfra liczby
		
		/*
		Zamiana poszczególnych cyfr liczby na znaki.
		Zamiana i zapis do tablicy odbywa się w kolejności od cyfry
		najmniej znaczącej. 
		*/
		while(num) {
			r = num % 10;
			num /= 10;

			arr[i] = r + 0x30;
			i--;
		}	
	}

	return arr;
}
