#include"PCTSingletonNetworkWorker.h"

PCTSingletonNetworkWorker::PCTSingletonNetworkWorker()
{
    //NADA
}

PCTSingletonNetworkWorker::~PCTSingletonNetworkWorker()
{
    //NADA
}

PCTNetworkWorker& PCTSingletonNetworkWorker::getInstance()
{
    static PCTNetworkWorker instance;
    return instance;
}
