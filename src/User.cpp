//
// Created by netanel on 24/11/2019.
//

#include <algorithm>
#include <stdlib.h>
#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"

//region User
User::User(const std::string &name): history(), name(name) {

}

std::string User::getName() const {
    return name;
}

std::vector<Watchable *> User::get_history() const {
    return history;
}

void User::addToHistory(Watchable *watchable) {
    if(watchable != nullptr){
        history.push_back(watchable->clone());
    }
}

User *User::createUser(const std::string &name, const std::string &recommendationAlgorithm) {
    if(recommendationAlgorithm == "len"){
        return new LengthRecommenderUser(name);
    } else if(recommendationAlgorithm == "rer"){
        return new RerunRecommenderUser(name);
    } else if (recommendationAlgorithm == "gen"){
        return new GenreRecommenderUser(name);
    } else {
        return nullptr;
    }
}

User::~User() {
    clean();
};

User::User(const User &other) : history() {
    copy(other);
}

User::User(User &&rval) : history() {
    steal(rval);
}

User& User::operator=(const User &other) {
    if (&other != this) {
        clean();
        copy(other);
    }
    return *this;
}
User& User::operator=(User &&rval) {
    if (&rval != this) {
        clean();
        steal(rval);
    }
    return *this;
}

void User::clean() {
    for (auto &watchable : history) {
        delete watchable;
        watchable = nullptr;
    }
}

void User::copy(const User &other) {
    history = std::vector<Watchable*>();
    for (auto pWatchable : other.history) {
        history.push_back(pWatchable->clone());
    }
}

void User::steal(User &other) {
    history = std::move(other.history);
}
//endregion

LengthRecommenderUser::LengthRecommenderUser(const std::string& name): User(name){
}

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    const std::vector<Watchable*>& content = s.getContent();
    if(!history.empty()) {
        std::vector<bool> watched(content.size());
        for (size_t i = 0; i < watched.size(); i = i + 1) {
            watched[i] = false;
        }

        unsigned long sum = 0;
        for (size_t i = 0; i < history.size(); i = i + 1) {
            watched[history[i]->getId()] = true;
            sum = sum + history[i]->getLength();
        }

        double avarage = double((double)sum / history.size());
        bool found = false;
        size_t recIndex = -1;
        double recVal = INFINITY;
        for (size_t i = 0; i < content.size(); i = i + 1) {
            Watchable *watchable = content[i];
            if (watched[watchable->getId()]) {
                continue;
            }

            double dist = abs(avarage - watchable->getLength());
            if(dist < recVal){
                found = true;
                recIndex = i;
                recVal = dist;
            }
        }

        if (!found) {
            return nullptr;
        }
        return content[recIndex];
    }

    return nullptr;
}

User *LengthRecommenderUser::createCopy(const std::string &name) const {
    LengthRecommenderUser *newUser = new LengthRecommenderUser(name);
    newUser->history = history;
    return  newUser;
}

User *LengthRecommenderUser::clone() const {
    return createCopy(getName());
}

LengthRecommenderUser::~LengthRecommenderUser() = default;
LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &other) : User(other) {}
LengthRecommenderUser::LengthRecommenderUser(LengthRecommenderUser &&rval) : User(std::move(rval)) {}
LengthRecommenderUser& LengthRecommenderUser::operator=(const LengthRecommenderUser &other) {
    if (&other != this) {
        this->User::operator=(other);
    }
    return *this;
}
LengthRecommenderUser& LengthRecommenderUser::operator=(LengthRecommenderUser &&rval) {
    if (&rval != this) {
        this->User::operator=(std::move(rval));
    }
    return *this;
}

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name), historyIndex(-1) {}

User* RerunRecommenderUser::createCopy(const std::string &name) const {
    RerunRecommenderUser *duplicate = new RerunRecommenderUser(name);
    duplicate->history = history;
    return duplicate;
}

User* RerunRecommenderUser::clone() const {
    return createCopy(getName());
}

Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    if (history.empty()) {
        return nullptr;
    }

    ++historyIndex;
    historyIndex = historyIndex % history.size();
    return history[historyIndex];
}

RerunRecommenderUser::~RerunRecommenderUser() = default;
RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser &other) : User(other) {}
RerunRecommenderUser::RerunRecommenderUser(RerunRecommenderUser &&rval) : User(std::move(rval)) {}
RerunRecommenderUser& RerunRecommenderUser::operator=(const RerunRecommenderUser &other) {
    if (&other != this) {
        this->User::operator=(other);
    }
    return *this;
}
RerunRecommenderUser& RerunRecommenderUser::operator=(RerunRecommenderUser &&rval) {
    if (&rval != this) {
        this->User::operator=(std::move(rval));
    }
    return *this;
}

GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name) { }

User* GenreRecommenderUser::createCopy(const std::string &name) const {
    GenreRecommenderUser *duplicate = new GenreRecommenderUser(name);
    duplicate->history = history;
    return duplicate;
}

User* GenreRecommenderUser::clone() const {
    return createCopy(getName());
}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    const std::vector<Watchable*>& content = s.getContent();

    std::vector<bool> watched(content.size());
    std::unordered_map<std::string, int> tagsPopularityMap;
    for (const auto &watchable : history) {
        watched[watchable->getId()] = true;
        for (const auto &tag : watchable->getTags()) {
            ++tagsPopularityMap[tag];
        }
    }

    std::vector<std::tuple<Watchable*, std::string, int, int>> candidates;
    for (size_t i = 0; i < content.size(); ++i) {
        Watchable *watchable = content[i];
        if (watched[watchable->getId()]) {
            continue;
        }

        const std::vector<std::string> &watchableTags = watchable->getTags();
        std::string mostPopularTag = *std::max_element(watchableTags.begin(), watchableTags.end(), [&tagsPopularityMap](const std::string &a, const std::string &b) {
            if (tagsPopularityMap[a] == tagsPopularityMap[b]) {
                return a > b;
            }

            return tagsPopularityMap[a] < tagsPopularityMap[b];
        });
        candidates.emplace_back(content[i], mostPopularTag, tagsPopularityMap[mostPopularTag], i);
    }

    std::sort(candidates.begin(), candidates.end(), [](const std::tuple<Watchable*, std::string, int, int> &a, const std::tuple<Watchable*, std::string, int, int> &b) {
        if (std::get<2>(a) == std::get<2>(b)) {
            if (std::get<1>(a) == std::get<1>(b)) {
                return std::get<3>(a) > std::get<3>(b);
            }

            return std::get<1>(a) > std::get<1>(b);
        }

        return std::get<2>(a) < std::get<2>(b);
    });

    if (candidates.empty()) {
        return nullptr;
    }
    return std::get<0>(candidates[candidates.size() - 1]);
}

GenreRecommenderUser::~GenreRecommenderUser() = default;
GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser &other) : User(other) {}
GenreRecommenderUser::GenreRecommenderUser(GenreRecommenderUser &&rval) : User(std::move(rval)) {}
GenreRecommenderUser& GenreRecommenderUser::operator=(const GenreRecommenderUser &other) {
    if (&other != this) {
        this->User::operator=(other);
    }
    return *this;
}
GenreRecommenderUser& GenreRecommenderUser::operator=(GenreRecommenderUser &&rval) {
    if (&rval != this) {
        this->User::operator=(std::move(rval));
    }
    return *this;
}