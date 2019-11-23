#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"

//region BaseAction
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
void BaseAction::setArgs(std::vector<std::string> &args){
    this->args = &args;
}

/* Return a reference to the current args */
const std::vector<std::string>& BaseAction::getArgs() {
    return *args;
}
//endregion

//region CreateUser
/* Action that creates new user if this two condition are valid
 * 1. The given user name isn't exist.
 * 2. The recommendation code is valid.
*/
void CreateUser::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.size() < 2){
        error("Usage: createuser<user_name> <recommendation_algorithm>");
    } else {
        if(sess.getUser(vArgs[0]) == nullptr){
            User* newUser = User::createUser(vArgs[0], vArgs[1]);
            // The recommendation algorithem name is not valid
            if(newUser == nullptr){
                error("recommendation algorithm code is not valid");
            } else {
                sess.addUser(newUser);
                complete();
            }
        } else {
            error("the new user name is already taken");
        }
    }
}

/* Return a string representation of the action*/
std::string CreateUser::toString() const {
    if(getStatus() != ERROR){
        return "CreateUser" + std::to_string(getStatus());
    } else {
        return  "CreateUser" + getErrorMsg();
    }
}
//endregion