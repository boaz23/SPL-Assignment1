#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <vector>
#include <iostream>
#include "../include/Watchable.h"


class Session;
class User;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};

class BaseAction{
public:
	BaseAction();
	virtual ~BaseAction();
	ActionStatus getStatus() const;
	virtual void act(Session& sess)=0;
	virtual std::string toString() const=0;
    void setArgs(const std::vector<std::string> &args);
    const std::vector<std::string>& getArgs();
    virtual BaseAction* clone() = 0;
    virtual void copyData(BaseAction& copyTo);
    const std::string& getErrMsg() const;
    static const std::string& getStatusName(ActionStatus status);
protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;
private:
    static std::string statusesNames[];
	std::string errorMsg;
	ActionStatus status;
    std::vector<std::string> args;
};

class CreateUser  : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();
};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};


class DuplicateUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class PrintContentList : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() ;
private:
    virtual void watchRecommendation(Session &sess, Watchable *watchable);

};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone() ;
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone() ;
};
#endif
