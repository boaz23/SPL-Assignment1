//
// Created by netanel on 24/11/2019.
//

#include <algorithm>
#include "../include/User.h"
#include "../include/Watchable.h"
#include <stdlib.h>

//region User
User::User(const std::string &name): name(name), history(){

}

User::~User() = default;

std::string User::getName() const {
    return name;
}

std::vector<Watchable *> User::get_history() const {
    return history;
}

void User::addToHistory(Watchable *watchable) {
    if(watchable != nullptr){
        history.push_back(watchable);
    }
}

User *User::createUser(const std::string &name, const std::string &recommendationAlgorithm) {
    if(recommendationAlgorithm.compare("len") == 0){
        return new LengthRecommenderUser(name);
    } else if(recommendationAlgorithm.compare("rer")){
        return new RerunRecommenderUser(name);
    } else if(recommendationAlgorithm.compare("gen")){
        return new GenreRecommenderUser(name);
    } else {
        return nullptr;
    }
}
//endregion

LengthRecommenderUser::LengthRecommenderUser(const std::string& name): User(name){
}

// TODO: test
Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    int avarage = 0;
    unsigned long sum = 0;

    std::vector<Watchable*> content = s.getContent();
    if(!history.empty()) {
        std::vector<bool> watched(content.size());
        for (unsigned long i = 0; i < watched.size(); i = i + 1) {
            watched[i] = false;
        }

        for (unsigned long i = 0; i < history.size(); i = i + 1) {
            watched[history[i]->getId()] = true;
            sum = sum + history[i]->getLength();
        }

        avarage = int(sum / history.size());
        unsigned long recIndex = 0;
        int recVal = abs(avarage - history[0]->getLength());
        for (unsigned long i = 1; i < history.size(); i = i + 1) {
            if(abs(avarage - history[i]->getLength()) < recVal){
                recIndex = i;
                recVal = abs(avarage - history[i]->getLength());
            }
        }

        return history[recIndex];
    }

    return nullptr;
}

void LengthRecommenderUser::addToHistory(Watchable *watchable) {
}

User *LengthRecommenderUser::createCopy(const std::string &name) const {
    LengthRecommenderUser *newUser = new LengthRecommenderUser(name);
    // TODO: check it actually copies rather than moves
    newUser->history = history;
    return  newUser;
}

User *LengthRecommenderUser::clone() const {
    return createCopy(getName());
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

    historyIndex = (++historyIndex) % history.size();
    return history[historyIndex];
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

// TODO: test
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    std::vector<Watchable*> content = s.getContent();

    // TODO: check if vector initializes all elements to default value
    std::vector<bool> watched(content.size());
    std::unordered_map<std::string, int> tagsPopularityMap;
    for (const auto &watchable : history) {
        watched[watchable->getId()] = true;
        for (const auto &tag : watchable->getTags()) {
            ++tagsPopularityMap[tag];
        }
    }

    std::vector<std::tuple<Watchable*, std::string, int, int>> candidates(content.size());
    for (size_t i = 0; i < content.size(); ++i) {
        Watchable *watchable = content[i];
        if (watched[watchable->getId()]) {
            continue;
        }

        std::vector<std::string> watchableTags = watchable->getTags();
        std::string mostPopularTag = *std::max_element(watchableTags.begin(), watchableTags.end(), [&tagsPopularityMap](const std::string &a, const std::string &b) {
            if (tagsPopularityMap[a] == tagsPopularityMap[b]) {
                return a < b;
            }

            return tagsPopularityMap[a] < tagsPopularityMap[b];
        });
        candidates[i] = std::make_tuple(content[i], mostPopularTag, tagsPopularityMap[mostPopularTag], i);
    }

    std::sort(candidates.begin(), candidates.end(), [](const std::tuple<Watchable*, std::string, int, int> &a, const std::tuple<Watchable*, std::string, int, int> &b) {
        if (std::get<2>(a) == std::get<2>(b)) {
            if (std::get<1>(a) == std::get<1>(b)) {
                return std::get<3>(a) < std::get<3>(b);
            }

            return std::get<1>(a) < std::get<1>(b);
        }

        return std::get<2>(a) < std::get<2>(b);
    });

    if (candidates.empty()) {
        return nullptr;
    }
    return std::get<0>(candidates[candidates.size() - 1]);
}