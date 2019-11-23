#include "../include/ConfigSeriesReader.h"
#include "../include/Watchable.h"

using nlohmann::json;
using std::string;
using std::vector;
using std::unique_ptr;

ConfigSeriesReader::ConfigSeriesReader(const nlohmann::json &seriesSection) : _seriesSection(seriesSection) {}

int ConfigSeriesReader::getSeriesCount() {
    return _seriesSection.size();
}

unique_ptr<TvSeries> ConfigSeriesReader::readSeries(int i) {
    json jSeries = _seriesSection[i];
    vector<string> tags;
    json jTags = jSeries["tags"];
    for (auto &jTag : jTags) {
        tags.push_back(jTag);
    }

    vector<int> seasons;
    json jSeasons = jSeries["seasons"];
    for (auto &jSeason : jSeasons) {
        seasons.push_back(jSeason);
    }

    return unique_ptr<TvSeries>(new TvSeries(jSeries["name"], jSeries["episode_length"], seasons, tags));
}