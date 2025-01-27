#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"

using namespace std;

Episode::Episode(long id, const string &seriesName, int length, int season, int episode,
                 const vector<string> &tags) : Watchable(id, length, tags),
                 seriesName(seriesName),
                 season(season),
                 episode(episode),
                 nextEpisodeId(-1) {}

string Episode::toString() const {
    return seriesName + " S" + to_string(season) + "E" + to_string(episode);
}

Watchable* Episode::getNextWatchable(Session &session) const {
    Watchable *recommendation = session.getContentById(nextEpisodeId);
    if (recommendation) {
        return recommendation;
    }

    return session.getActiveUser()->getRecommendation(session);
}

Watchable* Episode::clone() const {
    return new Episode(getId(), seriesName, getLength(), season, episode, getTags());
}

void Episode::setNextEpisodeId(long nextEpisodeId) {
    this->nextEpisodeId = nextEpisodeId;
}

string Episode::paddNumber(int n) const {
    if (n < 10) {
        return "0" + to_string(n);
    }

    return to_string(n);
}