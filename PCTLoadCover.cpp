#include"PCTLoadCover.h"
#include"PCTSingletonCache.h"
#include"PCTSingletonNetworkWorker.h"
#include"PCTSingletonThreadWorker.h"
#include"PCTDecodeCover.h"
#include"PCTDownloadCover.h"

PCTLoadCover::PCTLoadCover(PCTMovie movie, std::shared_ptr<PCTCancellationToken> token, unsigned int indx, PCTCommon common, int priority)
    :PCTWorkItem(priority), mMovie(movie), mToken(token), mIndex(indx), mCommon(common)
{
    //NADA
}

void PCTLoadCover::run()
{
    if (!mToken->isCancelled()) {
        std::vector<char> data;
        //TAL VEZ DEBERIA DISTINGUIR UN FALLO DE LA DB DE
        //LA AUSENCIA DE LA PELICULA
        if (PCTSingletonCache::getInstance().get(mMovie.imdb_code + "_COVER", data)) {
            //mToken->getMutex()->lock();
            if (!mToken->isCancelled()) {
                std::shared_ptr<PCTDecodeCover> task = std::make_shared<PCTDecodeCover>(std::move(data), mToken, mIndex, mCommon, -1);
                PCTSingletonThreadWorker::getInstance().addWorkItem(std::move(task));
            }
            //mToken->getMutex()->unlock();
        } else {
            //mToken->getMutex()->lock();
            if (!mToken->isCancelled()) {
                //LE PASO EL IMDDB_CODE PORQUE DESPUES LO NECESITA
                //PARA GUARDAR EL COVER EN LA DB
                std::shared_ptr<PCTDownloadCover> task = std::make_shared<PCTDownloadCover>(mMovie.imdb_code, mToken, mIndex, mCommon, -2);
                task->setURL(mMovie.medium_cover_image_url);
                PCTSingletonNetworkWorker::getInstance().addNetworkItem(std::move(task));
            }
            //mToken->getMutex()->unlock();
        }
    }
}
