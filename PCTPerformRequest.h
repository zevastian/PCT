#include<memory>
#include<vector>
#include"PCTHttpGetRequest.h"
#include"PCTCancellationToken.h"
#include"PCTCommon.h"

#ifndef PCT_PREPAREREQUEST_H_
#define PCT_PREPAREREQUEST_H_

class PCTPerformRequest : public PCTHttpGetRequest
{
private:
    std::shared_ptr<PCTCancellationToken> mToken;
    std::vector<char> mRequestData;
    PCTCommon mCommon;

public:
    /**/
    PCTPerformRequest(std::shared_ptr<PCTCancellationToken> token, PCTCommon common, int priority);

    /**/
    bool onStart();
    /**/
    bool onWrite(std::vector<char> buffer);
    /**/
    void onFinish(PCT_NETWORK_CODE code);
};

#endif
