#include<queue>
#include<memory>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<thread>
#include"PCTWorkItem.h"
#include"PCTSharedPtrGreater.h"

#ifndef _PCT_THREADWORKER_H_
#define _PCT_THREADWORKER_H_

class PCTThreadWorker
{
private:
    std::priority_queue<std::shared_ptr<PCTWorkItem>, std::vector<std::shared_ptr<PCTWorkItem>>, PCTSharedPtrGreater<PCTWorkItem>> mQueueItem;
    std::mutex mMutexQueueItem;
    std::condition_variable mCondVarWaitItem;
    std::vector<std::thread> mThreads;
    bool mExit;

public:
    /**/
    PCTThreadWorker();
    /**/
    ~PCTThreadWorker();

    /**/
    void addWorkItem(std::shared_ptr<PCTWorkItem> item);
};

#endif
