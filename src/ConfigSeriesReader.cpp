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
    for (int j = 0; j < jTags.size(); ++j) {
        tags.push_back(jTags[j]);
    }

    vector<int> seasons;
    json jSeasons = jSeries["seasons"];
    for (int j = 0; j < jSeasons.size(); ++j) {
        seasons.push_back(jSeasons[j]);
    }

    return unique_ptr<TvSeries>(new TvSeries(jSeries["name"], jSeries["episode_length"], seasons, tags));
}