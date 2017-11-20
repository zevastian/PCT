#include<string>
#include<vector>
#include"PCTNetworkItem.h"

#ifndef _PCT_HTTPGETREQUEST_H_
#define _PCT_HTTPGETREQUEST_H_

enum PCT_NETWORK_CODE {
    PCT_NET_SUCCESS,
    PCT_NET_FAILURE
};

class PCTHttpGetRequest : public PCTNetworkItem
{
private:
    //TEST
    std::string mURL;

public:
    /**/
    PCTHttpGetRequest(int priority);

    /**/
    bool onStart(CURL* handle);
    /**/
    void onFinish(CURL* handle, CURLcode code);
    /**/
    static size_t onWritePrivate(char* ptr, size_t size, size_t nmemb, PCTHttpGetRequest* self);

    /**/
    virtual bool onStart() = 0;
    /**/
    virtual bool onWrite(std::vector<char> buffer) = 0;
    /**/
    virtual void onFinish(PCT_NETWORK_CODE code) = 0;

    /**/
    void setURL(std::string url);
};

#endif
