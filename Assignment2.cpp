#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
class Assignment2 {
public:
	static std::mutex myMutex;

	void printMessage(int id) {
		myMutex.lock();
		std::cout << "Hello from thread: " << id << std::endl;
		myMutex.unlock();
	}

	void printMessageLock(int id) {
		std::lock_guard<std::mutex> guard(myMutex);
		std::cout << "Hello from thread: " << id << std::endl;
		//automatically unlocks when out of scope
		//no failing to unlock due to exceptions
	}
};

//Assign mutex here, why?
std::mutex Assignment2::myMutex;


/*
int main() {
	std::vector<std::thread> threads;
	Assignment2 obj;

	//emplace_back is used to add a new element to the end of the vector
	//adding threads to the vector
	//creating 10 threads
	for (int i = 0; i < 10; i++) {
		threads.emplace_back(&Assignment2::printMessageLock,&obj, i);
	}

	//join the threads with the main thread
	for (auto& thread : threads) {
		thread.join();
	}

	return 0;

	//The output is a complete mess and inconsistent. The threads are not synchronized and the output is not consistent.
	//Speed of threads? Randomness of threads?
	// what is the shared resource here?
	//The shared resource is the console output.

}
*/




	


