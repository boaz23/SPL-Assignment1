#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include "ActionFactory.h"
#include "json.hpp"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    const std::vector<Watchable*>& getContent() const;
    const std::vector<BaseAction*>& getActionLog() const;
    Watchable* getContentById(size_t id);
    void addToActionLog(BaseAction &action);
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
    std::unordered_map<std::string, ActionFactory*> actionsFactory;

    const std::string DEFAULT_USER_NAME = "default";

    void clean();
    void steal(Session &other);
    void copy(const Session &other);

    void initializeFromConfig(const std::string &configFilePath);
    void initializeFromConfig(const nlohmann::json &jsonCfg);

    static std::vector<std::string> splitStringBySpace(std::string str);
};
#endif
