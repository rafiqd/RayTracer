//
// Created by Rafiq on 2018-09-22.
//
#include <iostream>
#include <mutex>
#include <vector>
#include <unistd.h>
#include "parallel.h"
#include "main.h"

static bool shutdownThreads = false;
static std::mutex workListMutex;
static std::condition_variable workListCondition;
class ParallelForLoop;
static ParallelForLoop *workList = nullptr;
static std::vector<std::thread> threads;
static std::vector<int> seeds;
thread_local int ThreadIndex;
thread_local RNG gen;
class ParallelForLoop {
public:
    std::function<void(int)> func;

    const int maxIndex;
    const int chunkSize;
    int nextIndex = 0;
    int activeWorkers = 0;
    ParallelForLoop *next = nullptr;
    int nX = -1;

    ParallelForLoop(const std::function<void(int)> _func, int _maxIndex, int _chunkSize)
            : func(_func), maxIndex(_maxIndex), chunkSize(_chunkSize) {}

    bool Finished() const {
        return nextIndex >= maxIndex && activeWorkers == 0;
    }
};


static void workerThreadFunc(int tIndex){
    ThreadIndex = tIndex;
    RNG gen(seeds[tIndex]);

    std::unique_lock<std::mutex> lock(workListMutex);
    while(!shutdownThreads){
        if (workList) {
            ParallelForLoop &loop = *workList;
            int indexStart = loop.nextIndex;
            int indexEnd = std::min(indexStart + loop.chunkSize, loop.maxIndex);

            loop.nextIndex = indexEnd;
            if (loop.nextIndex == loop.maxIndex){
                workList = loop.next;
            }
            loop.activeWorkers++;
            std::cout << "worker " << tIndex << " working on [" << indexStart << ", " << indexEnd << "]" << std::endl;
            lock.unlock();
            for (int index = indexStart; index < indexEnd; ++index){
                loop.func(index);
            }

            lock.lock();
            workedon[tIndex] += 1;
            loop.activeWorkers--;
            if (loop.Finished()){
                workListCondition.notify_all();
            }
        } else {
            workListCondition.wait(lock);
        }
    }
}

void ParallelFor(std::function<void(int)> func, int count, int chunkSize){
    if (threads.empty() || count < chunkSize){
        for(int i = 0; i < count; ++i){
            func(i);
        }
        return;
    }

    RNG gen(seeds[0]);
    ParallelForLoop loop(std::move(func), count, chunkSize);
    workListMutex.lock();
    loop.next = workList;
    workList = &loop;
    workListMutex.unlock();

    std::unique_lock<std::mutex> lock(workListMutex);
    workListCondition.notify_all();

    while(!loop.Finished()){
        int indexStart = loop.nextIndex;
        int indexEnd = std::min(indexStart + loop.chunkSize, loop.maxIndex);

        loop.nextIndex = indexEnd;
        if(loop.nextIndex == loop.maxIndex) workList = loop.next;
        loop.activeWorkers++;
        if (indexStart != indexEnd){
            std::cout << "worker " << 0 << " working on [" << indexStart << ", " << indexEnd << "]" << std::endl;
            workedon[0] += 1;
        }
        lock.unlock();
        for (int index = indexStart; index < indexEnd; ++index){
            loop.func(index);
        }
        lock.lock();

        loop.activeWorkers--;
    }

}

int NumSystemCores() {
    return std::max(1u, std::thread::hardware_concurrency());
}

void ParallelInit(){
    int nThreads = NumSystemCores();
    ThreadIndex = 0;
    for (int i = 0; i < nThreads - 1; ++i){
        seeds.push_back(i);
        threads.push_back(std::thread(workerThreadFunc, i+1));
    }
}

void ParallelCleanup(){
    if (threads.empty())
        return;
    {
        std::lock_guard<std::mutex> lock(workListMutex);
        shutdownThreads = true;
        workListCondition.notify_all();
    }

    for (std::thread &thread : threads) thread.join();
    threads.erase(threads.begin(), threads.end());
    shutdownThreads = false;
}
