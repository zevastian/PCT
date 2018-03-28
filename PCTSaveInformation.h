#include<vector>
#include<string>
#include"PCTWorkItem.h"

#ifndef _PCT_SAVEINFORMATION_H_
#define _PCT_SAVEINFORMATION_H_

class PCTSaveInformation : public PCTWorkItem
{
private:
    std::string mImdbCode;
    std::vector<char> mInfoMovie;

public:
    /**/
    PCTSaveInformation(std::string imdbCode, std::vector<char> infoMovie, int priority);
    /**/
    void run();
};

#endif
