#include<curl/curl.h>
#include<thread>
#include<mutex>
#include<queue>
#include<list>
#include<vector>
#include<condition_variable>
#include<memory>
#include"PCTNetworkItem.h"
#include"PCTSharedPtrGreater.h"

#ifndef _PCT_NETWORKWORKER_H_
#define _PCT_NETWORKWORKER_H_
#define PCT_MAX_COUNT_ITEMS 8

class PCTNetworkWorker
{
private:
    std::priority_queue<std::shared_ptr<PCTNetworkItem>, std::vector<std::shared_ptr<PCTNetworkItem>>, PCTSharedPtrGreater<PCTNetworkItem>> mItemsWaiting;
    std::list<std::pair<std::shared_ptr<PCTNetworkItem>, CURL*>> mItemsWorking;
    std::queue<CURL*> mHandles;
    CURLM* mMulti;
    std::thread mThread;
    std::mutex mMutexQueueItemsWaiting;
    std::condition_variable mCondVarItemsWaiting;
    bool mExit;

public:
    /**/
    PCTNetworkWorker();
    /**/
    ~PCTNetworkWorker();

    /**/
    void addNetworkItem(std::shared_ptr<PCTNetworkItem> item);
};

#endif
