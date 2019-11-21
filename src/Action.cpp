#include "../include/Action.h"

/* Basic constructor of the class */
BaseAction::BaseAction() : errorMsg(""), status(PENDING), args(nullptr){}

/* Return the status of the action */
ActionStatus BaseAction::getStatus() const {
    return status;
}

/* Set the status of the class to COMPLETED */
void BaseAction::complete() {
    status = COMPLETED;
}

/* Set the status of the class to ERROR and set errorMsg
 * with the given string */
void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = "ERROR" + errorMsg;
}

/* Return the error message if there is one */
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

/* Copy the args to the vArgs of the class. */
void BaseAction::setArgs(std::vector<const std::string> &args){
    this->args = &args;
}

/* Return a reference to the current args */
std::vector<const std::string>& BaseAction::getArgs() {
    return *args;
}