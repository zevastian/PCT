#include<curl/curl.h>

#ifndef _PCT_NETWORKITEM_H_
#define _PCT_NETWORKITEM_H_

class PCTNetworkItem
{
private:
    int mPriority;

public:
    /**/
    PCTNetworkItem(int priority);
    /**/
    bool operator<(const PCTNetworkItem& item) const;
    /**/
    bool operator<=(const PCTNetworkItem& item) const;
    /**/
    bool operator>(const PCTNetworkItem& item) const;
    /**/
    bool operator>=(const PCTNetworkItem& item) const;
    /**/
    virtual bool onStart(CURL* handle) = 0;
    /**/
    virtual void onFinish(CURL* handle, CURLcode code) = 0;
};

#endif
