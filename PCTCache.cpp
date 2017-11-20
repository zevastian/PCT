#include<string>
#include<string.h>
#include<stdexcept>
#include"PCTCache.h"

PCTCache::PCTCache()
    :mConection(nullptr), mSet(nullptr), mGet(nullptr)
{
    if (sqlite3_open("cache", &mConection)) {
        throw std::runtime_error(sqlite3_errmsg(mConection));
    }

    char* err = nullptr;
    if (sqlite3_exec(mConection, "PRAGMA journal_mode = OFF;", NULL, NULL, &err) != SQLITE_OK) {
        sqlite3_close(mConection);
        mConection = nullptr;
        throw std::runtime_error(err);
    }

    if (sqlite3_exec(mConection, "PRAGMA synchronous = OFF;", NULL, NULL, &err) != SQLITE_OK) {
        sqlite3_close(mConection);
        mConection = nullptr;
        throw std::runtime_error(err);
    }

    if (sqlite3_exec(mConection, "PRAGMA locking_mode = EXCLUSIVE;", NULL, NULL, &err) != SQLITE_OK) {
        sqlite3_close(mConection);
        mConection = nullptr;
        throw std::runtime_error(err);
    }

    //CHEQUEO SI LA BASE DE DATO TIENE LA TABLA YA CREADA
    int num = 0;
    std::string sql = "SELECT COUNT(TYPE) FROM SQLITE_MASTER WHERE TYPE = 'table' AND NAME = 'MOVIE_PICTURE';";
    if (sqlite3_exec(mConection, sql.c_str(), callback, &num, &err) != SQLITE_OK) {
        sqlite3_close(mConection);
        mConection = nullptr;
        throw std::runtime_error(err);
    }

    //SI NO ESTA CREADA CREO LA TABLA
    if (!num) {
        sql = "CREATE TABLE MOVIE_PICTURE(";
        sql += "URL_PICTURE TEXT PRIMARY KEY NOT NULL,";
        sql += "PICTURE BLOB NOT NULL);";
        if (sqlite3_exec(mConection, sql.c_str(), NULL, NULL, &err) != SQLITE_OK) {
            sqlite3_close(mConection);
            mConection = nullptr;
            throw std::runtime_error(err);
        }
    }

    sql = "INSERT INTO MOVIE_PICTURE (URL_PICTURE, PICTURE) VALUES (?, ?);";
    if (sqlite3_prepare_v2(mConection, sql.c_str(), sql.length(), &mSet, NULL) != SQLITE_OK) {
        sqlite3_close(mConection);
        mConection = nullptr;
        throw std::runtime_error("sqlite3_prepare_v2 failed");
    }

    sql = "SELECT PICTURE, LENGTH(PICTURE) FROM MOVIE_PICTURE WHERE URL_PICTURE = ?;";
    if (sqlite3_prepare_v2(mConection, sql.c_str(), sql.length(), &mGet, NULL) != SQLITE_OK) {
        sqlite3_finalize(mSet);
        sqlite3_close(mConection);
        mConection = nullptr;
        throw std::runtime_error("sqlite3_prepare_v2 failed");
    }
}

PCTCache::~PCTCache()
{
    if (mConection) {
        sqlite3_finalize(mSet);
        sqlite3_finalize(mGet);
        sqlite3_close(mConection);
    }
}

int PCTCache::callback(void* data, int count, char** rows, char** name)
{
    if (count == 1 && rows) {
        *((int*)data) = atoi(rows[0]);
        return 0;
    }
    return 1;
}

bool PCTCache::get(std::string id, std::vector<char>& data)
{
    mMutex.lock();
    //REINICIO ANTES DE USAR
    sqlite3_reset(mGet);
    sqlite3_clear_bindings(mGet);
    bool ret = false;
    if (sqlite3_bind_text(mGet, 1, id.c_str(), id.length(), SQLITE_STATIC) == SQLITE_OK) {
        if (sqlite3_step(mGet) == SQLITE_ROW) {
            int size = sqlite3_column_int(mGet, 1);
            data.resize(size);
            memcpy(data.data(), sqlite3_column_blob(mGet, 0), size);
            ret = true;
        }
    }
    mMutex.unlock();
    return ret;
}

void PCTCache::set(std::string id, std::vector<char> data)
{
    mMutex.lock();
    //REINICIO ANTES DE USAR
    sqlite3_reset(mSet);
    sqlite3_clear_bindings(mSet);
    if (sqlite3_bind_text(mSet, 1, id.c_str(), id.length(), SQLITE_STATIC) == SQLITE_OK) {
        if (sqlite3_bind_blob(mSet, 2, data.data(), data.size()*sizeof(char), SQLITE_STATIC) == SQLITE_OK) {
            sqlite3_step(mSet);
        }
    }
    mMutex.unlock();
}
