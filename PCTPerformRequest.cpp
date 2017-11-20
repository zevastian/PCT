#include"PCTPerformRequest.h"
#include"PCTSingletonThreadWorker.h"
#include"PCTProcessRequest.h"

PCTPerformRequest::PCTPerformRequest(std::shared_ptr<PCTCancellationToken> token, PCTCommon common, int priority)
    :PCTHttpGetRequest(priority), mToken(token), mCommon(common)
{
    //NADA
}

bool PCTPerformRequest::onStart()
{
    return !mToken->isCancelled();
}

bool PCTPerformRequest::onWrite(std::vector<char> buffer)
{
    bool cancelled = mToken->isCancelled();
    if (!cancelled) {
        try {
            mRequestData.insert(mRequestData.end(), buffer.begin(), buffer.end());
        } catch(...) {
            cancelled = true;
        }
    }
    return !cancelled;
}

void PCTPerformRequest::onFinish(PCT_NETWORK_CODE code)
{
    //mToken->getMutex()->lock();
    if (!mToken->isCancelled()) {
        //SI FALLA DEBERIA NOTIFICARSELO AL USUARIO
        //EN ALGUNOS CASO (YTS CAIDO Y CLOUDFARE HACIENDO DE ESPEJO)
        //CODE ERA PCT_NET_SUCCESS PERO HABIA UN ERROR 520 (MEDIANTE WGET)
        //COMO NO SE ESTABA CONTEMPLANDO FALLABA EL PARSEO
        if (code == PCT_NET_SUCCESS) {
            //JANSSON FALLA SI EL STRING NO TERMINA EN CERO
            mRequestData.push_back(0);
            std::shared_ptr<PCTProcessRequest> task = std::make_shared<PCTProcessRequest>(std::move(mRequestData), mToken, mCommon, -2);
            PCTSingletonThreadWorker::getInstance().addWorkItem(std::move(task));
        }
    }
    //mToken->getMutex()->unlock();
}
