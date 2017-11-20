#include<jansson.h>
#include<iostream>
#include<fstream>
#include<algorithm>
#include"PCTApi.h"

PCTApi::PCTApi()
    :mPage(1)
{
    //NADA
}

PCTApi::~PCTApi()
{
    //NADA
}

PCT_APICODE PCTApi::process(std::vector<char>& buffer, std::vector<PCTMovie>& moviess)
{
    json_error_t error;
    json_t* root = json_loads(buffer.data(), 0, &error);

    if (!root) {
        std::cout << "jansson error:, line ";
        std::cout << std::to_string(error.line);
        std::cout << " column ";
        std::cout << std::to_string(error.column);
        std::cout << "\n\n";
        std::cout << std::string(buffer.begin(), buffer.end()) << std::endl;
        return PCT_APICODE::PCT_API_FAILURE;
    }

    if (!json_is_object(root)) {
        json_decref(root);
        std::cout << "jansson: an object expected" << std::endl;
        return PCT_APICODE::PCT_API_FAILURE;
    }

    json_t *movies = json_object_get(root, "torrent_results");
    if (!json_is_array(movies)) {
        json_decref(root);
        return PCT_APICODE::PCT_API_FAILURE;
    }

    size_t j;
    for (j = 0; j < json_array_size(movies); j++) {
        json_t *movie = json_array_get(movies, j);
        if (!json_is_object(movie)) {
            continue;
        }

        json_t *seeders = json_object_get(movie, "seeders");
        if (!json_is_integer(seeders)) {
            continue;
        }

        json_t *leechers = json_object_get(movie, "leechers");
        if (!json_is_integer(leechers)) {
           continue;
        }

        json_t *info = json_object_get(movie, "episode_info");
        if (!json_is_object(info)) {
           continue;
        }

        json_t *imdb = json_object_get(info, "imdb");
        if (!json_is_string(imdb)) {
            continue;
        }

        bool jump = false;
        for (unsigned int i = 0; i < moviess.size(); i++) {
            if (moviess[i].imdb_code == json_string_value(imdb)) {
                moviess[i].seeders += json_integer_value(seeders);
                jump = true;
                break;
            }
        }

        if (!jump) {
            PCTMovie mv;
            mv.imdb_code = json_string_value(imdb);
            mv.seeders = json_integer_value(seeders);
            moviess.push_back(mv);
        }
    }

    std::sort(moviess.begin(), moviess.end(), [](PCTMovie mv1, PCTMovie mv2){
        return mv1.seeders > mv2.seeders;
    });

    json_decref(root);
    return PCT_APICODE::PCT_API_SUCCESS;
}

PCT_APICODE PCTApi::prepare(std::string& url)
{
    std::ifstream inputfile("token", std::ios::binary);
    std::vector<char> key((std::istreambuf_iterator<char>(inputfile)), std::istreambuf_iterator<char>());
    url = "https://torrentapi.org/pubapi_v2.php?mode=list&category=movies&sort=seeders&format=json_extended&limit=100&token=" + std::string(key.begin(), key.end());
    return mPage++ > 1 ? PCT_APICODE::PCT_API_NOT_HAS_MORE : PCT_APICODE::PCT_API_HAS_MORE;
}
