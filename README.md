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
- Wątki filozofów `std::vector<std::thread> philosophers`:  
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
cl /EHsc SerwerChatu.cpp
```
Kompilacja KlientChatu.cpp:
```
cl /EHsc KlientChatu.cpp
```
Uruchomienie serwera:
```
SerwerChatu.exe
```
Uruchomienie klienta (możliwe uruchomienie wielu instancji):
```
KlientChatu.exe
```
Po uruchomieniu klienta należy podać IP serwera (127.0.0.1 dla serwera lokalnego).

## Instrukcja instalacji projektu na systemie Linux

Kompilacja serwera:
```
g++ SerwerChatu_linux.cpp -o SerwerChatu -pthread
```

Uruchomienie serwera:
```
./SerwerChatu
```

## Opis problemu

Celem projektu jest stworzenie wielowątkowego serwera chatu oraz klienta w C++. Każdy klient może wysyłać i odbierać wiadomości w czasie rzeczywistym. Serwer obsługuje wszystkich klientów jednocześnie. Po dołączeniu nowego klienta serwer wysyła mu dotychczasową historię chatu. Po przyjęciu nowej wiadomości od któregoś z klientów serwer rozsyła ją do wszystkich podłączonych klientów, oraz zapisuje w historii.

## Wylistowanie:

### Wątków i co reprezentują:
- Wątki klientów na serwerze `std::thread(handle_client, client_socket)`:  
  Każdy wątek obsługuje jednego połączonego klienta. Dzięki temu wielu klientów może komunikować się z serwerem jednocześnie. Nowy wątek klienta synchronizuje historię, a następnie czeka na wiadomości.
- Wątek odbierający wiadomości w kliencie `std::thread reciever(recieve_messages, sock)`:  
  Pozwala na jednoczesne odbieranie wiadomości i pisanie przez użytkownika.

### Sekcje krytyczne i ich rozwiązanie:
- Dostęp do listy klientów i historii wiadomości:  
  Synchronizowany za pomocą mutexów `clients_mutex` i `history_mutex`. Zapobiega to konfliktom podczas jednoczesnego dołączenia wielu klientów, lub przychodzących w tym samym momencie wiadomości. Zapewnia 
 odpowiednią kolejność zapisu wiadomości w historii.

---
