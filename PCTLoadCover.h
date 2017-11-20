#include<memory>
#include"PCTWorkItem.h"
#include"PCTCancellationToken.h"
#include"PCTApi.h"
#include"PCTCommon.h"

#ifndef _PCT_LOADCOVER_H_
#define _PCT_LOADCOVER_H_

class PCTLoadCover : public PCTWorkItem
{
private:
    PCTMovie mMovie;
    std::shared_ptr<PCTCancellationToken> mToken;
    unsigned int mIndex;
    PCTCommon mCommon;

public:
    /**/
    PCTLoadCover(PCTMovie movie, std::shared_ptr<PCTCancellationToken> token, unsigned int indx, PCTCommon common, int priority);
    /**/
    void run();
};

#endif
