- Mikrokontrolery AVR zwykle mają od kilku do kilkudziesięciu niezależnie programowalnych pinów IO pogrupowanych w 8-biotowe porty. Dostęp do nich na poziomie sprzętowym odbywa się za pomocą 3 specjalnych rejestró: `DDRx`, `PORTx`,`PINx`.
- Wszystykie definicje umożliwiające dostęp do portów IO procesora znajdują się w pliku nagłówkowym `<arv\io.h>`
- Każdy podrt IO skłąda się z 3 rejestrów: `PORTx`, `DDRx`, `PINx`
  - `DDRx` - określa kierunek poszczególnych pinów IO (wejście, wyjście)
  - `PORTx` - zawiera stan pinów ustawionych jako wyjścia lub aktywizuje rezystor podciągający przypisany pinowi
  - `PINx` - służy do odczytu stanu portu
- Domyślnie wszysytkie piny IO ustawione są jako wejścia bez włączonego rezystora podciągającego
- Dany pin portu jest pinem wyjściowym, jeśli odpowiadający mu bit w rejestrze `DDRx` ma wartość 1. W takiej sytuacji stan poszczególnych linii IO portu określa rejestr `PORTx`
- Jeśli dany bit rejestru `DDRx` ma wartość 0, to pin jest skonfigurowany jako wejście. W tej sytuacji, jeśli odpowiadający mu bit rejestru `PORTx` ma wartość 1, to dodatkowo włączany jest wbudowany w procesor rezystor podciagający, dzięki czemu na danym pinie panuje stan wysoki.
- Rejestr `PINx` odzwierciedla stan pinów danego portu.
- W rejestrze `PORTx` zawsze znajduje się wartość, którą tam pisaliśmy, a nie aktualny stan pinów portu IO. Aby odczytać stan pinów portu IO, musimy odczytać rejestr `PINx`
- Jeżeli dany pin portu ustawiony jest jako wyjście, jego stan w rejestrze PINx odpowiada wartości odpowiedniego bitu rejestru `PORTx`
  
```
DDRB=0xF0;
PORTB=0x00;
char a=PINB;
```
Najstarsze 4 bity portu B (`PB4`-`PB7`) będą wyjściami w stanie niskim a najmłosze 4 bity (`PB0`-`PB3`) będą wejściami bez podciągania. Ich stan możemy odczytać za pomocą pejestru `PINB`. Rejstr `PINB` będzie zawierał stan wszystkich bitów rejestru `PORTB`. Ponieważ 4 najmłosze są wejściami, wiec ich stan bedzie zależał od stanu linii podłączonych do tych pinów portu. Natomiast stan 4 najstarszych będzie zawsze wynonsił 0 - bo taką wartość wpisaliśmy do rejestru `PORTB`.
  
```
DDRB=0xF0;
PORTB=0xFF;
char a=PINB;
```
W tym przypadku odczytana wartość zmiennej `a` będzie wynosiłą 0xFF. Powyższy ciąg instrukcji ustawia 4 najstarsze bity portu B jako wyjścia, ich stan jest więc określany przez odpowiednie bity rejestru `PORTB`. Natomiast 4 najmłosze bity są wejściami, ale ponieważ odpowiadające im bity rejestru `PORTB` są ustawione, włączone są wewnętrzne rezystory podciągające. Stąd jeśli port zostawimy niepodłączony, to zawsze z `PINB` odczytamy wartość 0xFF. Gdyby rezystory podciągające na pinach skonfigurowanych jako wejściowe nie były włączone, to ich stan byłby nieustalony.