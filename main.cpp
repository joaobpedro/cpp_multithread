#include <iostream>
#include <chrono>
#include <mutex>
#include "thread.h"

std::mutex print_mutex;

void modify_element(int& element, int index) {
    // Simulate some heavy work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    element += 10;
    
    // Lock before printing so threads don't overwrite each other's text
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "Job for index " << index << " finished." << std::endl;
}

int main() {
    std::vector<int> my_data = {1, 2, 3, 4, 5, 6, 7, 8};
    
    ThreadPool pool;
    pool.Start(); // Create the worker threads

    // 1. Queue up the jobs
    for (size_t i = 0; i < my_data.size(); ++i) {
        // We capture `i` by value, and a reference to the specific element
        pool.QueueJob([&my_data, i]() {
            modify_element(my_data[i], i);
        });
    }

    // 2. Wait for jobs to finish
    // Since this implementation doesn't return futures, a common way 
    // to wait for the queue to empty is to poll the busy() method.
    while (pool.busy()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // 3. Stop the pool
    pool.Stop();

    // 4. Verify results
    std::cout << "Final data: ";
    for (int val : my_data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
