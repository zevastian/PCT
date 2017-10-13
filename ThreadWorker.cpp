#include"ThreadWorker.h"

PCTThreadWorker::PCTThreadWorker()
    :mExit(false)
{
    unsigned int num = std::thread::hardware_concurrency();
    if (num > 2) {
        num--;
    } else {
        num = 2;
    }

    for (unsigned int i = 0; i < num; i++) {

        mThreads.push_back(std::thread([&] {
            std::shared_ptr<PCTWorkItem> item;
            while(!mExit)
            {
                std::unique_lock<std::mutex> lock(mMutexCondVar);
                while(mQueueItem.empty() && !mExit) {
                    mCondVarWaitItem.wait(lock);
                }

                while (!mQueueItem.empty()) {
                    item = mQueueItem.front();
                    mQueueItem.pop();
                    mMutexCondVar.unlock();
                    item->run();
                    mMutexCondVar.lock();
                }
            }
        }));
    }
}

PCTThreadWorker::~PCTThreadWorker()
{
    mMutexCondVar.lock();
    mExit = true;
    mMutexCondVar.unlock();
    mCondVarWaitItem.notify_all();

    for (auto it = mThreads.begin(); it != mThreads.end(); it++) {
        if (it->joinable()) {
            it->join();
        }
    }
}

void PCTThreadWorker::addWorkItem(std::shared_ptr<PCTWorkItem> item)
{
    mMutexCondVar.lock();
    mQueueItem.push(item);
    mMutexCondVar.unlock();
    mCondVarWaitItem.notify_one();
}
