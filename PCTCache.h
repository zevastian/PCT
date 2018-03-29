#include<sqlite3.h>
#include<queue>
#include<mutex>

#ifndef PCT_CACHE__H_
#define PCT_CACHE__H_

class PCTCache
{
private:
    struct SQLiteConection {
        sqlite3* conection;
        sqlite3_stmt* stmtSet;
        sqlite3_stmt* stmtGet;
        SQLiteConection()
        {
            conection = NULL;
            stmtSet = NULL;
            stmtGet = NULL;
        }
    };

    std::queue<SQLiteConection> mConections;
    std::mutex mMutex;

    /**/
    static int callback(void* data, int count, char** rows, char** name);
    /**/
    void popConection(SQLiteConection& conection);
    /**/
    void pushConection(SQLiteConection conection);

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
