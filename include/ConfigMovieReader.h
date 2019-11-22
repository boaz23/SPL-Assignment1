#ifndef ASS1_CONFIGMOVIEREADER_H
#define ASS1_CONFIGMOVIEREADER_H

#include "../include/Watchable.h"
#include "../include/json.hpp"

class ConfigMovieReader {
public:
    ConfigMovieReader(const nlohmann::json& moviesSection);
    int getMoviesCount();
    Movie* readMovie(int i, long id);

private:
    const nlohmann::json& _moviesSection;
};


#endif
