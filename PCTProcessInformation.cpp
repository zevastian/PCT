#include<string>
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

            bool ux = false;
            if (boost::regex_search(input, match, boost::regex("_UX[0-9]{1,4}")) && match.size() > 0) {
                ux = true;
            }
            bool uy = false;
            if (boost::regex_search(input, match, boost::regex("_UY[0-9]{1,4}")) && match.size() > 0) {
                uy = true;
            }
            int crx = 0;
            int cry = 0;
            if (boost::regex_search(input, match, boost::regex("_CR[0-9]{1,4},[0-9]{1,4}")) && match.size() > 0) {
                std::string input = match.str(0);
                if (boost::regex_search(input, match, boost::regex("_CR(.*?),")) && match.size() > 0) {
                    crx = std::stoi(match.str(1));
                }
                if (boost::regex_search(input, match, boost::regex(",(.*?)$")) && match.size() > 0) {
                    cry = std::stoi(match.str(1));
                }
            }
            int alx = 0;
            int aly = 0;
            if (boost::regex_search(input, match, boost::regex("[0-9]{1,4},[0-9]{1,4}_AL")) && match.size() > 0) {
                std::string input = match.str(0);
                if (boost::regex_search(input, match, boost::regex("(.*?),")) && match.size() > 0) {
                    alx = std::stoi(match.str(1));
                }
                if (boost::regex_search(input, match, boost::regex(",(.*?)_AL")) && match.size() > 0) {
                    aly = std::stoi(match.str(1));
                }
            }

            if (boost::regex_search(input, match, boost::regex("https(.*?)\\._V")) && match.size() > 0) {
                std::string url;

                if (uy) {
                    alx += 2*(crx - 1);
                } else if (ux) {
                    aly += 2*(cry - 1);
                }
                if (alx/(float)aly > 0.667f) {
                    url = "https" + match.str(1) + "._V1_UY345_CR" + std::to_string((int)(0.5f*(345.0f*alx/(float)aly - 230.0f))) + ",0,230,345_AL_.jpg";
                } else {
                    url = "https" + match.str(1) + "._V1_UX230_CR0," + std::to_string((int)(0.5f*(230.0f*aly/(float)alx - 345.0f))) + ",230,345_AL_.jpg";
                }

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
