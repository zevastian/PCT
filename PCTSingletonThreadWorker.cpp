#include"PCTSingletonThreadWorker.h"

PCTSingletonThreadWorker::PCTSingletonThreadWorker()
{
    //NADA
}

PCTSingletonThreadWorker::~PCTSingletonThreadWorker()
{
    //NADA
}

PCTThreadWorker& PCTSingletonThreadWorker::getInstance()
{
    static PCTThreadWorker instance;
    return instance;
}
