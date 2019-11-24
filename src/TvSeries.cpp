#include "../include/TvSeries.h"

TvSeries::TvSeries(const std::string &name, int episodeLength, const std::vector<int> &seasons,
                   const std::vector<std::string> &tags) : _name(name),
                   _episodeLength(episodeLength),
                   _seasons(seasons),
                   _tags(tags) {}

const std::string& TvSeries::getName() const {
    return _name;
}
int TvSeries::getEpisodeLength() const {
    return _episodeLength;
}
const std::vector<int>& TvSeries::getSeasons() const {
    return _seasons;
}
const std::vector<std::string>& TvSeries::getTags() const {
    return _tags;
}