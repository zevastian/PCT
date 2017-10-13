#include<curl/curl.h>
#include<stdio.h>
#include<vector>

#ifndef _NETWORKITEM_H_
#define _NETWORKITEM_H_

enum PCT_NETWORK_CODE {
    PCT_NET_SUCCESS,
    PCT_NET_FAILURE
};

class PCTNetworkItem
{
private:

public:
    /**/
    virtual bool onStart() = 0;
    /**/
    virtual bool onStartPrivate(CURL* handle) = 0;
    /**/
    virtual bool onWrite(std::vector<char> buffer) = 0;
    /**/
    static size_t onWritePrivate(char* ptr, size_t size, size_t nmemb, PCTNetworkItem* self);
    /**/
    virtual void onFinish(PCT_NETWORK_CODE code) = 0;
};

#endif
