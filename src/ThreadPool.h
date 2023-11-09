#include <future>
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

#ifndef __THREADPOOL__
#define __THREADPOOL__

class ThreadPool
{
private:
   bool stop;
   std::vector<std::thread> workers;
   std::queue<std::function<void()>> tasks;
   std::mutex queueMutex;
   std::condition_variable condition;

public:
   ThreadPool(int numThreads);
   ~ThreadPool();
   template <class F>
   void AddTask(F &&f)
   {
      {
         std::unique_lock<std::mutex> lock(queueMutex);
         tasks.emplace(std::forward<F>(f));
      }
      condition.notify_one();
   }
};

#endif