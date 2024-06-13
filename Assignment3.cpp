#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <fstream>
#include <filesystem>


class Assignment3 {
public:

	std::vector<std::string> readBibleText(const std::string& filename) {
		std::vector<std::string> lines;
		std::ifstream file(filename);
		std::string line;
		while (std::getline(file, line)) {
			lines.push_back(line);
		}
		return lines;
	}
	int totalMatches = 0;
	double totalExecutionTime = 0.0;
	std::mutex coutMutex;
	std::mutex counterMutex;

	void searchInLines(const std::vector<std::string>& lines, const std::string& searchTerm, int start, int end) {
		int count = 0;
		auto startTime = std::chrono::high_resolution_clock::now();

		for (int i = start; i < end; i++) {
			if (lines[i].find(searchTerm) != std::string::npos) {
				count++;
			}
		}

		auto endTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> executionTime = endTime - startTime;

		{
			std::lock_guard<std::mutex> guard(counterMutex);
			totalMatches += count;
			totalExecutionTime += executionTime.count();
		}

		{
			std::lock_guard<std::mutex> guard(coutMutex);
			std::cout << "Thread ID: " << std::this_thread::get_id()
				<< " found " << count << " instances of " << searchTerm << " in " << executionTime.count() << "ms" << std::endl;
		}
	}



};

/*
int main() {


	Assignment3 obj;
	//the resource file containt my bible.txt how do i load it below?
	auto lines = obj.readBibleText("./bible.txt");

	const std::string searchTerm = "God";
	const int N = std::thread::hardware_concurrency(); // Or any other number you choose
	std::vector<std::thread> threads;

	int linesPerThread = lines.size() / N;
	for (int i = 0; i < N; ++i) {
		int start = i * linesPerThread;
		int end = (i + 1) * linesPerThread;
		if (i == N - 1) {
			end = lines.size(); // Ensure the last thread covers the rest
		}
		threads.emplace_back(&Assignment3::searchInLines, &obj, std::cref(lines), searchTerm, start, end);
	}

	for (auto& thread : threads) {
		thread.join();
	}

	std::cout << "Total matches: " << obj.totalMatches << std::endl;
	std::cout << "Total execution time: " << obj.totalExecutionTime << "ms" << std::endl;

	return 0;


	// total matches and total execution time are shared between threads
	//Std::cout is the shared resource
	//Increasing N will increase the number of threads and the number of lines each thread will search, but it will also increase the overhead of creating and managing threads.
	//Ideal number is often the the same number of hardware threads or logical cores available on the CPU. Maximum parallelism.
	//N exceeds the number of hardware threads = performence will degrade due to context switching and overhead of managing threads.
	// hyper thread = two or more threads simultaneously share the same core.

}
*/



