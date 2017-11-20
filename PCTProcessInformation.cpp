#include<boost/regex.hpp>
#include"PCTProcessInformation.h"
#include"PCTSingletonThreadWorker.h"
#include"PCTLoadCover.h"
#include"PCTSaveInformation.h"

PCTProcessInformation::PCTProcessInformation(PCTMovie movie, std::vector<char> infoMovie, std::shared_ptr<PCTCancellationToken> token, unsigned int indx, PCTCommon common, int priority)
    :PCTWorkItem(priority), mMovie(movie), mInfoMovie(infoMovie), mToken(token), mIndex(indx), mCommon(common)
{
    //NADA
}

void PCTProcessInformation::run()
{
    if (!mToken->isCancelled()) {
        boost::smatch match;
        std::string input (mInfoMovie.begin(), mInfoMovie.end());
        if (boost::regex_search(input, match, boost::regex("<link rel='image_src' href=\"(.*?)\">")) && match.size() > 0) {
            input = match.str(1);
            if (boost::regex_search(input, match, boost::regex("https(.*?)\\._V")) && match.size() > 0) {
                std::string url = "https" + match.str(1) + "._V1_UX182_CR0,0,182,268_AL_.jpg";
                if (!mToken->isCancelled()) {
                    mMovie.small_cover_image_url = url;
                    mMovie.medium_cover_image_url = url;
                    mMovie.large_cover_image_url = url;
                    std::shared_ptr<PCTLoadCover> task = std::make_shared<PCTLoadCover>(mMovie, mToken, mIndex, mCommon, 0);
                    PCTSingletonThreadWorker::getInstance().addWorkItem(std::move(task));
                }
                //GUARDAR AUNQUE ESTE CANCELLADO
                //LOS METADATOS TIENE QUE SER CORRECTOS
                std::vector<char> data(url.begin(), url.end());
                std::shared_ptr<PCTSaveInformation> task = std::make_shared<PCTSaveInformation>(mMovie.imdb_code, std::move(data), 2);
                PCTSingletonThreadWorker::getInstance().addWorkItem(std::move(task));
            }
        }
    }
}
