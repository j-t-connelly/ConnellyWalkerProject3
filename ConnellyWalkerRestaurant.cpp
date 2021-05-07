/*
James Connelly & Billy Walker
Doctor Heinold
CMSCI 356
Project 3
Problem 2
*/

#include <thread>
#include <chrono> //sleeping
#include <iostream>
#include <random>
#include <string> // always imported just in case
#include <mutex> // locks
#include <condition_variable>

std::mutex lock;
std::condition_variable waiting_room;
int timeCounter = 0;
const double ranges[24] = { 5, 5, 7, 10, 8, 5, 3, 2, 1, .5, 1, 2, 2, 3, 4, 3, 1, .25, .5, 2, 3, 4, 4, 5 };
int customerNumber = 1;
bool open = true;
const uint8_t CAPACITY = 7;
int count = 0; // will keep track of how many customers are available, if more cutomers than CAPACITY, incoming customers leave



void timeCounterKeeper()
{
	while (open) {
		if (timeCounter == 24 || timeCounter == 0) {
			// Back to midnight
			lock.lock();
			timeCounter = 0;
			std::cout << "\n**** It's now midnight. ****\n" << std::endl;
			lock.unlock();
		}
		else if (timeCounter == 12) {
			// high noon
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
			// 1 - 11 AM
			lock.lock();
			std::cout << "\n**** It's now " << timeCounter << " am. ****\n" << std::endl;
			lock.unlock();
		}
		timeCounter++;
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}

void server()
{
	while (open) {
		std::unique_lock<std::mutex> noCheck(lock);
		waiting_room.wait(noCheck, []() {return count == 0; });
		std::cout << "@@ Server is on break. @@" << std::endl;
		noCheck.unlock();
		std::unique_lock<std::mutex> placeCheck(lock);
		waiting_room.wait(placeCheck, []() {return count != 0; });
		std::cout << "@@ Server is back. @@" << std::endl;
		placeCheck.unlock();
	}
}

void customer(int cus)
{
	if (count == 7) {
		lock.lock();
		std::cout << "Customer #" << cus << " enters.  ";
		std::cout << "There are " << count << " customers already here. ";
		std::cout << "TOO BUSY! I'm leaving!" << std::endl;
		lock.unlock();
	}
	else {
		bool flag = false;
		lock.lock();
		//unfortunately due to all of the prints this has to be locked down like this
		std::cout << "Customer #" << cus << " enters.  ";
		if (count == 0) {
			std::cout << "The place is empty";
		}
		else if (count == 1) {
			std::cout << "There is 1 customer already here.";
		}
		else {
			std::cout << "There are " << count << " customers already here.";
		}
		if (count >= 6) {
			flag = true;
			std::cout << "I'll wait for a seat.";
		}
		count++;
		std::cout << std::endl;
		lock.unlock();
		if (flag) {
			std::unique_lock<std::mutex> caseCheck(lock);
			waiting_room.wait(caseCheck, []() {return count <= 5; });
			caseCheck.unlock();
		}
		lock.lock();
		std::cout << "Customer #" << cus << " seated. There ";
		if (count == 1)
			std::cout << "is 1 customer in the restaurant.";
		else
			std::cout << "are " << count << " customers in the restaurant.";
		std::cout << std::endl;
		lock.unlock();
		std::uniform_int_distribution<> unif(1, 5);
		std::default_random_engine re;
		std::this_thread::sleep_for(std::chrono::seconds(unif(re)));
		lock.lock();
		count--;
		std::cout << "Customer #" << cus << " leaves.  There";
		if (count == 1)
			std::cout << " is 1 customer now.";
		else
			std::cout << " are " << count << " customers now.";
		std::cout << std::endl;
		lock.unlock();
	}
	waiting_room.notify_all();
}

int main(int argc, char** argv)
{
	std::thread timer(timeCounterKeeper);
	std::thread serverWorker(server);
	std::vector<std::thread> customerThreads; // To store threads, otherwise bad things happen when one goes rogue
	while (open) {
		double upper_bound = ranges[timeCounter];
		double sleepDuration = (double)(std::rand()) / ((double)(RAND_MAX/upper_bound)); // unif is a random number from 0 to ranges[timeCounter] chosen at random
		std::this_thread::sleep_for(std::chrono::duration<double>(sleepDuration)); // Sleep for this long before a customer enters
		customerThreads.push_back(std::thread(customer, customerNumber));
		customerNumber++;
	}
	timer.join();
	serverWorker.join();
	
}
