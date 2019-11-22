#include "../include/ConfigMovieReader.h"

using nlohmann::json;
using std::string;
using std::vector;

ConfigMovieReader::ConfigMovieReader(const json& moviesSection) : _moviesSection(moviesSection) { }

int ConfigMovieReader::getMoviesCount() {
    return  _moviesSection.size();
}

Movie* ConfigMovieReader::readMovie(int i, long id) {
    json jMovie = _moviesSection.at(i);
    vector<string> tags;
    json jTags = jMovie.at("tags");
    for (int j = 0; j < jTags.size(); ++j) {
        tags.push_back(jTags.at(j));
    }
//    return new Movie(id, jMovie.at("name"), jMovie.at("length"), tags);
    return nullptr;
}