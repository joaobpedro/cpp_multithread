#pragma once

#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
    void Start();
    void Start(int num_of_threads);
    void QueueJob(const std::function<void()>& job);
    void Stop();
    bool busy();

private:
    void ThreadLoop();

    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
    std::atomic<int> active_threads{0};      // this store how many threads are working at the momment
};