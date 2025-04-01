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
