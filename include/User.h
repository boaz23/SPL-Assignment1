#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual ~User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    static User* createUser(const std::string &name, const std::string &recommendationAlgorithm);
    virtual User* createCopy(const std::string &name) const = 0; // TODO add to draw io
    virtual User* clone() const = 0;
    virtual void addToHistory(Watchable *watchable); // TODO add to draw io
protected:
    std::vector<Watchable*> history;
private:
    const std::string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* clone() const;
    virtual User* createCopy(const std::string &name) const;
    virtual void addToHistory(Watchable *watchable);
private:
    unsigned long totalWatchTime; // TODO add to draw io
    int avarage;
    unsigned long historySize;
    std::vector<Watchable *> contentNotWatched;
    bool watchedAll;

};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* clone() const;
private:
    int historyIndex;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* clone() const;
private:
};

#endif
