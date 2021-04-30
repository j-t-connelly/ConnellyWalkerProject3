#include <iostream>
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
	unsigned long int sum = 0; //0 - 4,294,967,295 (could also use long long int, but 0-18,446,744,073,709,551,615 seemed like overkill)
	for (int i = 0; i <= num; i++) {
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
	lock.lock();
	std::cout << "Thread " << name << " Exiting." << std::endl;
	lock.unlock();
}

int main(int argc, char* [] argv)
{
	std::thread a(f, "A", );
	std::thread b(f, "B", );
	std::thread c(f, "C", );
	std::thread d(f, "D", );
}