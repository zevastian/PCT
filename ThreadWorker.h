#include<queue>
#include<memory>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<thread>
#include"WorkItem.h"

#ifndef _THREADWORKER_H_
#define _THREADWORKER_H_

class PCTThreadWorker
{
private:
    std::queue<std::shared_ptr<PCTWorkItem>> mQueueItem;
    std::mutex mMutexCondVar;
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
