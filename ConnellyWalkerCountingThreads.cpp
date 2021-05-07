/*
James Connelly & Billy Walker
Doctor Heinold
CMSCI 356
Project 3
Problem 1
*/

#include <iostream>
#include <thread>
#include <string>
#include <mutex> //locks
#include <condition_variable>

std::mutex lock;
std::condition_variable cv;
const uint8_t THREAD_COUNT = 4;// smaller is better
uint8_t count = 0;

void f(std::string name, long num) {
	lock.lock();
	std::cout << "Thread " << name << " Started." << std::endl;
	lock.unlock();
	unsigned long long sum = 0; 
	for (unsigned long long i = 1; i <= num; i++) {
		lock.lock();
		sum += i;
		lock.unlock();
	}
	lock.lock();
	std::cout << "Thread " << name << " done counting. Got " << sum << "." << std::endl;
	lock.unlock();
	std::unique_lock<std::mutex> caseCheck(lock); // from what I read it has to be this way, even though the other lock is used the entire time, this way it doesn't break the conditional variable
	count++;
	caseCheck.unlock();
	cv.notify_all();
	caseCheck.lock();
	cv.wait(caseCheck, []() {return count == THREAD_COUNT; });
	std::cout << "Thread " << name << " Exiting." << std::endl;
	caseCheck.unlock();
}

int main(int argc, char** argv)
{
	std::thread a(f, "A", 1000000);
	std::thread b(f, "B", 2000000);
	std::thread c(f, "C", 4000000);
	std::thread d(f, "D", 6000000);

	//TODO: See if there is a way to do this without calling join()
	a.join();
	b.join();
	c.join();
	d.join();
	// If you do not join at the end, it will try to explode since there is nothing else in main... 
}