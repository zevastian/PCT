#include"PCTCancellationToken.h"

PCTCancellationToken::PCTCancellationToken()
    :mValue(false), mMutex(std::make_shared<std::mutex>())
{
    //NADA
}

void PCTCancellationToken::cancel()
{
    mMutex->lock();
    mValue = true;
    mMutex->unlock();
}

bool PCTCancellationToken::isCancelled()
{
    return mValue;
}

std::shared_ptr<std::mutex> PCTCancellationToken::getMutex()
{
    return mMutex;
}
