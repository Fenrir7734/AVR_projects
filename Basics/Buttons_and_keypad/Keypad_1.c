    #include <avr/io.h>                //Biblioteka Wej/Wyj
    #include <util/delay.h>            //Biblioteka Opóźnień

    int main(void) {
    
    DDRA = 0xF0;     
    DDRC = 0xFF;    
    DDRB = 0xFF;

	uint8_t col = 0;
	uint8_t row = 0;
	uint8_t key = 0;

    while(1) {
             
          for(uint8_t i = 4; i <= 7; i++) {
		  	PORTA = ~_BV(i);
			col = PORTA;

			uint8_t row = PINA & 0x0F;

			if(row != 0x0F) {
				row = row | 0xF0;
				key = col & row;
				PORTC = key;
			} 
		}

		switch(key) {
			case 0b11101110: PORTB = 1; break;
			case 0b11011110: PORTB = 2; break;
			case 0b10111110: PORTB = 3; break;
			case 0b01111110: PORTB = 4; break;
			case 0b11101101: PORTB = 5; break;
			case 0b11011101: PORTB = 6; break;
			case 0b10111101: PORTB = 7; break;
			case 0b01111101: PORTB = 8; break;
			case 0b11101011: PORTB = 9; break;
			case 0b11011011: PORTB = 10; break;
			case 0b10111011: PORTB = 11; break;
			case 0b01111011: PORTB = 12; break;
			case 0b11100111: PORTB = 13; break;
			case 0b11010111: PORTB = 14; break;
			case 0b10110111: PORTB = 15; break;
			case 0b01110111: PORTB = 16; break;
			default: break;
		}
                     
    }
}
