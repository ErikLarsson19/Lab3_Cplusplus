#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
class Assignment1 {
public:
    void incrementVector(std::vector<int>& myVector) {
        for (int& element : myVector) {
            element++;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void incrementVectorRange(std::vector<int>& myVector, int start, int end, int increment) {
        for (int i = start; i < end; ++i) {
            myVector[i] += increment;
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
        }
    }
};

/*
* int main() {
    Assignment1 obj;
    std::vector<int> myVector(20, 0);

    std::cout << "before: ";
    for (const int& element : myVector) {
        std::cout << element << " ";
    }
    std::cout << "\n";


    std::thread worker(&Assignment1::incrementVector, &obj, std::ref(myVector));
    worker.join();


    std::cout << "After: ";
    for (const int& element : myVector) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    const int n = 5;
    std::vector<int> newVector(20, 0);
    std::vector<std::thread> workers;
    for (int i = 0; i < n; ++i) {
        workers.push_back(std::thread(&Assignment1::incrementVectorRange, &obj, std::ref(newVector), i * 5, (i + 1) * 5, i + 1)); //5 = elements per thread fix
    }
    for (std::thread& t : workers) {
        t.join();
    }
    std::cout << "After: ";
    for (const int& element : newVector) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    // still no shared resource, threads working on different parts of the vector

    return 0;


    //join is called on trhead worker, so the main thread will wait for the worker to finish
    //before it continues to print the vector
    //if we don't call join, the main thread will continue to print the vector while the worker is still running

    //each thread increments elements by an incremental value equal to the thread number


}
*/


