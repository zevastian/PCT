#include<memory>
#include"PCTWorkItem.h"
#include"PCTCancellationToken.h"
#include"PCTCommon.h"

#ifndef _PCT_PREPAREREQUEST_H_
#define _PCT_PREPAREREQUEST_H_

class PCTPrepareRequest : public PCTWorkItem
{
private:
    std::shared_ptr<PCTCancellationToken> mToken;
    PCTCommon mCommon;

public:
    /**/
    PCTPrepareRequest(std::shared_ptr<PCTCancellationToken> token, PCTCommon common, int priority);
    /**/
    void run();
};

#endif
