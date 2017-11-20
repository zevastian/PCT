#include"PCTNetworkWorker.h"

#ifndef _PCT_SINGLETONNETWORKWORKER_H_
#define _PCT_SINGLETONNETWORKWORKER_H_

//TODO
//ME PARECE QUE ESTA MAL IMPLEMENTADA
class PCTSingletonNetworkWorker
{
private:
    /**/
    PCTSingletonNetworkWorker();
    /**/
    PCTSingletonNetworkWorker(PCTSingletonNetworkWorker const& copy);
    /**/
    PCTSingletonNetworkWorker& operator=(PCTSingletonNetworkWorker const& copy);
    /**/
    ~PCTSingletonNetworkWorker();

public:
    /**/
    static PCTNetworkWorker& getInstance();
};

#endif
