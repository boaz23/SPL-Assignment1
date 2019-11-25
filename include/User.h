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
    virtual User* createCopy(const std::string &name) const = 0;
    virtual User* clone() const = 0;
    virtual void addToHistory(Watchable *watchable);
protected:
    std::vector<Watchable*> history;
private:
    std::string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* clone() const;
    virtual User* createCopy(const std::string &name) const;
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* createCopy(const std::string &name) const;
    virtual User* clone() const;
private:
    size_t historyIndex;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* createCopy(const std::string &name) const;
    virtual User* clone() const;
private:
};

#endif
