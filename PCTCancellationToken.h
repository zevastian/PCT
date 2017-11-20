#include<mutex>
#include<memory>
#include<atomic>

#ifndef _PCT_CANCELLATIONTOKEN_H_
#define _PCT_CANCELLATIONTOKEN_H_

class PCTCancellationToken
{
private:
    std::atomic<bool> mValue;
    std::shared_ptr<std::mutex> mMutex;

public:
    /**/
    PCTCancellationToken();

    /**/
    void cancel();
    /**/
    bool isCancelled();
    /**/
    std::shared_ptr<std::mutex> getMutex();
};

#endif
