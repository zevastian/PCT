#include<string>
#include"NetworkItem.h"

#ifndef _NETWORKGETREQUEST_H_
#define _NETWORKGETREQUEST_H_

class PCTNetworkGETRequest : public PCTNetworkItem
{
private:
    enum PCT_GET_OPTION {
        PCT_URL,
        PCT_USER_AGENT,
        PCT_ACCEPT_ENCODING,
        PCT_FOLLOW_LOCATION,
        PCT_NO_SIGNAL,
        PCT_USE_SSL,
        PCT_VERIFY_HOST,
        PCT_VERIFY_PEER,
        PCT_TIMEOUT
    };
    //TEST
    std::string mURL;

public:
    /**/
    void setURL(std::string url);
    /**/
    void setUserAgent(std::string userAgent);
    /**/
    void setAcceptEncoding(bool enable);
    /**/
    void setFollowLocation(bool enable);
    /**/
    void setNoSignal(bool enable);
    /**/
    void setUseSSL(bool enable);
    /**/
    void setVerifyHost(bool enable);
    /**/
    void setVerifyPeer(bool enable);
    /**/
    void setTimeout(int seconds);
    /**/
    bool onStartPrivate(CURL* handle);
};

#endif
