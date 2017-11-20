#include<jansson.h>
#include<iostream>
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

PCT_APICODE PCTApi::process(std::vector<char>& buffer, std::vector<PCTMovie>& movies)
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

    json_t* moviess = json_object_get(root, "MovieList");
    if (!json_is_array(moviess)) {
        json_decref(root);
        std::cout << "MovieList: an array expected" << std::endl;
        return PCT_APICODE::PCT_API_FAILURE;
    }

    size_t i;
    for (i = 0; i < json_array_size(moviess); i++) {
        json_t* movie = json_array_get(moviess, i);
        if (json_is_object(movie)) {
            json_t* imdbCode = json_object_get(movie, "imdb");
            if (json_is_string(imdbCode)) {

                PCTMovie ytsMovie;
                ytsMovie.imdb_code = json_string_value(imdbCode);

                json_t* poster = json_object_get(movie, "poster_med");
                if (json_is_string(poster)) {
                    ytsMovie.small_cover_image_url = json_string_value(poster);
                    ytsMovie.medium_cover_image_url = json_string_value(poster);
                    ytsMovie.large_cover_image_url = json_string_value(poster);
                    movies.push_back(ytsMovie);
                }
            }
        }
    }

    json_decref(root);
    return PCT_APICODE::PCT_API_SUCCESS;
}

PCT_APICODE PCTApi::prepare(std::string& url)
{
    url = "https://api.torrentsapi.com/list?sort=seeds&count=50&page=" + std::to_string(mPage++);
    return PCT_APICODE::PCT_API_HAS_MORE;
}
