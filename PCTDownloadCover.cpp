#include"PCTDownloadCover.h"
#include"PCTSingletonThreadWorker.h"
#include"PCTSingletonCache.h"
#include"PCTDecodeCover.h"
#include"PCTSaveCover.h"

PCTDownloadCover::PCTDownloadCover(std::string imdbCode, std::shared_ptr<PCTCancellationToken> token, unsigned int indx, PCTCommon common, int priority)
    :PCTHttpGetRequest(priority), mImdbCode(imdbCode), mToken(token), mIndx(indx), mCommon(common)
{
    //NADA
}

bool PCTDownloadCover::onStart()
{
    return !mToken->isCancelled();
}

bool PCTDownloadCover::onWrite(std::vector<char> buffer)
{
    bool cancelled = mToken->isCancelled();
    if (!cancelled) {
        try {
            mCoverData.insert(mCoverData.end(), buffer.begin(), buffer.end());
        } catch(...) {
            cancelled = true;
        }
    }
    return !cancelled;
}

void PCTDownloadCover::onFinish(PCT_NETWORK_CODE code)
{
    if (code == PCT_NET_SUCCESS) {
        //mToken->getMutex()->lock();
        if (!mToken->isCancelled()) {
            //SI USO UN STD::MOVE ACA LUEGO CUANDO LO QUIERA USAR PARA GUARDAR VA ESTAR VACIO
            std::shared_ptr<PCTDecodeCover> taskDecode = std::make_shared<PCTDecodeCover>(mCoverData, mToken, mIndx, mCommon, -1);
            PCTSingletonThreadWorker::getInstance().addWorkItem(std::move(taskDecode));
        }
        //mToken->getMutex()->unlock();
        //GUARDAR AUNQUE ESTE CANCELLADO
        //ESTO TIENE UN PROBLEMA, PUEDE QUE EL COVER
        //NO PUEDA SER DECODIFICADO E IGUAL SE VA A GUARDAR
        std::shared_ptr<PCTSaveCover> taskSave = std::make_shared<PCTSaveCover>(mImdbCode, std::move(mCoverData), 2);
        PCTSingletonThreadWorker::getInstance().addWorkItem(std::move(taskSave));
    }
}
