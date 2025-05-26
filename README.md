# SO2_project_1

## Instrukcja instalacji projektu za pomocą Developer Command Prompt for VS 2022

Kompilacja JezdacyFilozofowie/JedzacyFilozofowie.cpp
```
cl /EHsc JedzacyFilozofowie.cpp
```
Uruchomienie programu z dokładnie 1 dodatkowym argumentem - liczbą filozofów.
```
.\JedzacyFilozofowie 5
```
Uruchomienie programu na systemach UNIX nie jest możliwe ze względu na wykorzystaną bibliotekę do kontroli wyjścia w terminalu.

## Opis problemu
Problem ucztujących filozofów to zadanie polegające na synchronizacji dostępu procesów do ograniczonej ilości zasobów. 
Pięciu filozofów siedzi przy stole. Każdy wykonuje jedną z dwóch czynności – je lub myśli. Stół jest okrągły, przed każdym z nich znajduje się miska ze spaghetti, a pomiędzy każdą sąsiadującą parą filozofów leży widelec, a więc każda osoba ma przy sobie dwie sztuki – po swojej lewej i prawej stronie. Zakłada się, że każdy filozof korzysta z dwóch widelkców do jedzenia potrawy. Dodatkowo nie ma możliwości skorzystania z widelca, który nie znajduje się bezpośrednio przed daną osobą.

Filozofowie nigdy nie rozmawiają ze sobą, co stwarza zagrożenie zakleszczenia w sytuacji, gdy każdy z nich zabierze lewy widelec i będzie czekał na prawy (lub na odwrót).

Aby do takiej sytuacji nie dopuścić, w projekcie zastosowano hierarchię zasobów. Każdy filozof najpierw podnosi widelec o niższym numerze. Jeśli czterech filozofów jednocześnie podniesie swoje widelce z niższymi numerami, na stole pozostanie widelec z najwyższym numerem, przez co piąty filozof nie będzie mógł podnieść żadnego. Dzięki temu nie dojdzie do zakleszczenia.

## Wylistowanie:

### Wątków i co reprezentują:
- Wątki filozofów `(std::vector<std::thread> philosophers)`:  
Każdy wątek reprezentuje jednego filozofa
Liczba wątków jest określona przez `PHILOSOPHERS_COUNT` (podawane jako argument programu)
Każdy wątek wykonuje funkcję `run(id)`, gdzie wykonuje cyklicznie sekwencję: myślenie -> próba podniesienia widelców -> jedzenie -> odkładanie widelców
### Sekcje krytyczne i ich rozwiązanie:
- Dostęp do widelców:  
Rozwiązane przez tablicę mutexów `std::mutex* forks`
Każdy widelec jest reprezentowany przez jeden mutex
Dwaj filozofowie nie mogą jednocześnie posiadać jednego mutexa
Do zakleszczenia nie dochodzi, ponieważ filozofowie zawsze podnoszą widelce w tej samej kolejności (najpierw o niższym numerze)
- Dostęp do wyświetlania stanu (display_states()):  
Chroniony przez mutex `display_mutex`
Zabezpiecza przed jednoczesnym aktualizowaniem konsoli przez różnych filozofów. Dzięki temu wyświetlane dane zawsze są aktualne i prawidłowe.

# SO2_project_2

## Instrukcja instalacji projektu za pomocą Developer Command Prompt for VS 2022

Kompilacja SerwerChatu.cpp:
```
cl /EHsc SerwerChatu.cpp /link ws2_32.lib
```
Kompilacja KlientChatu.cpp:
```
cl /EHsc KlientChatu.cpp /link ws2_32.lib
```
Uruchomienie serwera:
```
serwer.exe
```
Uruchomienie klienta (możesz uruchomić wiele instancji, na tym samym lub innym komputerze w sieci):
```
klient.exe
```
Po uruchomieniu klienta podaj IP serwera (np. `127.0.0.1` dla lokalnego testu).

Uruchomienie programu na systemach UNIX nie jest możliwe ze względu na wykorzystaną bibliotekę WinSock.

## Instrukcja instalacji projektu na systemie Linux

Kompilacja serwera:
```
g++ SerwerChatu.cpp -o serwer -lpthread
```

Uruchomienie serwera:
```
./serwer
```

## Opis problemu

Celem projektu jest stworzenie prostego wielowątkowego serwera chatu oraz klienta w C++ z użyciem WinSock. Każdy klient może wysyłać i odbierać wiadomości w czasie rzeczywistym. Serwer obsługuje wielu klientów jednocześnie, a komunikacja odbywa się przez TCP.

## Wylistowanie:

### Wątków i co reprezentują:
- Wątki klientów na serwerze (`std::thread`):  
  Każdy wątek obsługuje jednego połączonego klienta. Dzięki temu wielu klientów może komunikować się z serwerem jednocześnie.
- Wątek odbierający wiadomości w kliencie (`std::thread`):  
  Pozwala na jednoczesne odbieranie wiadomości i pisanie przez użytkownika.

### Sekcje krytyczne i ich rozwiązanie:
- Dostęp do listy klientów i historii wiadomości:  
  Synchronizowany za pomocą mutexów (`std::mutex`). Zapobiega to konfliktom podczas jednoczesnego dostępu przez wiele wątków.
- Dostęp do wyświetlania wiadomości:  
  Chroniony przez mutex, aby zapobiec nakładaniu się komunikatów w konsoli.

---
