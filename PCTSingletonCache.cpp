#include"PCTSingletonCache.h"

PCTSingletonCache::PCTSingletonCache()
{
    //NADA
}

PCTSingletonCache::~PCTSingletonCache()
{
    //NADA
}

PCTCache& PCTSingletonCache::getInstance()
{
    static PCTCache instance;
    return instance;
}
