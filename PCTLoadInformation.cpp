#include"PCTLoadInformation.h"
#include"PCTSingletonCache.h"
#include"PCTSingletonNetworkWorker.h"
#include"PCTSingletonThreadWorker.h"
#include"PCTLoadCover.h"
#include"PCTDownloadInformation.h"

PCTLoadInformation::PCTLoadInformation(PCTMovie movie, std::shared_ptr<PCTCancellationToken> token, unsigned int indx, PCTCommon common, int priority)
    :PCTWorkItem(priority), mMovie(movie), mToken(token), mIndex(indx), mCommon(common)
{
    //NADA
}

void PCTLoadInformation::run()
{
    if (!mToken->isCancelled()) {
        std::vector<char> data;
        //TAL VEZ DEBERIA DISTINGUIR UN FALLO DE LA DB DE
        //LA AUSENCIA DE LA PELICULA
        if (PCTSingletonCache::getInstance().get(mMovie.imdb_code + "_INFO", data)) {
            //mToken->getMutex()->lock();
            if (!mToken->isCancelled()) {
                std::string url(data.begin(), data.end());
                mMovie.small_cover_image_url = url;
                mMovie.medium_cover_image_url = url;
                mMovie.large_cover_image_url = url;
                std::shared_ptr<PCTLoadCover> task = std::make_shared<PCTLoadCover>(mMovie, mToken, mIndex, mCommon, 0);
                PCTSingletonThreadWorker::getInstance().addWorkItem(std::move(task));
            }
            //mToken->getMutex()->unlock();
        } else {
            //mToken->getMutex()->lock();
            if (!mToken->isCancelled()) {
                //LE PASO EL IMDDB_CODE PORQUE DESPUES LO NECESITA
                //PARA GUARDAR LA INFORMACION EN LA DB
                std::shared_ptr<PCTDownloadInformation> task = std::make_shared<PCTDownloadInformation>(mMovie, mToken, mIndex, mCommon, -1);
                task->setURL("https://www.imdb.com/title/" + mMovie.imdb_code + "/mediaindex");
                PCTSingletonNetworkWorker::getInstance().addNetworkItem(std::move(task));
            }
            //mToken->getMutex()->unlock();
        }
    }
}
