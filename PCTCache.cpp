#include<string>
#include<cstring>
#include<stdexcept>
#include<thread>
#include"PCTCache.h"
#include"PCTSingletonThreadWorker.h"

//NO SE ESTA CONTROLANDO BIEN LOS ERRORES
PCTCache::PCTCache()
{
    if (sqlite3_enable_shared_cache(1) != SQLITE_OK) {
        throw std::runtime_error("sqlite3_enable_shared_cache failed");
    }

    unsigned int numThread = PCTSingletonThreadWorker::getInstance().getConcurrency();
    //CREAR CONEXIONES
    for (unsigned int i = 0; i < numThread; i++) {
        SQLiteConection sqlcon;
        if (sqlite3_open_v2("cache", &sqlcon.conection, SQLITE_OPEN_NOMUTEX |
                            SQLITE_OPEN_SHAREDCACHE | SQLITE_OPEN_READWRITE |
                            SQLITE_OPEN_CREATE, NULL)) {
            throw std::runtime_error(sqlite3_errmsg(sqlcon.conection));
        }

        //CONFIGURO PARA MAXIMO RENDIMIENTO
        char* err = nullptr;
        if (sqlite3_exec(sqlcon.conection, "PRAGMA journal_mode = OFF;", NULL, NULL, &err) != SQLITE_OK) {
            sqlite3_close(sqlcon.conection);
            throw std::runtime_error(err);
        }
        if (sqlite3_exec(sqlcon.conection, "PRAGMA synchronous = OFF;", NULL, NULL, &err) != SQLITE_OK) {
            sqlite3_close(sqlcon.conection);
            throw std::runtime_error(err);
        }
        if (sqlite3_exec(sqlcon.conection, "PRAGMA locking_mode = EXCLUSIVE;", NULL, NULL, &err) != SQLITE_OK) {
            sqlite3_close(sqlcon.conection);
            throw std::runtime_error(err);
        }

        mConections.push(sqlcon);
    }

    //CHEQUEO SI LA BASE DE DATO TIENE LA TABLA YA CREADA
    int num = 0;
    char* err = nullptr;
    std::string sql = "SELECT COUNT(TYPE) FROM SQLITE_MASTER WHERE TYPE = 'table' AND NAME = 'MOVIE_PICTURE';";
    if (sqlite3_exec(mConections.front().conection, sql.c_str(), callback, &num, &err) != SQLITE_OK) {
        sqlite3_close(mConections.front().conection);
        throw std::runtime_error(err);
    }
    //SI NO ESTA CREADA CREO LA TABLA
    if (!num) {
        sql = "CREATE TABLE MOVIE_PICTURE(";
        sql += "URL_PICTURE TEXT PRIMARY KEY NOT NULL,";
        sql += "PICTURE BLOB NOT NULL);";
        if (sqlite3_exec(mConections.front().conection, sql.c_str(), NULL, NULL, &err) != SQLITE_OK) {
            sqlite3_close(mConections.front().conection);
            throw std::runtime_error(err);
        }
    }

    //PREPARO
    for (unsigned int i = 0; i < numThread; i++) {
        SQLiteConection sqlcon = mConections.front();
        mConections.pop();
        std::string sql = "INSERT INTO MOVIE_PICTURE (URL_PICTURE, PICTURE) VALUES (?, ?);";
        if (sqlite3_prepare_v2(sqlcon.conection, sql.c_str(), sql.length(), &sqlcon.stmtSet, NULL) != SQLITE_OK) {
            sqlite3_close(sqlcon.conection);
            throw std::runtime_error("sqlite3_prepare_v2 failed");
        }

        sql = "SELECT PICTURE, LENGTH(PICTURE) FROM MOVIE_PICTURE WHERE URL_PICTURE = ?;";
        if (sqlite3_prepare_v2(sqlcon.conection, sql.c_str(), sql.length(), &sqlcon.stmtGet, NULL) != SQLITE_OK) {
            sqlite3_finalize(sqlcon.stmtSet);
            sqlite3_close(sqlcon.conection);
            throw std::runtime_error("sqlite3_prepare_v2 failed");
        }
        mConections.push(sqlcon);
    }
}

PCTCache::~PCTCache()
{
    while (!mConections.empty()) {
        SQLiteConection sqlcon = mConections.front();
        mConections.pop();
        sqlite3_finalize(sqlcon.stmtSet);
        sqlite3_finalize(sqlcon.stmtGet);
        sqlite3_close(sqlcon.conection);
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
    SQLiteConection sqlcon;
    mMutex.lock();
    sqlcon = mConections.front();
    mConections.pop();
    mMutex.unlock();

    //REINICIO ANTES DE USAR
    sqlite3_reset(sqlcon.stmtGet);
    sqlite3_clear_bindings(sqlcon.stmtGet);
    bool ret = false;
    if (sqlite3_bind_text(sqlcon.stmtGet, 1, id.c_str(), id.length(), SQLITE_STATIC) == SQLITE_OK) {
        if (sqlite3_step(sqlcon.stmtGet) == SQLITE_ROW) {
            int size = sqlite3_column_int(sqlcon.stmtGet, 1);
            data.resize(size);
            std::memcpy(data.data(), sqlite3_column_blob(sqlcon.stmtGet, 0), size);
            ret = true;
        }
    }

    mMutex.lock();
    mConections.push(sqlcon);
    mMutex.unlock();
    return ret;
}

void PCTCache::set(std::string id, std::vector<char> data)
{
    SQLiteConection sqlcon;
    mMutex.lock();
    sqlcon = mConections.front();
    mConections.pop();
    mMutex.unlock();

    //REINICIO ANTES DE USAR
    sqlite3_reset(sqlcon.stmtSet);
    sqlite3_clear_bindings(sqlcon.stmtSet);
    if (sqlite3_bind_text(sqlcon.stmtSet, 1, id.c_str(), id.length(), SQLITE_STATIC) == SQLITE_OK) {
        if (sqlite3_bind_blob(sqlcon.stmtSet, 2, data.data(), data.size(), SQLITE_STATIC) == SQLITE_OK) {
            sqlite3_step(sqlcon.stmtSet);
        }
    }

    mMutex.lock();
    mConections.push(sqlcon);
    mMutex.unlock();
}
