#include<vector>
#include<string>
#include"PCTWorkItem.h"

#ifndef _PCT_SAVECOVER_H_
#define _PCT_SAVECOVER_H_

class PCTSaveCover : public PCTWorkItem
{
private:
    std::string mImdbCode;
    std::vector<char> mCoverData;

public:
    /**/
    PCTSaveCover(std::string imdbCode, std::vector<char> dataCover, int priority);
    /**/
    void run();
};

#endif
