#include <thread>
#include <chrono> //sleeping
#include <iostream>
#include <random>
#include <string> // always imported just in case
#include <mutex> // locks
#include <condition_variable>

std::mutex lock;
std::condition_variable cv;
int timeCounter = 0;
const double ranges[24] = { 5, 5, 7, 10, 8, 5, 3, 2, 1, .5, 1, 2, 2, 3, 4, 3, 1, .25, .5, 2, 3, 4, 4, 5 };
int customerNumber = 1;
const uint8_t WAITING_ROOM = 2;
const uint8_t TABLES = 5; // Total number of tables
uint8_t count = 0; // will keep track of how many customers are available, if more cutomers than TABLES + WAITING_ROOM, incoming customers leave



void timeCounterKeeper()
{
	while (true) {
		if (timeCounter == 0) {
			lock.lock();
			std::cout << "**** It's now midnight.****\n" << std::endl;
			lock.unlock();
		}
		std::this_thread::sleep_for(std::chrono::seconds(5));
		timeCounter++;
		if (timeCounter == 24) {
			timeCounter = 0;
			lock.lock();
			std::cout << "\n**** It's now midnight. ****\n" << std::endl;
			lock.unlock();
		}
		else if (timeCounter == 12) {
			lock.lock();
			std::cout << "\n**** It's now noon. ****\n" << std::endl;
			lock.unlock();
		}
		else if (timeCounter > 12) {
			// AFTERNOON
			int actual = timeCounter % 12;
			lock.lock();
			std::cout << "\n**** It's now " << actual << " pm. ****\n" << std::endl;
			lock.unlock();
		}
		else {
			lock.lock();
			std::cout << "\n**** It's now " << timeCounter << " am. ****\n" << std::endl;
			lock.unlock();
		}
	}
}

void server()
{
}

void customer()
{
}

int main(int argc, char** argv)
{
	std::thread timer(timeCounterKeeper);
	//std::thread server(server);
	timer.join();
}
