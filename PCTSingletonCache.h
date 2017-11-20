#include"PCTCache.h"

#ifndef _PCT_SINGLETONCACHE_H_
#define _PCT_SINGLETONCACHE_H_

//TODO
//ME PARECE QUE ESTA MAL IMPLEMENTADA
class PCTSingletonCache
{
private:
    /**/
    PCTSingletonCache();
    /**/
    PCTSingletonCache(PCTSingletonCache const& copy);
    /**/
    PCTSingletonCache& operator=(PCTSingletonCache const& copy);
    /**/
    ~PCTSingletonCache();

public:
    /**/
    static PCTCache& getInstance();
};

#endif
