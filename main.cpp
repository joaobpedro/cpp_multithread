#include <cstdio>
#include <iostream>
#include <chrono>
#include <mutex>
#include "thread.h"


std::mutex print_mutex;

enum StateMachine {
    NO_RUN,
    FIRST_RUN,
    SECOND_RUN,
};


void modify_element(int& element, int index) {
    // Simulate some heavy work
    element += 10;
}

void wrapper(std::vector<int> &my_data, ThreadPool &pool) {
    for (size_t i = 0; i < my_data.size(); ++i) {
        pool.QueueJob([&my_data, i]() {
            modify_element(my_data[i], i);
        });
    }
}

int main() {

    StateMachine state = StateMachine::NO_RUN;
    std::vector<int> my_data = {1, 2, 3, 4, 5, 6, 7, 8};
    
    ThreadPool pool;
    pool.Start(2); 

    ThreadPool pool_2;
    pool_2.Start();

    // need to wait for the threads to finish
    if (state == StateMachine::NO_RUN) {
        for (size_t i = 0; i < my_data.size(); ++i) {
            pool.QueueJob([&my_data, i]() {
                modify_element(my_data[i], i);
            });
        }
        state = StateMachine::FIRST_RUN;
    }


    while (pool.busy()) {
        printf("First wait\n");
        //wait
    }

    pool.QueueJob([&my_data, &pool_2]() {
        wrapper(my_data, pool_2);
    });

    while (pool.busy() || pool_2.busy()) {
        printf("Second wait\n");
    }

    pool.Stop();
    pool_2.Stop();

    std::cout << "Final data: ";
    for (int val : my_data) {
        std::cout << val << " ";
    }
    
    std::cout << std::endl;

    return 0;
}


#include "thread.cpp"