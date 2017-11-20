#include<memory>
#include<vector>
#include"PCTWorkItem.h"
#include"PCTCancellationToken.h"
#include"PCTCommon.h"

#ifndef _PCT_DECODECOVER_H_
#define _PCT_DECODECOVER_H_

class PCTDecodeCover : public PCTWorkItem
{
private:
    std::vector<char> mData;
    std::shared_ptr<PCTCancellationToken> mToken;
    unsigned int mIndx;
    PCTCommon mCommon;

public:
    /**/
    PCTDecodeCover(std::vector<char> data, std::shared_ptr<PCTCancellationToken> token, unsigned int indx, PCTCommon common, int priority);
    /**/
    void run();
};

#endif
