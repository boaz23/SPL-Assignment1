#include "../include/Watchable.h"
#include "../include/User.h"

using namespace std;

Episode::Episode(long id, const string &seriesName, int length, int season, int episode,
                 const vector<string> &tags) : Watchable(id, length, tags),
                 seriesName(seriesName),
                 season(season),
                 episode(episode),
                 nextEpisodeId(-1) {}

string Episode::toString() const {
    return seriesName + "S" + paddNumber(season) + "E" + paddNumber(episode);
}

Watchable* Episode::getNextWatchable(Session &session) const {
    if (nextEpisodeId >= 0) {
        return session.getContentById(nextEpisodeId);
    }

    return session.getActiveUser()->getRecommendation(session);
}

Watchable* Episode::clone() const {
    return new Episode(getId(), seriesName, getLength(), season, episode, getTags());
}

string Episode::paddNumber(int n) const {
    if (n < 10) {
        return "0" + to_string(n);
    }

    return to_string(n);
}