- Timer zlicza od 0 do 255
- Tempo zliczania można okreslic wybierając źródło sygnału taktującego licznika.
- Po osiągnięciu przez licznikk wartości maksymalnej zliczanie jest kontynowane od zera.
- zliczanie może być wstrzymane tylko poprzez odłaczenie sygnału taktującego licznika.
- Preskaler działa jak zwykły licznik liczacy do ustalonej wartości, po zliczeniu generowany jest impuls, który następnie trafia do timera.
- Jedyną możliwościa zatrzymania timera (jego wyłaczenia) jest wybranie takiej konfiguracji preskalera, w której jest on odłączony od sygnału zegarowego

- Osiągnięcie wartości skrajnych (przepełnienie licznika) sygnalizowane jest poprzez ustawienie bitu TOVn rejestru TIFR. Flaga ta może być traktowana jakok dodatkokwy bit rejestru liczniak. Jest ona kasowana programowo poprzez wpisanie do niej wartości 1 lub automatycznie po skoku do procedury obsługi przerwania przepełnienia licznika (TIMERn_OVF_vect). 
- Przerwanie przepełnienia licznika możan odblokować poprzez ustawienie bitu TOIEn rejestru TIMSK.
- Licznik można programowo inicjować dowolną wartościa, można także w każdej chwili rejestr licznka odczytywać. Operacji tych dokonuje się poprzez rejstr TCNTn
- Pozytywny wynik porównianai rejestru TCNTn z danym rejestrm OCRn powoduje ustawienie flagi OCFn w rejsestrze TIFRn - kazdy z rejestrów OCR ma własną flagę. Jej zerowanie odbywa sie programow, poprzez wpisanie na odpowiednią pozycję rejestru wartości 1, lub automatycznie, po wykonaniu skoku do procedury obsługi przerwania TIMERn_COMPx_vect.
- Aby wowołanei takiego przerwania było możliwe należy ustawić odpowiednią flagę zezwolenia na przerwanie OCIEn w rejestrze TIMSKn.
- Wpisywanie nowej wartości do rejstru TCNTn powoduje zablokowanie porównania z rejstrami OCRn na jeden cykl zegara tiemra. W efekcie, pomimo że TCNTn będzie równe OCRn nie zostanie ustawiona flaga OCFn.
- Zdarzenie Compare Match można wywołać programowo poprzez ustawienie flagi FOCn rejestru TCCRn.

- jeśli okres sygnału trwa 100ms a stan 1 występuje przez 20ms to współczynnik wypełnienia wynosi (20/100)*100% = 20%
- W trybie fast PWM licznik zlicza tylko w górę, dzięki czemu częstotliwość uzyskanego przebiegu PWM jest dwuktornie wyższa niż w innych trybach PWM.
- W zależności od konfiguracji licznika osiągnięcie wybranego stanu sygnalizowane jest ustawieniem flagi TOV lub OCF. W przypadku odblokowania stosownych przerwań generowane są także przerwania o wektorach TIMn_OVF_vecy lub TIMn_COMP_vect