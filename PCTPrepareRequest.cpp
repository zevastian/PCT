#include"PCTPrepareRequest.h"
#include"PCTSingletonNetworkWorker.h"
#include"PCTSingletonApi.h"
#include"PCTPerformRequest.h"

PCTPrepareRequest::PCTPrepareRequest(std::shared_ptr<PCTCancellationToken> token, PCTCommon common, int priority)
    :PCTWorkItem(priority), mToken(token), mCommon(common)
{
    //NADA
}

void PCTPrepareRequest::run()
{
    if (!mToken->isCancelled()) {
        std::string url;
        //NO ESTOY CONTROLANDO ERRORES
        if (PCTSingletonApi::getInstance().prepare(url) == PCT_APICODE::PCT_API_HAS_MORE) {
            //mToken->getMutex()->lock();
            if (!mToken->isCancelled()) {
                std::shared_ptr<PCTPerformRequest> task = std::make_shared<PCTPerformRequest>(mToken, mCommon, -3);
                task->setURL(url);
                PCTSingletonNetworkWorker::getInstance().addNetworkItem(std::move(task));
            }
            //mToken->getMutex()->unlock();
        }
    }
}
