#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    std::vector<Watchable*> getContent() const;
    std::vector<BaseAction*> getActionLog() const;
    Watchable* getContentById(long id);
    User* getActiveUser() const;
    User* getUser(const std::string &name);
    bool addUser(User &user);
    bool changeActiveUser(User &user);
    bool removeUser(User &user);
    void raiseExistFlag();

    Session(const Session &other);
    Session(Session &&rval);
    Session& operator=(const Session &other);
    Session& operator=(Session &&rval);
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    bool exitFlag;

    void clean();
    void steal(Session &other);
    void copy(const Session &other);
};
#endif
