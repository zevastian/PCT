#include"PCTThreadWorker.h"

PCTThreadWorker::PCTThreadWorker()
    :mExit(false)
{
    mNumThread = std::thread::hardware_concurrency();
    if (!mNumThread) {
        mNumThread = 1;
    }

    for (unsigned int i = 0; i < mNumThread; i++) {
        mThreads.push_back(std::thread([&] {
            bool exit = false;
            do {
                std::unique_lock<std::mutex> lock(mMutexQueueItem);
                while(mQueueItem.empty() && !mExit)
                {
                    mCondVarWaitItem.wait(lock);
                }

                if (!mQueueItem.empty())
                {
                    do {
                        auto item = std::move(mQueueItem.top());
                        mQueueItem.pop();
                        mMutexQueueItem.unlock();
                        item->run();
                        mMutexQueueItem.lock();
                    } while (!mQueueItem.empty());
                } else if (mExit)
                {
                    exit = true;
                }
            } while(!exit);
        }));
    }
}

PCTThreadWorker::~PCTThreadWorker()
{
    mMutexQueueItem.lock();
    mExit = true;
    mMutexQueueItem.unlock();
    mCondVarWaitItem.notify_all();

    for (auto it = mThreads.begin(); it != mThreads.end(); it++) {
        if (it->joinable()) {
            it->join();
        }
    }
}

void PCTThreadWorker::addWorkItem(std::shared_ptr<PCTWorkItem> item)
{
    mMutexQueueItem.lock();
    mQueueItem.push(std::move(item));
    mMutexQueueItem.unlock();
    mCondVarWaitItem.notify_one();
}

unsigned int PCTThreadWorker::getConcurrency()
{
    return mNumThread;
}
