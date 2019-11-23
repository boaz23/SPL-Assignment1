#include <fstream>
#include "../include/Session.h"
#include "../include/ConfigMovieReader.h"
#include "../include/ConfigSeriesReader.h"
#include "../include/User.h"

using namespace std;
using json = nlohmann::json;

void Session::clean() {
    for (auto &watchable : content) {
        delete watchable;
    }
    for (auto &action : actionsLog) {
        delete action;
    }
    for (auto &user : userMap) {
        delete user.second;
    }
    activeUser = nullptr;
}

void Session::steal(Session &other) {
    content = move(other.content);
    actionsLog = move(other.actionsLog);
    userMap = move(other.userMap);
    activeUser = other.activeUser;
    other.activeUser = nullptr;
    exitFlag = other.exitFlag;
}

void Session::copy(const Session &other) {
    content = vector<Watchable*>();
    for (auto pWatchable : other.content) {
        content.push_back(pWatchable->clone());
    }

    actionsLog = vector<BaseAction*>();
    for (auto pAction : other.actionsLog) {
        actionsLog.push_back(pAction->clone());
    }

    userMap = unordered_map<string, User*>();
    for (const auto& user : other.userMap) {
        userMap[user.first] = user.second->clone();
    }

    activeUser = userMap[other.activeUser->getName()];
    exitFlag = other.exitFlag;
}

Session::~Session() {
    clean();
}

Session::Session(const Session &other) {
    copy(other);
}
Session::Session(Session &&rval) {
    steal(rval);
}
Session& Session::operator=(const Session &other) {
    if (&other != this) {
        clean();
        copy(other);
    }
    return *this;
}
Session& Session::operator=(Session &&rval) {
    if (&rval != this) {
        clean();
        steal(rval);
    }
    return *this;
}

Session::Session(const string &configFilePath) : content(), actionsLog(), userMap(), activeUser(nullptr), exitFlag(false) {
    long watchableId = -1;
    vector<Watchable*> content;

    ifstream ifsCfg(configFilePath);
    json jsonCfg;
    ifsCfg >> jsonCfg;

    ConfigMovieReader mvReader(jsonCfg["movies"]);
    for (int i = 0; i < mvReader.getMoviesCount(); ++i) {
        content.push_back(mvReader.readMovie(i, ++watchableId));
    }

    vector<unique_ptr<TvSeries>> tvSerieses;
    ConfigSeriesReader srsReader(jsonCfg["tv_series"]);
    for (int i = 0; i < srsReader.getSeriesCount(); ++i) {
        tvSerieses.push_back(srsReader.readSeries(i));
    }

    for (int i = 0; i < tvSerieses.size(); ++i) {
        TvSeries& tvSeries = *tvSerieses[i];
        vector<int> seasons = tvSeries.getSeasons();
        for (int season = 0; season < seasons.size(); ++season) {
            int episodeCount = seasons[i];
            for (int episode = 0; episode < episodeCount; ++episode) {
                content.push_back(new Episode(
                        ++watchableId,
                        tvSeries.getName(),
                        tvSeries.getEpisodeLength(),
                        season + 1,
                        episode + 1,
                        tvSeries.getTags()
                        ));
            }
        }
    }
}

void Session::start() {

}

vector<Watchable*> Session::getContent() const {
    return content;
}
vector<BaseAction*> Session::getActionLog() const {
    return actionsLog;
}
Watchable* Session::getContentById(long id) {
    return content[id];
}

User* Session::getActiveUser() const {
    return activeUser;
}
User* Session::getUser(const string &name) {
    return userMap[name];
}
bool Session::addUser(User *user) {
    if (user == nullptr) {
        throw "User cannot be null.";
    }

    if (userMap.count(user->getName()) > 0) {
        return false;
    }

    userMap[user->getName()] = user;
    return true;
}
bool Session::changeActiveUser(User *user) {
    if (user == nullptr) {
        throw "User cannot be null.";
    }

    activeUser = user;
}
bool Session::removeUser(User *user) {
    if (user == nullptr) {
        throw "User cannot be null.";
    }

    userMap.erase(user->getName());
}

void Session::raiseExistFlag() {
    exitFlag = true;
}