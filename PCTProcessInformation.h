#include<memory>
#include<vector>
#include"PCTWorkItem.h"
#include"PCTCancellationToken.h"
#include"PCTApi.h"
#include"PCTCommon.h"

#ifndef _PCT_PROCESSINFORMATION_H_
#define _PCT_PROCESSINFORMATION_H_

class PCTProcessInformation : public PCTWorkItem
{
private:
    PCTMovie mMovie;
    std::vector<char> mInfoMovie;
    std::shared_ptr<PCTCancellationToken> mToken;
    unsigned int mIndex;
    PCTCommon mCommon;

public:
    /**/
    PCTProcessInformation(PCTMovie movie, std::vector<char> infoMovie, std::shared_ptr<PCTCancellationToken> token, unsigned int indx, PCTCommon common, int priority);
    /**/
    void run();
};

#endif

