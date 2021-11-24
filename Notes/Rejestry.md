
# Spis treści
- [Spis treści](#spis-treści)
- [Rejestry](#rejestry)
  - [DDRx](#ddrx)
  - [PORTx](#portx)
  - [PINx](#pinx)
  - [TCNTn](#tcntn)
  - [TCCRn](#tccrn)
  - [OCRn](#ocrn)
  - [TIMSK](#timsk)
  - [TIFR](#tifr)
  - [MCUCR](#mcucr)
  - [MCUCSR](#mcucsr)
  - [GICR](#gicr)
  - [GIFR](#gifr)
- [Tryby pracy licznika](#tryby-pracy-licznika)
  - [Normal](#normal)
  - [CTC](#ctc)
  - [Tryb PWM](#tryb-pwm)
- [Przerwania](#przerwania)
  - [Jak uaktywnić przerwanie?](#jak-uaktywnić-przerwanie)
  - [sei()/cli()](#seicli)
  - [Wektory przerwań](#wektory-przerwań)
  - [Liczniki - funkcje obsługi przerwań](#liczniki---funkcje-obsługi-przerwań)
- [Przykłady](#przykłady)
  - [Przerwania od Timera](#przerwania-od-timera)
  - [Przerwanie zewnętrzne](#przerwanie-zewnętrzne)

# Rejestry

## DDRx
`DDRx` - rejestr konfiguracyjny - Po resecie wszystkie rejestry są wyzerowane (0x00) czyli domyślnie PORTx jest portem wejściowym.  
  
`DDRA = 0xFF` - port A jest portem wyjściowym 

## PORTx
`PORTx` - rejestr komunikacyjny - do zapisu

## PINx
`PINx` - rejestr komunikacyjny - tylko do odczytu, służy do odczytywania stanu portu lub śledzi stan odpowiadającego mu `PORTx` jeżeli jest on skonfigurowany jako port wyjściowy

```c
DDRB=0xF0;
PORTB=0x00;
char a=PINB;
```
Najstarsze 4 bity portu B (`PB4`-`PB7`) będą wyjściami w stanie niskim a najmłosze 4 bity (`PB0`-`PB3`) będą wejściami bez podciągania. Ich stan możemy odczytać za pomocą pejestru `PINB`. Rejstr `PINB` będzie zawierał stan wszystkich bitów rejestru `PORTB`. Ponieważ 4 najmłosze są wejściami, wiec ich stan bedzie zależał od stanu linii podłączonych do tych pinów portu. Natomiast stan 4 najstarszych będzie zawsze wynonsił 0 - bo taką wartość wpisaliśmy do rejestru `PORTB`.
  
```c
DDRB=0xF0;
PORTB=0xFF;
char a=PINB;
```
W tym przypadku odczytana wartość zmiennej `a` będzie wynosiłą 0xFF. Powyższy ciąg instrukcji ustawia 4 najstarsze bity portu B jako wyjścia, ich stan jest więc określany przez odpowiednie bity rejestru `PORTB`. Natomiast 4 najmłosze bity są wejściami, ale ponieważ odpowiadające im bity rejestru `PORTB` są ustawione, włączone są wewnętrzne rezystory podciągające. Stąd jeśli port zostawimy niepodłączony, to zawsze z `PINB` odczytamy wartość 0xFF. Gdyby rezystory podciągające na pinach skonfigurowanych jako wejściowe nie były włączone, to ich stan byłby nieustalony.

## TCNTn
Rejestr licznikowy czyli aktualny stan licznika. Można zmodyfikować stan licznika wpisując tutaj żądaną wartość.

## TCCRn
`TCCR0` jest rejestrem Timer0 który jest używany do ustawiania trybów timera. Aby Timer mógł działać należy ustawić mu źródło sygnału taktującego. Timer domyślnie nie ma ustawionego źródła zegara, jest zatrzymany, więc trzy Clock Select Bits (`CS02`, `CS01`, `CS00`) są ustawione na 0. Ponadto tryb pracy Timera0 możemy zmieniać poprzez manipulacje bitami `WGM01` i `WGM00`. Timer domyślnie znajduje się w normalnym trybie pracy (`WGM01` i `WGM00` są ustawione na 0).

- `CS02`, `CS01`, `CS00` - służą do ustawienia źródła zegara Timera
- `WGM01` i `WGM00` - służa do ustawienia trybu pracy Timera
- `COM01` i `COM00` - służa do konfiguracji trybu Fast PWM albo gdy ten tryb jest wyłączony

| Mode | WGM01 | WGM00 | Timer/Counter Mode of Operation |
| :---: | :---: | :---: | :---: |
| 0 | 0 | 0 | Normal |
| 1 | 0 | 1 | PWM, Phase Correct |
| 2 | 1 | 0 | CTC |
| 3 | 1 | 1 | Fast PWM |

| CS01 | CS01 | CS00 | Description |
| :---: | :---: | :---: | :---: |
| 0 | 0 | 0 | No clock source (Timer/Counter stopped) |
| 0 | 0 | 1 | clk/1 (No prescaling) |
| 0 | 1 | 0 | clk/8 |
| 0 | 1 | 1 | clk/64 |
| 1 | 0 | 0 | clk/256 |
| 1 | 0 | 1 | clk/1024 |
| 1 | 1 | 0 | External clock source on T0 pin. Clock on falling edge |
| 1 | 1 | 1 | External clock source on T0 pin. Clock on rising edge |

## OCRn
`OCRn` - rejestr dla Compare Match którego zawartość porównywana jest z zawartością rejestru `TCNTn` pod warunkiem uruchomienia trybu CTC licznika.

## TIMSK
`TIMSK` - jest rejestrem w którym możemy uaktywnić przerwania dla timerów oraz zdecydować kiedy te przerwania mają być generowane. Jest wspólny dla wszystkich liczników.

- `TOIE0` - flaga która uaktywnia tryb przerwania na przepełnienie licznika a więc kiedy flaga `TOV0` w `TIFR` zostanie ustawiona.
- `OCIE0` - flaga która uaktywnia tryb przerwania na Compare Match a więc kiedy flaga `OCF0` w `TIFR` zostanie ustawiona.

| OCIE2 | TOIE2 | TICIE1 | OCIE1A | OCIE1B | TOIE1 | OCIE0 | TOIE0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |

## TIFR
`TIFR` - Rejestr flag informujących o wystąpieniu przepełnienia
- `OCF0` - Flaga ta jest ustawiana jeżeli wystąpiła równość między licznikiem `TCNT0` a wartością w rejestrze `OCR0`
- `TOV0` - Flaga ta jest ustawiana jeżeli wystąpiło przepełnienie licznika `TCNT0` 
- 
| OCF2 | TOV2 | ICF1 | OCF1A | OCF1B | TOV1 | OCF0 | TOV0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |

## MCUCR

`MCUCR` - służy do ustawiania sposobu wyzwalania przerwania na INT0 oraz INT1.
  - `ISC11`, `ISC10` - odpowiadają za `INT1`
  - `ISC01`, `ISC00` - odpowiadają za `INT0`


| SE | SM2 | SM1 | SM0 | ISC11 | ISC10 | ISC01 | ISC00 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |


| ISC01 | ISC00 | Sposób zgłaszania przerwania dla `INT0` |
| :---: | :---: | :---: |
| 0 | 0 | Zgłaszanie niskim poziomem logicznym |
| 0 | 1 | Zgłaszanie negacją stanu logicznego |
| 1 | 0 | Zgłaszanie opadającym zboczem | 
| 1 | 1 | Zgłaszanie narastającym zboczem |

| ISC11 | ISC10 | Sposób zgłaszania przerwania dla `INT1` |
| :---: | :---: | :---: |
| 0 | 0 | Zgłaszanie niskim poziomem logicznym |
| 0 | 1 | Zgłaszanie negacją stanu logicznego |
| 1 | 0 | Zgłaszanie opadającym zboczem | 
| 1 | 1 | Zgłaszanie narastającym zboczem |

## MCUCSR
`MCUCSR` - służy do ustawianai sposobu wyzwalania przerwania na `INT2`
  - `ISC2` - odpowiada za `INT2`

| ISC2 | Sposób zgłaszania przerwania dla `INT2` |
| :---: | :---: |
| 0 | Zgłaszanie opadającym zboczem |
| 1 | Zgłaszanie narastającym zboczem |

## GICR
`GICR` - służy do uaktywnienia zewnętrznych przerwań `INT1`, `INT0` i `INT2`. 

- Wpisanie wartości `1` pod bit `INT1`, `INT0`, `INT2` uaktywnia odpowiadające mu przerwanie. 
-  `INT1`, `INT0` i `INT2` - nasłuchują odpowiednio na `PD3`, `PD2`, `PB2`

| INT1 | INT0 | INT2 | - | - | - | IVSEL | IVCE |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |

## GIFR
`GIFR` - w rejestrze tym mogą być ustawione flagi `INTF0`, `INTF1` i `INFT2` które oznaczają że wystąpiło żądanie przerwania zewnętrznego na odpowiadającej im linii.`INTF0`, `INTF1` i `INFT2` - flagi te są resetowane automatycznie w procedurze obsługi przerwania.

| INTF1 | INTF0 | INTF2 | - | - | - | - | - |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |

# Tryby pracy licznika

## Normal
Licznik zlicza do 255. Po przekroczeniu tej wartości następuje przepełnienie a w konsekwencji rejestr `TCNTn` jest resetowany oraz ustawiana jest flaga przepełnienia `TOVn` w rejestrze `TIFR`

## CTC
Licznik działa jak w trybie normalnym, z jedną różnicą - rejestr zliczający `TCNTn` jest zerowany za każdym razem, gdy jego stan zrówna się ze stanem rejestru `OCRn`. Jeżeli `TCNTn` zrówna się z `OCRn` ustawiana jest flaga `OCFn` w rejestrze `TIFR` 

## Tryb PWM

# Przerwania

Gdy pojawia się przerwanie, którym może być niemal dowolne zdarzenie, jak na przykład przepełnienie się licznika procesor wstrzymuje wykonywanie funkcji głównej i przechodzi do funkcji obsługi przerwania `ISR`.
Przerwanie jest to zdarzenie które powoduje zmianę kolejności wykonywania programu. Jeżeli nastąpi przerwanie program wstrzymuje aktualnie wykonywane zadanie i przechodzi do odpowiedniej procedury obsługi przerwań.

## Jak uaktywnić przerwanie?
- Globalna flaga zezwolenia na przerwanie musi być odblokowana: makro `sei()`
- Flaga zezwolenia na przerwanie, które chcemy obsłużyć, musi byc ustawiona: 
  - Przerwanie zewnętrzne: ustawienie odpowiednich flag w rejestrze `GICR` oraz sposobu wyzwalania w rejestrze `MCUCR` lub `MCUCSR`
  - Przerwanie od timera: ustawienie odpowiednich flag w rejestrze `TIMSK`
- Musi istnieć kod procedurt obsługi przerwania: Makro `ISR` z wektorem przerwania np. `ISR(INT0_vect) {}`
- Musi wystąpić zdarzenie wywołujące przerwanie np. przejście z stanu wysokiego na stan niski

## sei()/cli()
Makrodefinicje te powodują odpowiednio odblokowanie i zablokowanie przerwań poprzez modyfikację flagi `I` w rejestrze `SREG`. Makrodefinicja `sei()` powinna byc wykorzystywana tylko raz w programie, do odblokowania przerwań. Nigdy nie powinna zajść sytuacja, w której konieczne byłoby wykorzystanie makrodefinicji `cli()`

## Wektory przerwań
  - Przepełnienie licznika - `TIMER0_OVF_vect`, `TIMER1_OVF_vect`, `TIMER2_OVF_vect`
  - Osiągnięcie przez liczbik zadanej wartości - `TIMER0_COMP_vect`, `TIMER1_COMP_vect`, `TIMER2_COMP_vect`
  - Zewnętrzne żądanie przerwania - `INT0_vect`, `INT1_vect`, `INT2_vect`

## Liczniki - funkcje obsługi przerwań

| Przepełnienie | Porównanie | Rejestracja impulsów |
| :-----------: | :--------: | :------------------: |
| `TIMER0_OVF_vect` | `TIMER0_COMP_vect` | |
| `TIMER1_OVF_vect` | `TIMER1_COMPA_vect`  `TIMER1_COMPB_vect` | `TIMER1_CAPT_vect`|
| `TIMER2_OVF_vect` | `TIMER2_COMP_vect` | |

# Przykłady

## Przerwania od Timera
```c
#include <avr/io.h> //biblioteka do obsługi wejścia/wyjścia
#include <avr/interrupt.h>	//biblioteka do obsługi przerwań


/*
volatile żeby umożliwić modyfikację tej zmiennej w 
procedurze obsługi przerwania
*/
volatile uint8_t counter = 0;

/*
Procedura obsługi przerwania która odpowiada
za przerwanie Compare Match. Procedura ta
zostanie wykonana jeżeli w rejestrze TIMSK 
została ustawiona flaga OCIE0 oraz w rejestrze
TIFR flaga OCF0.
*/
ISR(TIMER0_COMP_vect) {
    PORTA ^= _BV(PA0);
    counter++;
    
}

int main(void) {

    /*
    Ustawienie portu A jako wyjściowego
    */
    DDRA = 0xFF;
    
    /*
    Ustawienie timera w tryb CTC oraz źródła sygnału
    taktuającego na sygnał wewnętrzny wraz z 
    preskalerem 1024.
    */
    TCCR0 |= _BV(CS00) | _BV(CS02) | _BV(WGM01);
    
    /*
    Uaktywnienie przerwań w timer0. Ustawienei generowania
    żądania przerwania na Compare Match.
    */
    TIMSK |= _BV(OCIE0);
    
    /*
    Ustawienie wartości rejestru OCR0 tak aby zliczał 100ms
        
    1MHz/1024 = 976.6Hz
    1/976.6Hz = 1.024ms
    100ms/1.024ms = 98
    98 - 1 = 97 	<- ponieważ liczymy od 0

    Nie wiem czy to nie powinno być jeszcze podzielone przez 2
    */
    OCR0 = 97;

    /*
    Ustawienie globalnej flagi zezwolanie na przerwanie.
    */
    sei();

    while(1) {}
}
```

## Przerwanie zewnętrzne

```c
#include <avr/io.h>			//biblioteka do obsługi wejścia/wyjścia
#include <avr/interrupt.h> 	//biblioteka do obsługi przerwań


/*
Procedura obsługi przerwań z wektorem 
przerwania INT0_vect który odpowiada za
zewnętrzne żądanie przerwania dla linii
INT0.
*/
ISR(INT0_vect) {
    PORTA ^= _BV(PA0);
}


/*
Procedura obsługi przerwania dla przerwania
INT2.
*/
ISR(INT2_vect) {
    PORTA ^= _BV(PA2);
}


int main(void) {

    DDRA = 0xFF;
    
    /*
    Ustawienie wyzwalania przerwania na INT0 
    poprzez zbocze narastając. 
    */
    MCUCR |= _BV(ISC01) | _BV(ISC00); 

    /*
    Ustawienei wyzwalania przerwania na INT2
    poprzez zbocze narastające
    */
    MCUCSR |= _BV(ISC2); 

    /*
    Uaktywnienie przerwań INT0 oraz INT2

    INT1 nasłuchuje na PD3
    INT0 nasłuchuje na PD2
    INT2 nasłuchuje na PB2

    Żądanie przerwania może być zgłoszone niezależnie
    czy dany pin jest ustawiony jako wejście czy wyjście
    */		
    GICR |= _BV(INT0) | _BV(INT2);
            
    /*
    Ustawienie flag przerwania dla INT0 oraz INT2. 
    Poniższa operacja spowoduje natychmiastowe 
    przejście do procedury obsługi przerwania gdy
    obsługa przerwań zostanie już aktywowana.
    */
    GIFR |= _BV(INTF0) | _BV(INTF2);
    
    /*
    Funkcja ta odblokowuje obsługę przerwań poprzez 
    ustawienie flagi I w rejestrze SREG. Funkcja ta
    powinna być wywołana tylko raz, na poczatku programu 
    */
    sei(); 								

    while(1) {}
}

```