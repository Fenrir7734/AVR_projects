# ADC
- `ADC` umożliwia konwersję 8 linii analogowych
- Możliwość tylko jednej konwersji jednocześnie, jeżeli chcemy wykonać konwersję napięcia z kilku linii, konwersje te będą musiały zostać wykonane pokolei.
- Po zakończeniu jednej konwersji, `ADC` musi zostać uruchomiony od nowa.
- Wartość napięcia możliwego do konwersji zawiera się pomiędzy 0V (gnd) a napięciem przyłożonym do pinu ADVREF.
- `ADC` może przeprowadzić konwersję napięcia w dwuch rozdzielczościach, 8 i 10 bitowej. Aby zmienić rozdzielczość na 8 bitową należy ustawić bit `LOWERS` w rejestrze `ADC_MR`
- zegar kontrolera ADC zawsze działa. *Setting SYSC and ADC bits in the clock set/clear registers of the PMC has no effect. The System Controller and ADC are continuously clocked.*
- Wyniki konwersji przechowywane są w dedykowanych rejestrach `ACD_CDRx` gdzie x oznacza numer kanału odpowiadającego danemu rejestrowi. Dodatkowo wynik ostatniej konwersji na jednym z aktywnych kanałów przechowywany jest w rejestrze `ADC_LCDR`. Kiedy konwersja jest zakończona, bit `EOCx` w rejestrze `ADC_SR` jest ustawiany. Jednocześnie ustawiny jest bit `DRDY` który oznacza, że w rejestrze `ADC_LCDR` zapisana jest wartość z ostatniej konwersji.
- Odczytanie w programie wartości z rejestru `ADC_CDRx` kasuje bit `EOCx` w rejestrze `ADC_SR`. Podobnie odczytanie wartości z rejestru `ADC_LCDR` powoduje wyzerowanie bitu `DRDY`.
- Jeżeli rejestr `ADC_CDR` nie został przeczytany zanim nastąpi zakończenie następnej konwersji, ustawiana jest flaga `OVRE` w rejestrze `ADC_SR`. Podobnie, jeżeli `DRDY` pozostaje w stanie wysokim gdy zakończy się kolejna konwersja, ustawiany jest bit `GOVRE` w `ADC_SR`.
- Aby rozpocząć konwersję należy ustawić bit `START` w rejestrze `ADC_CR`.
- Wystarczy raz ustawić `START` aby dokonać konwersji na wszystkich aktywnych kanałach.
- Aktywacja i dezaktywacja kanałów odbywa się poprzez odpowiednio rejestry `ADC_CHER` i `ADC_CHDR`. 

# Rejestry - podsumowanie
- `ADC_CR` - rejestr kontrolny
  - `SWRST` - wpisanie 1 powoduje reset rejestrów `ADC`
  - `START` - wpisanie 1 powoduje rozpoczęcie konwersji analogowo cyfrowej
- `ADC_MR` - rejestr do konfiguracji trybów przetwornika
  - `LOWRES` - wartość 0 - 10 bitowa rozdzielczość przetwornika, wartość 1 - 8 bitowa rozdzielczość
  - `PRESCAL` - ustawienine preskalera dla przetwornika ADCClock = MCK / ( (PRESCAL+1) * 2 )
  - `STARTUP` - Startup Time = (STARTUP+1) * 8 / ADCClock
  - `SHTIM` - Sample & Hold Time = SHTIM/ADCClock
- `ADC_CHER` - rejestr umożliwiający aktywowanie jednego z 8 kanałów przetwornika.
- `ADC_CHDR` - rejestr umożliwiający dezaktywowanie jednego z 8 kanałów przetwornika.
- `ADC_SR` - rejestr statusowy przetwornika `ADC`. **Czytanie z tego rejestru powoduje jego wyzerowanie.**
  - `EOCx` - wartość 0 - kanał x jest wyłączony albo konwersja się jeszcze nie zakończyła, wartość 1 - konwersja się zakończyła
  - `OVREx` - wartość 1 - error spowodowany zakończeniem konwersji nowego sygnału gdy flaga `EOCx` pozostawała w stanie wysokim.
  - `DRDY` - wartość 0 - żadna konwersja się nie zakończyła od czasu ostatniego odczytu `ADC_LCDR`, wartość 1 - przynajmniej jedna konwersja się zakończyła od czasu ostatniego odczytu `ADC_LCDR`
  - `GOVRE` - wartość 1 - error spowodowany zakończenie konwersji nowego sygnału gdy flaga `DRDY` pozostawała w stanie wysokim. 
- `ADC_LCDR` - rejestr umożliwiający odczyt wyniku ostatniej konwersji analogowo cyfrowej. Wartość ta jest nadpisywana w momęcie zakończenia kolejnej konwersji.
- `ADC_CDRx` - rejestr umożliwiający odczyt wyniku konwersji analogowo cyfrowej dla kanału x. Wartośc do tego rejestru jest wpisywana tylko gdy kanał x jest aktywny.
- `ADC_IER` - rejest umożliwiający aktywację przerwań od `ADC`
- `ADC_IDR` - rejest umożliwiający dezaktywację przerwań od `ADC`

```c
int main (){

    PMC_PCER = PMC_PCER_PIOA;//czy potrzebne jest załączanie zegara systemowego do ADC??
    ADC_CR = ADC_CR_SWRST; //reset rejestrów ADC -> zerowanie wartości rejestrów przechowujących liczby związane z napięciem
    ADC_MR = (23<<ADC_MR_PRESCAL_BIT)|(1<<ADC_MR_SHTIM_BIT)|(2<<ADC_MR_STARTUP_BIT);
    //ADC_CHER= (1 << 6);//załączenie kanału ADC na którym ma być konwersja (1 dla aktywacji kanału)
    ADC_CHER= (1 << 5);
    //start POJEDYNCZEJ KONWERSJI !! jeden start - jedna konwersja na aktywnych kanałach!

    InitLCD();
    SetContrast(30);
    LCDClearScreen();

    ADC_CR= ADC_CR_START;
    while (1) {
        if ((ADC_SR & ADC_SR_EOC6)!=0) {
                ADC_CR= ADC_CR_START;
        int a = ADC_CDR6;
        char c[4];
        itoa(a, c, 10);
        LCDSetRect(20, 20, 70, 120, 1, WHITE);
        LCDPutStr(c, 20, 20,LARGE, BLACK, WHITE);
        Delay_(100000);
        }
    }
}
```

```c
int main (){

    PMC_PCER = PMC_PCER_PIOA;//czy potrzebne jest załączanie zegara systemowego do ADC??
    ADC_CR = ADC_CR_SWRST; //reset rejestrów ADC -> zerowanie wartości rejestrów przechowujących liczby związane z napięciem
    ADC_MR = (23<<ADC_MR_PRESCAL_BIT)|(1<<ADC_MR_SHTIM_BIT)|(2<<ADC_MR_STARTUP_BIT);
    //ADC_CHER= (1 << 6);//załączenie kanału ADC na którym ma być konwersja (1 dla aktywacji kanału)
    ADC_CHER= (1 << 5);
    //start POJEDYNCZEJ KONWERSJI !! jeden start - jedna konwersja na aktywnych kanałach!

    InitLCD();
    SetContrast(30);
    LCDClearScreen();

    ADC_CR= ADC_CR_START;
    while (1) {
      if ((ADC_SR & ADC_SR_EOC5)!=0) {
            ADC_CR= ADC_CR_START;
      int a = ADC_CDR5;
      //float b = (1.0 * a * 20) / (500 * 1.0); 
      float b = ((a - 500 * 1.0) / 7 * 1.0) + 20 * 1.0;
      char c[30];
      sprintf(c, "Term: %.2f", b);
      //itoa(a, c, 10);
      LCDSetRect(70, 20, 70, 120, 1, WHITE);
      LCDPutStr(c, 70, 1, LARGE, BLACK, WHITE);
      //LCDPutStr("Term: ", 70, 1, LARGE, BLACK, WHITE);
      //LCDPutStr(c, 70, 40,LARGE, BLACK, WHITE);
      LCDSetCircle(70, 95, 3, BLACK);
      LCDPutStr("C", 70, 100,LARGE, BLACK, WHITE);
      //Delay_(100000);
    }
  }
}
```

```c
int main(void){
    PMC_PCER= …
    // Power management controller (s. 30 datasheet)
    …

    // konfiguracja ADC, konwersja wartości napięcia z kanału 3 przetwornika:
    ADC_CR = 1 << 0; // lub ADC_CR_SWRST – to inny zapis tego samego – reset przetwornika
    ADC_CHER = 1 << 3; // aktywowanie kanałów na których ma byd przeprowadzona konwersja
    ADC_IDR = 0xFFFFF; // świadomie wyłączamy przerwania od wszystkich zdarzeo(źródeł) z ADC
    ADC_MR = (23<<ADC_MR_PRESCAL_BIT) | (2<<ADC_MR_STARTUP_BIT) | (1<<ADC_MR_SHTIM_BIT)
                        // w rejestrze ADC_MR możliwe również ustawienia niższej rozdzielczości
    ADC_CR = 1<<1; // lub ADC_CR_START; // start konwersji na aktywnych kanałach
    while ((ADC_SR & ADC_SR_EOC3) == 0) {}  // dopóki flaga EOC3 jest rowna 0 (konwersja na kanale 3
                                            // nie zakooczyła się) czekaj
    … // wykonaj akcję przypisaną do konwersji
}
```