#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <vector>
#include <iostream>


class Session;
class User;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
	BaseAction();
	ActionStatus getStatus() const;
	virtual void act(Session& sess)=0;
	virtual std::string toString() const=0;
    void setArgs(std::vector<std::string> &args);
    const std::vector<std::string>& getArgs();
protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;
private:
	std::string errorMsg;
	ActionStatus status;
    std::vector<std::string>* args;
};

class CreateUser  : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
};


class DuplicateUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
};

class PrintContentList : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
private:
    virtual void watchRecomandtion(Session &sess, User *activeUser);
};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};
#endif
