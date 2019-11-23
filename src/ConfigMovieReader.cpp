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
    for (auto &jTag : jTags) {
        tags.push_back(jTag);
    }
    return new Movie(id, jMovie.at("name"), jMovie.at("length"), tags);
}