#include<vector>
#include<string>

#ifndef PCT_API_H_
#define PCT_API_H_

struct PCTMovie {
    std::string imdb_code;
    std::string background_image_url;
    std::string small_cover_image_url;
    std::string medium_cover_image_url;
    std::string large_cover_image_url;
    unsigned int seeders;
};

enum PCT_APICODE {
    PCT_API_FAILURE,
    PCT_API_SUCCESS,
    PCT_API_HAS_MORE,
    PCT_API_NOT_HAS_MORE
};

class PCTApi
{
private:
    int mPage;

public:
    /**/
    PCTApi();
    /**/
    ~PCTApi();

    /**/
    PCT_APICODE prepare(std::string& url);
    /**/
    PCT_APICODE process(std::vector<char>& buffer, std::vector<PCTMovie>& movies);
};

#endif
