- Zmiana stanu pewnych wejść (przerwania INT0, INT1, INT2, ICP1)
- Określony stan pewnych wejść (przerwania INT0, INT1).
- Przepełnienie licznika (TIMER0 OVF, TIMER1 OVF, TIMER2 OVF).
- Osiągnięcie przez licznik zadanej wartości (TIMER0 COMP, TIMER1 COMPA, TIMER1 COMPB, TIMER2 COMP).
- Zakończenie przetwarzania analogowo-cyfrowego (ADC).
- Zakończenie transmisji przez interfejs szeregowy (SPI STC, USART TXC, TWI)
- Odebranie danych przez interfejs szeregowy (USART RXC)
- Gotowość pamięci EEPROM (EERDY).
- Zmiana stanu wyjścia komparatora (ANACOMP).

- powrót z procedury obsługi przerwania odbywa się poprzez instrukcję RETI - instrukcja ta zachowuje się jak instrukcja RET z tym że dodatkowo ustawia flagę I w rejestrze SREG, co umożliwia przyjmowanie kolejnych przerwań.


## Wektory przerwań
- INT0_vect - Zewnętrzne żadanie przerwania
- PCINT0_vect - Przerwanie zmiany stanu pinu
- TIMER0_COMP_vect - Przerwanie Compare Match
- TIMER0_OVF_vect - przerwanie nadmiaru licznika timera

Opcje:
- ISR_BLOCK - wartość domyślna. Funkcja tak zadeklarowana będzie wykonywana z zablokawanymi przerwaniami, przerwania zostaną odblokowane po jej zakończeniu. Można ją pominąc.
- ISR_NOBLOCK - funkcja odblokuje przerwania najszybciej jak to tylko jest możliwe. Procedura obsługi przerwań będzie mogła być przerwana przez inne przerwania, również przez kolejne przerwanie które ją wywołało

## Warunki przerwania
- Globalna flaga zezwolenia na przerwanie musi być odblokowana: makro `sei()`
- Flaga zezwolenia na przerwanie, które chcemy obsłużyć, musi byc odblokowana: ustawienie odpowiednich flag w rejestrze `GICR`
- Musi istnieć kod procedurt obsługi przerwania: Makro `ISR` z wektorem przerwania np. `ISR(INT0_vect) {}`
- Musi wystąpić zdarzenie wywołujące przerwanie np. przejście z stanu wysokiego na stan niski

## Procedura obsługi przerwań
 - Nie może zwracać żadnego wyniku, `void`
 - musi zawierać specjalny prolog i epilog zapewniające przywrócenei stanu wszystkich zmodyfikowanych rejestrów mikrokontrolera do wartości jakie miały one w chwili wystąpenia przerwania
 - Aby nie musieć samemu tego pisać w języku C można zastosować makrodefinicję ISR(vector, ...) przyjmującą dwa parametry: `vector` określający numer wektora przerwania, którego dotyczy oraz opcjonaly parametr definiujący sposób w jaki zostanie wygenerowany prolog i epilog funkcji (`ISR_BLOCK` i `ISR_NONBLOCK`)

## sei()/cli()
Makrodefinicje te powodują odpowiednio odblokowanie i zablokowanie przerwań poprzez modyfikację flagi `I` w rejestrze `SREG`. Makrodefinicja `sei()` powinna byc wykorzystywana tylko raz w programie, do odblokowania przerwań. Nigdy nie powinna zajść sytuacja, w której konieczne byłoby wykorzystanie makrodefinicji `cli()`

## volatile 
Jest to słowo kluczone które służy do poinstruowania kompilatora, jak ma traktować podaną zmienną. Mówi mu że zmienna zadeklatowana z tym modyfikatorem może być modyfikowana w sposób niewynikający bezpośrednio z otaczającego kodu. Przy odwołaniu do tej zmiennej kompilator zawsze będzie pobierał jej wartość z pamięci unikając tworzenia jej kopii np. w rejestrach procesora. Dodanie modyfikatora `volatile` jest niezbędne w sytuacji, kiedy zmienna globalna jest modyfikowana w procedurze obsługi przerwania i jakiejkolwiek innej części kodu lub kiedy jej wartość może zmieniać się w sposób niewyninkający bezpośrednio z wykonywania kodu.

Pomiędzy różnymi odczytami, wartości zmiennej mogą być różne, nawet jeśli nie były zmodyfikowane w kodzie. Zastosowanie `volatile` powstrzymuje kompilator optymalizujący przed pomijaniem zapisów do pamięci lub w wypadku kolejnych odczytów czy zapisów zmiennej przed zastąpieniem jej w skompilowanym kodzie przez stałą.

- Im mniejszy adres tym wyższy priorytet przerwania
- Najwyższy priorytet ma `RESET`, potem przerwanie zewnętrzne `INT0` itd.
- W rejestrze `MCUCR` ustawia się sposób generowania przerwania przez linię `INT0` i `INT1`
- W rejestrze `MCUCSR` ustawia się sposób generowania przerwania przez linię `INT2`
- `MCUCR` dla `INT0` i `INT1`, `MCUCSR` dla `INT2`
- `GICR` - rejestr sterowania przerwaniami zewnętrznymi INTx - tutaj uaktywnia się poszczególne przerwania `INT1`, `INT0` i `INT2`
- `GIFR` - rejestr znaczników zgłaszania przerwań zewnętrznych - jeżeli zostanie wykryty sygnał wyzwalający przerwanie w tym rejestrze ustawiana jest odpowiednia flaga `INTF1`, `INTF0`, `INTF2` odopowiednio dla `INT1`, `INT0` i `INT2`
- `INTFx` - bit zgłoszenia przerwania na wejściu `INTx`, ustawiany gdy przerwanie jest odmaskowane i zgłoszone, kasowany po wejściu do procedury obsługi lub poprzez zapis jedynki logicznej.
- **Domyślnie `INT1` nasłuchuje na `PD3`, `INT0` na `PD2`, `INT2` na `PB2`**
- `TIMSK` - Rejestr maski przerwań do liczników, odblokowuje przerwania dla liczników, wspólny dla wszystkich liczników
  - `TOIE0` - bit maski przerwania od przepełnienia licznika Timer0. Gdy zostanie ustawionna ta flaga to jeśli w rejestrze `TIFR` zostanie ustawiona flaga `OVF0` oznaczające przepełnienie licznika, wystąpi przerwanie. Wykona się wtedy podprogram przerwania z wektorem przerwania `TIMER0_OVF_vect`
  - OCIE0 - bit maski przerwania do funkcji Output-Compare licznika Timer0. Tak samo jak wyżej tylko przerwanie zostanie wywołane gdy zostanie ustawiona flaga `OCF0`. Zostanie wywołany podprogram z wektorem przerwanina `TIMER0_COMP_vect`