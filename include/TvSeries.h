#ifndef ASS1_TVSERIES_H
#define ASS1_TVSERIES_H


#include <vector>
#include <string>

class TvSeries {
public:
    TvSeries(const std::string& name, int episodeLength, const std::vector<int>& seasons, const std::vector<std::string>& tags);
    const std::string& getName() const;
    int getEpisodeLength() const;
    const std::vector<int>& getSeasons() const;
    const std::vector<std::string>& getTags() const;

private:
    std::string _name;
    int _episodeLength;
    std::vector<int> _seasons;
    std::vector<std::string> _tags;
};


#endif
