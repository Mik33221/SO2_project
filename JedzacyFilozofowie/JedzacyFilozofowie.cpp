// JedzacyFilozofowie.cpp

#include <iostream>
#include <thread>
#include <mutex>
#include <array>
#include <chrono>
#include <sstream>
#include <vector>

#define NOMINMAX
#include <windows.h>

int PHILOSOPHERS_COUNT;
std::mutex* forks;
std::vector<std::string> philosophers_state;
std::stringstream display;
std::mutex display_mutex;

void ClearScreen()
{
	HANDLE                     hStdOut;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	// Move the cursor home 
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

void display_states() {
	display.str("");
	for (int i = 0; i < PHILOSOPHERS_COUNT; i++) {
		display << "Filozof " << i << ": " << philosophers_state[i] << "\n";
	}
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

int main(int argc, char** argv) {
	// argument checking
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <number_of_philosophers>\n";
		return 1;
	}

	try {
		PHILOSOPHERS_COUNT = std::stoi(argv[1]);
		if (PHILOSOPHERS_COUNT < 2) {
			std::cout << "Number of philosophers must be at least 2\n";
			return 1;
		}
	}
	catch (const std::exception& e) {
		std::cout << "Invalid number of philosophers\n";
		return 1;
	}

	srand(time(NULL));
	ClearScreen();

	// Initialize with the specified size
	forks = new std::mutex[PHILOSOPHERS_COUNT];

	philosophers_state.resize(PHILOSOPHERS_COUNT, "myśli...");

	std::vector<std::thread> philosophers(PHILOSOPHERS_COUNT);

	// Create threads
	for (int i = 0; i < PHILOSOPHERS_COUNT; i++) {
		philosophers[i] = std::thread(run, i);
	}

	for (auto& f : philosophers) {
		f.join();
	}

	delete[] forks;
	return 0;
}