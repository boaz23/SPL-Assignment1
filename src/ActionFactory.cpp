#include "../include/ActionFactory.h"

ActionFactory::~ActionFactory() = default;

BaseAction* CreateUserActionFactory::createAction() {
    return new CreateUser();
//    return nullptr;
}
ActionFactory* CreateUserActionFactory::clone() {
    return new CreateUserActionFactory();
}

BaseAction* ChangeActiveUserActionFactory::createAction() {
    return new ChangeActiveUser();
//    return nullptr;
}
ActionFactory* ChangeActiveUserActionFactory::clone() {
    return new ChangeActiveUserActionFactory();
}

BaseAction* DeleteUserActionFactory::createAction() {
    return new DeleteUser();
//    return nullptr;
}
ActionFactory* DeleteUserActionFactory::clone() {
    return new DeleteUserActionFactory();
}

BaseAction* DuplicateUserActionFactory::createAction() {
    return new DuplicateUser();
//    return nullptr;
}
ActionFactory* DuplicateUserActionFactory::clone() {
    return new DuplicateUserActionFactory();
}

BaseAction* PrintContentListActionFactory::createAction() {
    return new PrintContentList();
//    return nullptr;
}
ActionFactory* PrintContentListActionFactory::clone() {
    return new PrintContentListActionFactory();
}

BaseAction* PrintWatchHistoryActionFactory::createAction() {
    return new PrintWatchHistory();
//    return nullptr;
}
ActionFactory* PrintWatchHistoryActionFactory::clone() {
    return new PrintWatchHistoryActionFactory();
}

BaseAction* WatchActionFactory::createAction() {
    return new Watch();
//    return nullptr;
}
ActionFactory* WatchActionFactory::clone() {
    return new WatchActionFactory();
}

BaseAction* PrintActionsLogActionFactory::createAction() {
    return new PrintActionsLog();
//    return nullptr;
}
ActionFactory* PrintActionsLogActionFactory::clone() {
    return new PrintActionsLogActionFactory();
}

BaseAction* ExitActionFactory::createAction() {
    return new Exit();
//    return nullptr;
}
ActionFactory* ExitActionFactory::clone() {
    return new ExitActionFactory();
}