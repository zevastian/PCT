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

    json_t* data = json_object_get(root, "data");
    if (!json_is_object(data)) {
        json_decref(root);
        std::cout << "data: an object expected\n" << std::endl;
        return PCT_APICODE::PCT_API_FAILURE;
    }

    json_t* jsonMovies = json_object_get(data, "movies");
    if (!json_is_array(jsonMovies)) {
        json_decref(root);
        std::cout << "movies: an array expected\n" << std::endl;
        return PCT_APICODE::PCT_API_FAILURE;
    }

    size_t i;
    for (i = 0; i < json_array_size(jsonMovies); i++) {

        json_t* movie = json_array_get(jsonMovies, i);
        if (json_is_object(movie)) {

            json_t* imdbCode = json_object_get(movie, "imdb_code");
            if (json_is_string(imdbCode)) {

                PCTMovie ytsMovie;
                ytsMovie.imdb_code = json_string_value(imdbCode);

                json_t* background_image = json_object_get(movie, "background_image");
                if (json_is_string(background_image)) {
                    ytsMovie.background_image_url = json_string_value(background_image);
                }

                json_t* small_cover_image = json_object_get(movie, "small_cover_image");
                if (json_is_string(small_cover_image)) {
                    ytsMovie.small_cover_image_url = json_string_value(small_cover_image);
                }

                json_t* medium_cover_image = json_object_get(movie, "medium_cover_image");
                if (json_is_string(medium_cover_image)) {
                    ytsMovie.medium_cover_image_url = json_string_value(medium_cover_image);
                }

                json_t* large_cover_image = json_object_get(movie, "large_cover_image");
                if (json_is_string(large_cover_image)) {
                    ytsMovie.large_cover_image_url = json_string_value(large_cover_image);
                }

                movies.push_back(ytsMovie);
            }
        }
    }

    json_decref(root);
    return PCT_APICODE::PCT_API_SUCCESS;
}

PCT_APICODE PCTApi::prepare(std::string& url)
{
    url = "https://yts.ag/api/v2/list_movies.json?sort_by=date_added&limit=50&page=" + std::to_string(mPage++);
    return PCT_APICODE::PCT_API_HAS_MORE;
}
