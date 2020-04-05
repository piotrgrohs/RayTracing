URUCHOMIENIE PROJEKTU
=====================

1. Należy otworzyć za pomocą środowiska Visual Studio 2019 plik solucji "RayTracing.sln" znajdujący się w folderze "project".
2. W celu przetestowania kompilujemy i uruchamiamy projekt (F5) - pojawiające się "warningi" ignorujemy. Po uruchomieniu programu i jego zakończeniu sprawdzamy, czy w katalogu "scripts" pojawił się plik "script0.bmp" - powinien przedstawiać jednolicie czarny obrazek. Jeżeli tak jest, wszystko jest OK.

UWAGA: Proszę uruchamiać program z poziomu kompilatora. Domyślnie jako katalog roboczy został wybrany folder "scripts", a parametrem wejściowym jest plik "script0.txt". 

UZUPEŁNIENIE PROJEKTU
=====================
1. Uzupełnienie metody obliczającej przecięcie promienia z kulą: w pliku Objects.cpp uzupełniamy metodę Sphere::FindInter (pierwsza metoda w tym pliku). Należy zapoznać się z uwagami. 
2. Dodanie teksturowania sferycznego: w pliku Mapping.h należy uzupełnić metodę GetUV dla klasy Spherical. Należy zapoznać się z uwagami.
3. Uzupełnienie metody obliczającej przecięcie promienia z płaszczyzną: po wyprowadzeniu odpowiednich wzorów na kartce, w pliku Objects.cpp uzupełniamy metodę Plane::FindIter celem wyznaczenia przecięcia z płaszczyzną. Wszystkie czynności przeprowadzamy analogicznie jak dla kuli.

WYSYŁANIE WYNIKÓW
=================

1. Należy wysłać TYLKO pliki 'objects.cpp' oraz 'mapping.h' spakowane do archiwum ZIP (i tylko zip!).
2. Na końcu laboratorium należy pamiętać o usunięciu z komputera wszystkich plików laboratoryjnych.


-------------------------------------------------------------
INFORMACJE DODATKOWE
-------------------------------------------------------------

INFORMACJE OGÓLNE
=================

Dostarczony program jest implementacją programu do ray-traceingu. Po skompilowaniu działa on w trybie wsadowym (tj. z wiersza poleceń), uruchamia się go w postaci:

RayTracing.exe nazwaskryptu.txt

Skrypty zawierają w sobie opis sceny (trójwymiarowego świata), czyli np. informację o położeniu kamery, obiektów (kul, płaszczyzn itp.), źródeł oświetlenia, użytych teksturach itd.

STRUKTURA KATALOGÓW
===================

"program" - zawiera skompilowany program w pełnej wersji, można się pobawić celem zobaczenia jak to wszystko powinno prawidłowo działać

"project" - pliki źródłowe projektu (Visual Studio 2019)

"scripts" - zawiera skrypt "script0.txt" z opisem przykładowej sceny na której testować będziemy projekt. Dodatkowo znajdują się tu obrazki które wyznaczają efekty jakie należy uzyskać:
script0-kule.jpg - po wprowadzeniu kodu obliczającego przecięcia z kulą
script0-kule+tekstury.jpg - po dodaniu kodu realizującego teksturowanie sferyczne
script0-plaszczyzny+kule+tekstury.jpg - po dodaniu kodu obliczającego przecięcie z płaszczyzną

"examples" - zawiera skrypty dodatkowe, które mogą posłużyć do testów

"images" - przykładowe obrazki wygenerowane przy pomocy programu
