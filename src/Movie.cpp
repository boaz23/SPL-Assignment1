#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"


using namespace std;

Movie::Movie(long id, const string &name, int length, const vector<string> &tags) : Watchable(id, length, tags), name(name) {}

string Movie::toString() const {
    return name;
}

Watchable* Watchable::getNextWatchable(Session &session) const {
    return session.getActiveUser()->getRecommendation(session);
}

Watchable* Movie::clone() const {
    return new Movie(getId(), name, getLength(), getTags());
}

