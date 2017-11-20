#include<sqlite3.h>
#include<vector>
#include<mutex>

#ifndef PCT_CACHE__H_
#define PCT_CACHE__H_

class PCTCache
{
private:
    sqlite3* mConection;
    sqlite3_stmt* mSet;
    sqlite3_stmt* mGet;
    std::mutex mMutex;
    static int callback(void* data, int count, char** rows, char** name);

public:
    /**/
    PCTCache();
    /**/
    ~PCTCache();

    /**/
    bool get(std::string id, std::vector<char>& data);
    /**/
    void set(std::string id, std::vector<char> data);
};

#endif
