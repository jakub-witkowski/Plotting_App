# LSR-Plot
LSR-Plot: aplikacja stworzona w ramach projektu zaliczeniowego na studiach podyplomowych "Programowanie z elementami systemów wbudowanych" (Zachodniopomorski Uniwersytet Technologiczny, Szczecin, luty 2024). W repozytorium znajduje się kod źródłowy aplikacji i testy jednostkowe.

Aplikacja wykorzystuje biblioteki Qt (wersja 6; https://www.qt.io) oraz framework ROOT (https://root.cern).

Ogólna logika programu - pseudokod:

1. Wczytaj zbiór danych do obiektu klasy TData. Zbiór danych składa się z wartości oznaczających głębokość w profilu rdzenia wiertniczego (w m poniżej poziomu dna - meters below seafloor) oraz wartości odpowiadających wiekowi geologicznemu próbki, wyrażonemu w mln lat. Zbiór danych wczytywany jest z pliku csv.
2. Sprawdź czy zbiór danych zawiera luki stratygraficzne (hiatusy).
3. Jeśli nie ma hiatusów, przekaż dane do jednego obiektu klasy TSegment.
4. Jeśli występuje n hiatusów, utwórz n+1 obiektów klasy TSegment i przekaż do nich dane z odpowiednich przedziałów głębokości i wieku.
5. W każdym obiekcie klasy TSegment znajdź wielomian o najkorzystniejszym dopasowaniu (wyrażonym przez stosunek chi2/ndf) i zwróć jego parametry, chi2, liczbę stopni swobody (ndf).
6. W każdym obiekcie klasy TSegment sprawdź, czy wyrażenie wielomianowe nie wykazuje zbyt wysokiego dopasowania do zbioru danych ("overfitting"). Jeśli występuje overfitting, wybierz wyrażenie wielomianowe o niższym stosunku chi2/ndf.
7. W każdym obiekcie klasy TSegment przelicz głębokości zgodnie ze znalezioną regresją wielomianową (wygładzanie zbioru danych).
8. każdym obiekcie klasy TSegment oblicz wahania tempa akumulacji osadu w funkcji czasu dla surowego oraz wygładzonego zbioru danych.
9. Zwróć tabelę surowych oraz wygładzonych wartości tempa akumulacji osadu dla odcinków pomiędzy poszczególnymi punktami węzłowymi zbioru danych.
10. Utwórz obiekt klasy TPlot przedstawiający wykres głębokości względem wieku (surowy i wygładzony zbiór danych) oraz wykres wahań tempa akumulacji (surowy i wygładzony zbiór danych).

Przykład potencjalnego zastosowania z rzeczywistego projektu badawczego: https://cp.copernicus.org/articles/17/1937/2021/ (szczególnie suplement: https://cp.copernicus.org/articles/17/1937/2021/cp-17-1937-2021-supplement.pdf).