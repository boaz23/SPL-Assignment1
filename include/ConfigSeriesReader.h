#ifndef ASS1_CONFIGSERIESREADER_H
#define ASS1_CONFIGSERIESREADER_H

#include "../include/json.hpp"
#include "../include/Watchable.h"
#include "../include/TvSeries.h"

class ConfigSeriesReader {
public:
    ConfigSeriesReader(const nlohmann::json& seriesSection);
    int getSeriesCount();
    TvSeries* readSeries(int i);

private:
    const nlohmann::json& _seriesSection;
};


#endif
