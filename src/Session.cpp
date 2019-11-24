#include <string>
#include <fstream>
#include <sstream>
#include "../include/Session.h"
#include "../include/ConfigMovieReader.h"
#include "../include/ConfigSeriesReader.h"
#include "../include/User.h"

using namespace std;
using json = nlohmann::json;

// TODO:: Test move/copy assignments/constructors when merged with working code

void Session::clean() {
    for (auto &watchable : content) {
        delete watchable;
        watchable = nullptr;
    }
    for (auto &action : actionsLog) {
        delete action;
        action = nullptr;
    }
    for (auto &user : userMap) {
        delete user.second;
        user.second = nullptr;
    }
    for (auto &actionFactory : actionsFactory) {
        delete actionFactory.second;
        actionFactory.second = nullptr;
    }
    activeUser = nullptr;
}

void Session::steal(Session &other) {
    content = move(other.content);
    actionsLog = move(other.actionsLog);
    userMap = move(other.userMap);
    actionsFactory = move(other.actionsFactory);
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

    actionsFactory = unordered_map<string, ActionFactory*>();
    for (const auto& actionFactory : other.actionsFactory) {
        actionsFactory[actionFactory.first] = actionFactory.second->clone();
    }

    activeUser = userMap[other.activeUser->getName()];
    exitFlag = other.exitFlag;
}

Session::~Session() {
    clean();
}

Session::Session(const Session &other) : content(), actionsLog(), userMap(), activeUser(nullptr), exitFlag(false), actionsFactory() {
    copy(other);
}
Session::Session(Session &&rval) : content(), actionsLog(), userMap(), activeUser(nullptr), exitFlag(false), actionsFactory() {
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

Session::Session(const string &configFilePath) : content(), actionsLog(), userMap(), activeUser(nullptr), exitFlag(false), actionsFactory() {
    initializeFromConfig(configFilePath);
    
    cout << "SPLFLIX is now on!" << endl;
    if (userMap.count(DEFAULT_USER_NAME) == 0) {
        User* pUser = new LengthRecommenderUser(DEFAULT_USER_NAME);
        userMap[pUser->getName()] = pUser;
        activeUser = pUser;
    }

    actionsFactory["createuser"] = new CreateUserActionFactory();
    actionsFactory["changeuser"] = new ChangeActiveUserActionFactory();
    actionsFactory["deleteuser"] = new DeleteUserActionFactory();
    actionsFactory["dupuser"] = new DuplicateUserActionFactory();
    actionsFactory["content"] = new PrintContentListActionFactory();
    actionsFactory["watchhist"] = new PrintWatchHistoryActionFactory();
    actionsFactory["watch"] = new WatchActionFactory();
    actionsFactory["log"] = new PrintActionsLogActionFactory();
    actionsFactory["exit"] = new ExitActionFactory();
}

void Session::start() {
    exitFlag = false;
    while (!exitFlag) {
        string command;
        getline(cin, command);
        vector<string> words = splitStringBySpace(command);
        if (words.empty()) {
            cout << "Error - no command entered" << endl;
            continue;
        }

        string actionCmd = words[0];
        if (actionsFactory.count(actionCmd) == 0) {
            cout << "Error - unknown action entered" << endl;
            continue;
        }

        words.erase(words.begin());
        BaseAction* action = actionsFactory[actionCmd]->createAction();
        actionsLog.push_back(action);
        action->setArgs(words);
        action->act(*this);
    }
    exitFlag = false;
}

vector<Watchable*> Session::getContent() const {
    return content;
}
vector<BaseAction*> Session::getActionLog() const {
    return actionsLog;
}

void Session::addToActionLog(BaseAction &action) {
    actionsLog.push_back(&action);
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
bool Session::addUser(User &user) {
    if (userMap.count(user.getName()) > 0) {
        return false;
    }

    userMap[user.getName()] = &user;
    return true;
}
bool Session::changeActiveUser(User &user) {
    activeUser = &user;
    return true;
}
bool Session::removeUser(User &user) {
    userMap.erase(user.getName());
    return true;
}

void Session::raiseExistFlag() {
    exitFlag = true;
}

void Session::initializeFromConfig(const std::string &configFilePath) {
    ifstream ifsCfg(configFilePath);
    json jsonCfg;
    ifsCfg >> jsonCfg;
    initializeFromConfig(jsonCfg);
}
void Session::initializeFromConfig(const nlohmann::json &jsonCfg) {
    long watchableId = -1;

    ConfigMovieReader mvReader(jsonCfg["movies"]);
    for (int i = 0; i < mvReader.getMoviesCount(); ++i) {
        content.push_back(mvReader.readMovie(i, ++watchableId));
    }

    vector<unique_ptr<TvSeries>> tvSerieses;
    ConfigSeriesReader srsReader(jsonCfg["tv_series"]);
    for (int i = 0; i < srsReader.getSeriesCount(); ++i) {
        tvSerieses.push_back(srsReader.readSeries(i));
    }

    for (size_t i = 0; i < tvSerieses.size(); ++i) {
        TvSeries& tvSeries = *tvSerieses[i];
        vector<int> seasons = tvSeries.getSeasons();
        Episode* lastEpisode = nullptr;
        for (size_t season = 0; season < seasons.size(); ++season) {
            int episodeCount = seasons[season];
            for (int iEpisode = 0; iEpisode < episodeCount; ++iEpisode) {
                Episode* pEpisode = new Episode(
                        ++watchableId,
                        tvSeries.getName(),
                        tvSeries.getEpisodeLength(),
                        season + 1,
                        iEpisode + 1,
                        tvSeries.getTags()
                );
                pEpisode->setNextEpisodeId(watchableId + 1);
                lastEpisode = pEpisode;
                content.push_back(pEpisode);
            }
        }
        lastEpisode->setNextEpisodeId(-1);
    }
}

vector<string> Session::splitStringBySpace(string str) {
    istringstream iss(str);
    return vector<string>(istream_iterator<string>(iss),istream_iterator<string>());
}