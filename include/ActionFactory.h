#ifndef ASS1_ACTIONFACTORY_H
#define ASS1_ACTIONFACTORY_H

#include "Action.h"

class ActionFactory {
public:
    virtual ~ActionFactory();
    virtual BaseAction* createAction() = 0;
    virtual ActionFactory* clone() = 0;
};

class CreateUserActionFactory  : public ActionFactory {
public:
    virtual BaseAction* createAction();
    virtual ActionFactory* clone();
};

class ChangeActiveUserActionFactory : public ActionFactory {
public:
    virtual BaseAction* createAction();
    virtual ActionFactory* clone();
};

class DeleteUserActionFactory : public ActionFactory {
public:
    virtual BaseAction* createAction();
    virtual ActionFactory* clone();
};


class DuplicateUserActionFactory : public ActionFactory {
public:
    virtual BaseAction* createAction();
    virtual ActionFactory* clone();
};

class PrintContentListActionFactory : public ActionFactory {
public:
    virtual BaseAction* createAction();
    virtual ActionFactory* clone();
};

class PrintWatchHistoryActionFactory : public ActionFactory {
public:
    virtual BaseAction* createAction();
    virtual ActionFactory* clone();
};


class WatchActionFactory : public ActionFactory {
public:
    virtual BaseAction* createAction();
    virtual ActionFactory* clone();
};


class PrintActionsLogActionFactory : public ActionFactory {
public:
    virtual BaseAction* createAction();
    virtual ActionFactory* clone();
};

class ExitActionFactory : public ActionFactory {
public:
    virtual BaseAction* createAction();
    virtual ActionFactory* clone();
};
#endif