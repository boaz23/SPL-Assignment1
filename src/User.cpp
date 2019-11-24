//
// Created by netanel on 24/11/2019.
//

#include <algorithm>
#include "../include/User.h"
#include "../include/Watchable.h"

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

LengthRecommenderUser::LengthRecommenderUser(const std::string& name): User(name), totalWatchTime(0), watchedAll(false),
                                                                        avarage(0), historySize(0), contentNotWatched(){
}

// TODO: test
Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    // Create the vector for content that wasnt watch yed
    if(contentNotWatched.empty() && watchedAll == false){
        delete(&contentNotWatched);
        contentNotWatched = s.getContent();
    }

    unsigned long recIndex = 0;
    int best;

    if(!contentNotWatched.empty()) {
        best = avarage - contentNotWatched[0].getLenght();
        for (int i = 1; i < contentNotWatched.size(); i = i + 1) {
            if (avarage - contentNotWatched[i].getLenght() < best) {
                best = avarage - contentNotWatched[i].getLenght();
                recIndex = i;
            }
        }

        return contentNotWatched[recIndex];
    }

    return nullptr;
}

void LengthRecommenderUser::addToHistory(Watchable *watchable) {
    User::addToHistory(watchable);
    // TODO remove the watchable from the cotentNotWatched

    totalWatchTime = totalWatchTime + watchable->getLength();
    avarage = (int)(totalWatchTime / history.size());
    historySize = historySize + 1;
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