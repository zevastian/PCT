#include"PCTSaveCover.h"
#include"PCTSingletonCache.h"

PCTSaveCover::PCTSaveCover(std::string imdbCode, std::vector<char> coverData, int priority)
    :PCTWorkItem(priority), mImdbCode(imdbCode), mCoverData(coverData)
{
    //NADA
}

void PCTSaveCover::run()
{
    //SET DEBERIA RETORNAR UN ERROR?
#ifndef __NO_SAVE__
    PCTSingletonCache::getInstance().set(mImdbCode + "_COVER", mCoverData);
#endif // __NO_SAVE__
}