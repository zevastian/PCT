#include"PCTSaveInformation.h"
#include"PCTSingletonCache.h"

PCTSaveInformation::PCTSaveInformation(std::string imdbCode, std::vector<char> infoMovie, int priority)
    :PCTWorkItem(priority), mImdbCode(imdbCode), mInfoMovie(infoMovie)
{
    //NADA
}

void PCTSaveInformation::run()
{
    //SET DEBERIA RETORNAR UN ERROR?
    PCTSingletonCache::getInstance().set(mImdbCode + "_INFO", mInfoMovie);
}


