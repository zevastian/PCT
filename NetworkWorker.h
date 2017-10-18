#include<curl/curl.h>
#include<thread>
#include<mutex>
#include<queue>
#include<list>
#include<vector>
#include<condition_variable>
#include<memory>
#include"NetworkItem.h"

#ifndef _NETWORKWORKER_H_
#define _NETWORKWORKER_H_
#define MAX_HANDLES 8

class PCTNetworkWorker
{
private:
    class PCTItemComparator
    {
    public:
        bool operator()(std::shared_ptr<PCTNetworkItem>& item1, std::shared_ptr<PCTNetworkItem>& item2);
    };

    std::priority_queue<std::shared_ptr<PCTNetworkItem>, std::vector<std::shared_ptr<PCTNetworkItem>>, PCTItemComparator> mItemsWaiting;
    std::list<std::pair<std::shared_ptr<PCTNetworkItem>, CURL*>> mItemsWorking;
    std::queue<CURL*> mCurlHandles;
    CURLM* mMultiHandle;
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
