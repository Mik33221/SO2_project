// JedzacyFilozofowie.cpp

#include <iostream>
#include <thread>
#include <mutex>
#include <array>
#include <chrono>
#include <sstream>

#define NOMINMAX
#include <windows.h>

const int PHILOSOPHERS_COUNT = 5;
std::array<std::mutex, PHILOSOPHERS_COUNT> forks;
std::array<std::string, PHILOSOPHERS_COUNT> philosophers_state;
std::stringstream display;
std::mutex display_mutex;

void ClearScreen()
{
	HANDLE                     hStdOut;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

void display_states() {
	display.str("");
	for (int i = 0; i < PHILOSOPHERS_COUNT; i++) {
		display << "Filozof " << i << ": " << philosophers_state[i] << "\n";
	}
	//system("clear"); // Uncomment for Linux
	ClearScreen();
	std::cout << display.str();
	std::cout << std::flush;
}

void run(int id) {
	while (true) {

		// Thinking
		display_mutex.lock();
		{
			philosophers_state[id] = "mysli...  ";
		}
		display_states();
		display_mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 2000 + 1000));

		// Getting forks
		int first = std::min(id, (id + 1) % PHILOSOPHERS_COUNT);
		int second = std::max(id, (id + 1) % PHILOSOPHERS_COUNT);


		// Always get forks with lower number first
		forks[first].lock();
		forks[second].lock();

		// Eating
		display_mutex.lock();
		{
			philosophers_state[id] = "je...     ";
		}
		display_states();
		display_mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 2000 + 1000));

		// Putting forks back
		forks[first].unlock();
		forks[second].unlock();
	}
}

int main() {
	srand(time(NULL));
	// Initialize
	for (auto& stan : philosophers_state) {
		stan = "myśli...";
	}

	std::array<std::thread, PHILOSOPHERS_COUNT> philosophers;

	// Create threads
	for (int i = 0; i < PHILOSOPHERS_COUNT; i++) {
		philosophers[i] = std::thread(run, i);
	}

	for (auto& f : philosophers) {
		f.join();
	}

	return 0;
}