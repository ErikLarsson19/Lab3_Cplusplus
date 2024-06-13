#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <fstream>
#include <queue>
#include <condition_variable>
#include <random>

class Assignment4 {
public:
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_int_distribution<> dis;

   
	struct Order {
		int id, processingTime;
		Order(int id, int processingTime) : id(id), processingTime(processingTime) {};
	};


	std::queue<Order> orderQueue;
	std::mutex queueMutex;
	std::condition_variable queueCondVar;
	std::mutex printMutex;
	const int maxQueueSize = 5;
    bool allOrdersPlaced = false;
    //random time order between 1-5 secs
	Assignment4() : gen(rd()), dis(1, 5) {};

    void customer(int customerId) {
        //accidental race condition here before queue.empty() in the while loop caused it
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate time between orders

            int processingTime = dis(gen);
            Order order(customerId, processingTime);

            {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueCondVar.wait(lock, [&] { return orderQueue.size() < maxQueueSize; });

                orderQueue.push(order);
                print("Customer " + std::to_string(customerId) + " placed order: " + std::to_string(order.id));
            }
            queueCondVar.notify_all();
        }
    }


    void barista(int baristaId) {
        while (true) {
            Order order(0, 0); // Initialize order to a default state
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueCondVar.wait(lock, [&] { return !orderQueue.empty() || allOrdersPlaced; });

                
                // Check if all orders are placed and the queue is empty, then exit
                /*
                if (orderQueue.empty() && allOrdersPlaced) {
                    break; // Exit the loop if no more orders will be placed and queue is empty
                }
                */
                

                // Get the next order from the queue
                order = orderQueue.front();
                orderQueue.pop();

                // Print message about starting order
                print("Barista " + std::to_string(baristaId) + " started order: " + std::to_string(order.id));
            }
            // Notify potentially waiting customer threads (if the queue was full)
            queueCondVar.notify_all();

            // Process order
            for (int progress = 0; progress <= 100; progress += 20) {
                // Simulate processing time based on order's processingTime
                std::this_thread::sleep_for(std::chrono::milliseconds(order.processingTime * progress / 100));

                // Print progress
                print("Barista " + std::to_string(baristaId) + " - Order " + std::to_string(order.id) + " progress: " + std::to_string(progress) + "%");
            }
        }
    }


    void runSimulation(int numCustomers, int numBaristas) {
        std::vector<std::thread> customerThreads;
        std::vector<std::thread> baristaThreads;

        // Launch customer threads
        for (int i = 1; i <= numCustomers; ++i) {
            customerThreads.emplace_back(&Assignment4::customer, this, i);
        }

        // Launch barista threads simultaneously
        for (int i = 1; i <= numBaristas; ++i) {
            baristaThreads.emplace_back(&Assignment4::barista, this, i);
        }

        // Wait for all customer threads to finish
        for (auto& t : customerThreads) {
            t.join();
        }

        // After all customer threads have finished, signal that all orders have been placed
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            allOrdersPlaced = true;
        }
        queueCondVar.notify_all(); // Notify waiting baristas

        // Join barista threads
        for (auto& t : baristaThreads) {
            t.join();
        }
    }


    void print(const std::string& message) {
        std::lock_guard<std::mutex> printLock(printMutex);
        std::cout << message << std::endl;
    }


};


int main() {
    Assignment4 a;
    a.runSimulation(10, 3);
    return 0;
}

