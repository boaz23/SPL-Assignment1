//
// Created by netanel on 24/11/2019.
//

#include "../include/User.h"

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
    newUser->history = history;
    return  newUser;
}

User *LengthRecommenderUser::clone() const {
    return createCopy(getName());
}



