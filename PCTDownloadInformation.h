#include<memory>
#include<vector>
#include"PCTHttpGetRequest.h"
#include"PCTCancellationToken.h"
#include"PCTApi.h"
#include"PCTCommon.h"

#ifndef PCT_DOWNLOADINFORMATION_H_
#define PCT_DOWNLOADINFORMATION_H_

class PCTDownloadInformation : public PCTHttpGetRequest
{
private:
    PCTMovie mMovie;
    std::shared_ptr<PCTCancellationToken> mToken;
    std::vector<char> mInforMovie;
    unsigned int mIndx;
    PCTCommon mCommon;

public:
    /**/
    PCTDownloadInformation(PCTMovie movie, std::shared_ptr<PCTCancellationToken> token, unsigned int indx, PCTCommon common, int priority);

    /**/
    bool onStart();
    /**/
    bool onWrite(std::vector<char> buffer);
    /**/
    void onFinish(PCT_NETWORK_CODE code);
};

#endif
