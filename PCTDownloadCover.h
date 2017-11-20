#include<memory>
#include<vector>
#include"PCTHttpGetRequest.h"
#include"PCTCancellationToken.h"
#include"PCTCommon.h"

#ifndef PCT_DOWNLOADCOVER_H_
#define PCT_DOWNLOADCOVER_H_

class PCTDownloadCover : public PCTHttpGetRequest
{
private:
    std::string mImdbCode;
    std::shared_ptr<PCTCancellationToken> mToken;
    std::vector<char> mCoverData;
    unsigned int mIndx;
    PCTCommon mCommon;

public:
    /**/
    PCTDownloadCover(std::string imdbCode, std::shared_ptr<PCTCancellationToken> token, unsigned int indx, PCTCommon common, int priority);

    /**/
    bool onStart();
    /**/
    bool onWrite(std::vector<char> buffer);
    /**/
    void onFinish(PCT_NETWORK_CODE code);
};

#endif
