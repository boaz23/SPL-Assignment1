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

    if(vArgs.size() != 2){
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

//region ChangeActiveUser
/* Action that changes the active user if it exist.
*/
void ChangeActiveUser::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.size() != 1){
        error("changeuser <user_name>");
    } else {
        if(sess.changeActiveUser(sess.getUser(vArgs[0]))){
            complete();

        } else {
            error("the user name doesnt exists");
        }
    }
}

/* Return a string representation of the action*/
std::string ChangeActiveUser::toString() const {
    if(getStatus() != ERROR){
        return "ChangeActiveUser" + std::to_string(getStatus());
    } else {
        return  "ChangeActiveUser" + getErrorMsg();
    }
}
//endregion

//region DeleteUser
/* Action that deletes the active user if it exist. */
void DeleteUser::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.size() != 1){
        error("deleteuser <user_name>");
    } else {
        // TODO check if the active user is equal to the given one
        if(sess.removeUser(sess.getUser(vArgs[0]))){
            complete();

        } else {
            error("the user name doesnt exists");
        }
    }
}

/* Return a string representation of the action*/
std::string DeleteUser::toString() const {
    if(getStatus() != ERROR){
        return "DeleteUser" + std::to_string(getStatus());
    } else {
        return  "DeleteUser" + getErrorMsg();
    }
}
//endregion

//region DuplicateUser
/* Action that duplicates a user, with all of its data,
 * except the username. */
void DuplicateUser::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.size() != 2){
        error("Usage: dupuser <original_user_name> <new_user_name>");
    } else {
        User* toDuplicate = sess.getUser(vArgs[0]);
        if(toDuplicate != nullptr){
                if(sess.getUser(vArgs[1]) == nullptr) {
                    User* duplicatedUser = toDuplicate->createCopy(vArgs[1]);
                    sess.addUser(duplicatedUser);
                    complete();
                } else {
                    error("the duplicated user name is already taken");
                }
        } else {
            error("the original user name isn't exist");
        }
    }
}

/* Return a string representation of the action*/
std::string DuplicateUser::toString() const {
    if(getStatus() != ERROR){
        return "DuplicateUser" + std::to_string(getStatus());
    } else {
        return  "DuplicateUser" + getErrorMsg();
    }
}
//endregion