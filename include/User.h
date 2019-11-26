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
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    static User* createUser(const std::string &name, const std::string &recommendationAlgorithm);
    virtual User* createCopy(const std::string &name) const = 0;
    virtual User* clone() const = 0;
    virtual void addToHistory(Watchable *watchable);

    virtual ~User();
    User(const User &other);
    User(User &&rval);
    User& operator=(const User &other);
    User& operator=(User &&rval);
protected:
    std::vector<Watchable*> history;
private:
    std::string name;

    void clean();
    void steal(User &other);
    void copy(const User &other);
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* clone() const;
    virtual User* createCopy(const std::string &name) const;

    ~LengthRecommenderUser() override;
    LengthRecommenderUser(const LengthRecommenderUser &other);
    LengthRecommenderUser(LengthRecommenderUser &&rval);
    LengthRecommenderUser& operator=(const LengthRecommenderUser &other);
    LengthRecommenderUser& operator=(LengthRecommenderUser &&rval);
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* createCopy(const std::string &name) const;
    virtual User* clone() const;

    ~RerunRecommenderUser() override;
    RerunRecommenderUser(const RerunRecommenderUser &other);
    RerunRecommenderUser(RerunRecommenderUser &&rval);
    RerunRecommenderUser& operator=(const RerunRecommenderUser &other);
    RerunRecommenderUser& operator=(RerunRecommenderUser &&rval);
private:
    size_t historyIndex;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* createCopy(const std::string &name) const;
    virtual User* clone() const;

    ~GenreRecommenderUser() override;
    GenreRecommenderUser(const GenreRecommenderUser &other);
    GenreRecommenderUser(GenreRecommenderUser &&rval);
    GenreRecommenderUser& operator=(const GenreRecommenderUser &other);
    GenreRecommenderUser& operator=(GenreRecommenderUser &&rval);
private:
    static std::string toLowerCase(const std::string &str);
};

#endif
