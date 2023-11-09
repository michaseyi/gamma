#include "ThreadPool.h"
#include <variant>

ThreadPool::ThreadPool(int numThreads)
{
   for (int i{0}; i < numThreads; ++i)
   {
      workers.emplace_back([this]
                           {
         while (true) {
            std::function<void()> task;
            {
               std::unique_lock<std::mutex> lock{queueMutex};
               condition.wait(lock, [this] { return stop || !tasks.empty();});
               
               if (stop && tasks.empty()) {
                  return;
               }
               task = std::move(tasks.front());
               tasks.pop();
            }
            task();
         } });
   }
}

ThreadPool::~ThreadPool()
{
   {
      std::unique_lock<std::mutex> lock(queueMutex);
      stop = true;
   }
   condition.notify_all();
   for (std::thread &worker : workers)
   {
      worker.join();
   }
}
