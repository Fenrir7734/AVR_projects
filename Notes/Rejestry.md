- `DDRA = 0xFF` - port A jest portem wyjściowym 

- `TCNT0` - rejestr licznikowy

- `TCCR0` jest rejestrem Timer0 który jest używany do ustawiania trybów timera. Aby Timer mógł działać należy ustawić mu źródło sygnału taktującego. Timer domyślnie nie ma ustawionego źródła zegara, jest zatrzymany, więc trzy Clock Select Bits (`CS02`, `CS01`, `CS00`) są ustawione na 0. Ponadto tryb pracy Timera0 możemy zmieniać poprzez manipulacje bitami `WGM01` i `WGM00`. Timer domyślnie znajduje się w normalnym trybie pracy (`WGM01` i `WGM00` są ustawione na 0).
  - `CS02`, `CS01`, `CS00` - służą do ustawienia źródła zegara Timera
  - `WGM01` i `WGM00` - służa do ustawienia trybu pracy Timera
  - `COM01` i `COM00` - służa do konfiguracji trybu Fast PWM albo gdy ten tryb jest wyłaczony

- `OCR0` - rejestr dla Compare Match którego zawartość porównywana jest z zawartością rejestru `TCNT0`

- `TIFR` - Rejestr flag informujących o wystąpieniu przepełnienia
  - `OCF0` - Flaga ta jest ustawiana jeżeli wystąpiła równość między licznikiem `TCNT0` a wartością w rejestrze `OCR0`
  - `TOV0` - Flaga ta jest ustawiana jeżeli wystąpiło przepełnienie liczninka `TCNT0` 

- `TIMSK` - jest rejestrem w którym możemy uaktywnić przerwanina dla timerów oraz zdecydować kiedy te przerwania mają być generowane.
  - `TOIE0` - flaga która uaktywnia tryb przerwania na przepełnienie licznika a wiec kiedy flaga `TOV0` w `TIFR` zostanie ustawiona.
  - `OCIE0` - flaga która uaktywnia tryb przerwania na Compare Match a wiec kiedy flaga `OCF0` w `TIFR` zostanie ustawiona.

- `MCUCR` - służy do ustawiania sposobu wyzwalania przerwania na INT0 oraz INT1.
  - `ISC11`, `ISC10` - odpowiadają za `INT1`
  - `ISC01`, `ISC00` - odpowiadają za `INT0`

- `MCUCSR` - służy do ustawianai sposobu wyzwalania przerwania na `INT2`
  - `ISC2` - odpowiada za `INT2`

- `GICR` - służy do uaktywnienia zewnętrznych przerwań `INT1`, `INT0` i `INT2`.
  -  `INT1`, `INT0` i `INT2` - nasłuchują odpowiednio na `PD3`, `PD2`, `PB2`

- `GIFR` - w rejestrze tym mogą być ustawione flagi `INTF0`, `INTF1` i `INFT2` które oznaczają że wystąpiło żadanie przerwania zewnętrznego na odpowiadającej im linii.
  - `INTF0`, `INTF1` i `INFT2` - flagi te są resetowane automatycznie w procedurze obsługi przerwania.

- `SREG`
  - `I` - flaga odpowiadająca za globalną zgodę na obsługe przerwań
  

- wektory przerwań:
  - `TIMER0_OVF_vect`
  - `TIMER0_COMP_vect`
  - `INT0_vect` - zewnętrzne żadanie przerwania

- `PORTA` - rejestr komunikacyjny - do zapisu
- `PINA` - rejestr komunikacyjny - tylko do odczytu, służy do odczytywania stanu portu lub śledzi stan `PORTA` jeżeli jest on skonfigurowany jako port wyjściowy
- `DDRA` - rejestr konfiguracyjny - Po resecie wszystkie rejestry są wyzerowane (0x00) czyli domyślnie PORTA jest portem wejściowym

## Liczniki - funkcje obsługi przerwań

| Przepełnienie | Porównanie | Rejestracja impulsów |
| :-----------: | :--------: | :------------------: |
| `TIMER0_OVF_vect` | `TIMER0_COMP_vect` | |
| `TIMER1_OVF_vect` | `TIMER1_COMPA_vect`  `TIMER1_COMPB_vect` | `TIMER1_CAPT_vect`|
| `TIMER2_OVF_vect` | `TIMER2_COMP_vect` | |

## TCCR0

| Mode | WGM01 | WGM00 | Timer/Counter Mode of Operation |
| :---: | :---: | :---: | :---: |
| 0 | 0 | 0 | Normal |
| 1 | 0 | 1 | PWM, Phase Correct |
| 2 | 1 | 0 | CTC |
| 3 | 1 | 1 | Fast PWM |

| CS01 | CS01 | CS00 | Description |
| 0 | 0 | 0 | No clock source (Timer/Counter stopped) |
| 0 | 0 | 1 | clk/1 (No prescaling) |
| 0 | 1 | 0 | clk/8 |
| 0 | 1 | 1 | clk/64 |
| 1 | 0 | 0 | clk/256 |
| 1 | 0 | 1 | clk/1024 |
| 1 | 1 | 0 | External clock source on T0 pin. Clock on falling edge |
| 1 | 1 | 1 | External clock source on T0 pin. Clock on rising edge |

## TIMSK

| OCIE2 | TOIE2 | TICIE1 | OCIE1A | OCIE1B | TOIE1 | OCIE0 | TOIE0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |

- Bit 1 - `OCIE0`: Timer/Counter0 Output Compare Match Interrupt Enable
- Bit 0 - `TOIE0`: Timer/Counter0 Overflow Interrupt Enable

## TIFR

| OCF2 | TOV2 | ICF1 | OCF1A | OCF1B | TOV1 | OCF0 | TOV0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |

- Bit 1 - `OCF0`: Output Compare Flag 0
- Bit 0 - `TOV0`: Timer/Counter0 Overflow Flag
