#include"PCTThreadWorker.h"

#ifndef _PCT_SINGLETONTHREADWORKER_H_
#define _PCT_SINGLETONTHREADWORKER_H_

//TODO
//ME PARECE QUE ESTA MAL IMPLEMENTADA
class PCTSingletonThreadWorker
{
private:
    /**/
    PCTSingletonThreadWorker();
    /**/
    PCTSingletonThreadWorker(PCTSingletonThreadWorker const& copy);
    /**/
    PCTSingletonThreadWorker& operator=(PCTSingletonThreadWorker const& copy);
    /**/
    ~PCTSingletonThreadWorker();

public:
    /**/
    static PCTThreadWorker& getInstance();
};

#endif
