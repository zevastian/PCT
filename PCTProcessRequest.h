#include<memory>
#include<vector>
#include"PCTWorkItem.h"
#include"PCTCancellationToken.h"
#include"PCTCommon.h"

#ifndef _PCT_PROCESSREQUEST_H_
#define _PCT_PROCESSREQUEST_H_

class PCTProcessRequest : public PCTWorkItem
{
private:
    std::vector<char> mRequestData;
    std::shared_ptr<PCTCancellationToken> mToken;
    PCTCommon mCommon;

public:
    /**/
    PCTProcessRequest(std::vector<char> requestData, std::shared_ptr<PCTCancellationToken> token, PCTCommon common, int priority);
    /**/
    void run();
};

#endif
