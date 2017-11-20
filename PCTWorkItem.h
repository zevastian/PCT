#ifndef _PCT_WORKITEM_H_
#define _PCT_WORKITEM_H_

class PCTWorkItem
{
private:
    int mPriority;

public:
    /**/
    PCTWorkItem(int priority);
    /**/
    bool operator<(const PCTWorkItem& item) const;
    /**/
    bool operator<=(const PCTWorkItem& item) const;
    /**/
    bool operator>(const PCTWorkItem& item) const;
    /**/
    bool operator>=(const PCTWorkItem& item) const;
    /**/
    virtual void run() = 0;
};

#endif
