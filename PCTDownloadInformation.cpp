#include"PCTDownloadInformation.h"
#include"PCTSingletonThreadWorker.h"
#include"PCTSingletonCache.h"
#include"PCTProcessInformation.h"

PCTDownloadInformation::PCTDownloadInformation(PCTMovie movie, std::shared_ptr<PCTCancellationToken> token, unsigned int indx, PCTCommon common, int priority)
    :PCTHttpGetRequest(priority), mMovie(movie), mToken(token), mIndx(indx), mCommon(common)
{
    //NADA
}

bool PCTDownloadInformation::onStart()
{
    return !mToken->isCancelled();
}

bool PCTDownloadInformation::onWrite(std::vector<char> buffer)
{
    bool cancelled = mToken->isCancelled();
    if (!cancelled) {
        try {
            mInforMovie.insert(mInforMovie.end(), buffer.begin(), buffer.end());
        } catch(...) {
            cancelled = true;
        }
    }
    return !cancelled;
}

void PCTDownloadInformation::onFinish(PCT_NETWORK_CODE code)
{
    if (code == PCT_NET_SUCCESS) {
        //mToken->getMutex()->lock();
        if (!mToken->isCancelled()) {
            std::shared_ptr<PCTProcessInformation> task = std::make_shared<PCTProcessInformation>(mMovie, std::move(mInforMovie), mToken, mIndx, mCommon, -1);
            PCTSingletonThreadWorker::getInstance().addWorkItem(std::move(task));
        }
        //mToken->getMutex()->unlock();
    }
}
