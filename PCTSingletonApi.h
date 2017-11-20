#include"PCTApi.h"

#ifndef _PCT_SINGLETONAPI_H_
#define _PCT_SINGLETONAPI_H_

//TODO
//ME PARECE QUE ESTA MAL IMPLEMENTADA
class PCTSingletonApi
{
private:
    /**/
    PCTSingletonApi();
    /**/
    PCTSingletonApi(PCTSingletonApi const& copy);
    /**/
    PCTSingletonApi& operator=(PCTSingletonApi const& copy);
    /**/
    ~PCTSingletonApi();

public:
    /**/
    static PCTApi& getInstance();
};

#endif

