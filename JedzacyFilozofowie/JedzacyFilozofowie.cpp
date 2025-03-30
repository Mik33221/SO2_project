// JedzacyFilozofowie.cpp 

#include <iostream>
#include <thread>
#include <mutex>
#include <array>
#include <chrono>
#include <sstream>

#define NOMINMAX
#include <windows.h>

const int LICZBA_FILOZOFOW = 5;
std::array<std::mutex, LICZBA_FILOZOFOW> widelce;
std::array<std::string, LICZBA_FILOZOFOW> stany_filozofow;
std::stringstream wyswietlanie;

void ClearScreen()
{
    HANDLE                     hStdOut;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Move the cursor home */
    SetConsoleCursorPosition(hStdOut, homeCoords);
}

void wyswietl_stany() {
    wyswietlanie.str("");
    for (int i = 0; i < LICZBA_FILOZOFOW; i++) {
        wyswietlanie << "Filozof " << i << ": " << stany_filozofow[i] << "\n";
    }
    //system("cls");
    ClearScreen();
    std::cout << wyswietlanie.str();
    std::cout << std::flush;
}

void wyswietlaj() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		wyswietl_stany();
	}
}

void filozofuj(int id) {
    while (true) {
        // Myślenie
        {
            stany_filozofow[id] = "mysli...  ";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 2000 + 1000));

        // Podnoszenie widelców
        int pierwszy = std::min(id, (id + 1) % LICZBA_FILOZOFOW);
        int drugi = std::max(id, (id + 1) % LICZBA_FILOZOFOW);

        // Zawsze najpierw podnosimy widelec o niższym numerze
        widelce[pierwszy].lock();
        widelce[drugi].lock();
    
        // Jedzenie
        {
            stany_filozofow[id] = "je...     ";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 2000 + 1000));

        // Odkładanie widelców (kolejność nie ma znaczenia)
        widelce[pierwszy].unlock();
        widelce[drugi].unlock();
    }
}

int main() {
	srand(time(NULL));
    // Inicjalizacja stanów
    for (auto& stan : stany_filozofow) {
        stan = "myśli...";
    }

    std::array<std::thread, LICZBA_FILOZOFOW> filozofowie;
    
    // Tworzenie wątków dla filozofów
    for (int i = 0; i < LICZBA_FILOZOFOW; i++) {
        filozofowie[i] = std::thread(filozofuj, i);
    }

	//Tworzenie wątka do wyświetlania 
	std::thread watek_wyswietlajacy(wyswietlaj);

    // Oczekiwanie na zakończenie wątków (w tym przypadku nigdy nie nastąpi)
    for (auto& f : filozofowie) {
        f.join();
    }

    return 0;
}