#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

#include <exception>

//region BaseAction
/* Basic constructor of the class */
BaseAction::BaseAction() : errorMsg(""), status(PENDING), args(){}

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
void BaseAction::setArgs( const std::vector<std::string> &args){
    this->args = args;
}

/* Return a reference to the current args */
const std::vector<std::string>& BaseAction::getArgs() {
    return args;
}

void BaseAction::copyData(BaseAction &copyTo) {
    copyTo.setArgs(getArgs());
    copyTo.status = status;
    copyTo.errorMsg = errorMsg;
}

BaseAction::~BaseAction() = default;
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
            User *newUser = User::createUser(vArgs[0], vArgs[1]);
            // The recommendation algorithem name is not valid
            if(newUser == nullptr){
                error("recommendation algorithm code is not valid");
            } else {
                sess.addUser(*newUser);
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
        if(sess.changeActiveUser(*sess.getUser(vArgs[0]))){
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
        if(sess.removeUser(*sess.getUser(vArgs[0]))){
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
                    sess.addUser(*duplicatedUser);
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

//region PrintContentList
/* Action that prints the content list
 * <content_id> <content_name> <content_length>minutes[<tag_1>, <tag_2>, ..., <tag_n>] */
void PrintContentList::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.size() != 2) {
        error("Usage: content");
    } else {
        User *activeUser = sess.getActiveUser();
        std::vector<Watchable *> userHistory = activeUser->get_history();
        std::vector<std::string> tagString;

        //In treading insert the size() to the for
        unsigned long historySize = userHistory.size();
        for (unsigned long i = 0; i < historySize; i = i + 1) {
            Watchable &tmpWachable = *userHistory[i];

            printf("%lu %s %d minutes ", i+1, tmpWachable.toString().c_str(),
                   tmpWachable.getLength());

            std::cout << "[";
            tagString = tmpWachable.getTags();
            for (unsigned long j = 0; j < tagString.size() - 1; j = j + 1) {
                std::cout << tagString[j] << ",";
            }
            std::cout << tagString[tagString.size()] << "]\n";
        }
        complete();
    }
}

/* Return a string representation of the action*/
std::string PrintContentList::toString() const {
    if(getStatus() != ERROR){
        return "PrintContentList" + std::to_string(getStatus());
    } else {
        return  "PrintContentList" + getErrorMsg();
    }
}
//endregion

//region PrintWatchHistory
/* Action that prints the watch history for the active user
 * <content_id> <content_name>  */
void PrintWatchHistory::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.size() != 2) {
        error("Usage: watchhist");
    } else {
        User *activeUser = sess.getActiveUser();
        std::vector<Watchable *> userHistory = activeUser->get_history();

        std::cout << "Watch history for " << activeUser->getName() << "\n";
        for(unsigned long i=0; i < userHistory.size(); i=i+1){
            std::cout << i+1 << ". " << userHistory[i]->toString() << "\n";
        }
        complete();
    }
}

/* Return a string representation of the action*/
std::string PrintWatchHistory::toString() const {
    if(getStatus() != ERROR){
        return "PrintWatchHistory" + std::to_string(getStatus());
    } else {
        return  "PrintWatchHistory" + getErrorMsg();
    }
}
//endregion

//region Watch
/* Action that prints the content list
 * <content_id> <content_name> <content_length>minutes[<tag_1>, <tag_2>, ..., <tag_n>] */
void Watch::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.size() != 1) {
        error("Usage: watch<content_id>");
    } else {
        User &activeUser = *(sess.getActiveUser());
        std::vector<Watchable *> content = sess.getContent();

        std::string::size_type sz;
        unsigned long cotentId = -1;
        try {
            cotentId = std::stoi(vArgs[0], &sz);
        } catch (std::exception& e){
            error("The content id isn't valid");
        }

        if(cotentId > 0 & cotentId <= content.size()){
            std::cout << "Watching " << content[cotentId]->toString() << "\n";
            activeUser.addToHistory(content[cotentId]);
            complete();

            watchRecommendation(sess, activeUser);
        }
    }
}

/* Return a string representation of the action*/
std::string Watch::toString() const {
    if(getStatus() != ERROR){
        return "PrintWatchHistory" + std::to_string(getStatus());
    } else {
        return  "PrintWatchHistory" + getErrorMsg();
    }
}

void Watch::watchRecommendation(Session &sess, User &activeUser) {
    bool watchNext = true;
    Watchable *recommendation = activeUser.getRecommendation(sess);
    std::string input;

    std::cout << "We recommend watching " << recommendation->toString() << ", continue watching? [y/n]\n";
    std::cin >> input;
    if(input.size() == 1){
        if(input[0] == 'y'){
            BaseAction &watchRec = *(new Watch());

            std::vector<std::string> arg;
            arg.push_back(std::to_string(recommendation->getId()));
            setArgs(arg);

            sess.addToActionLog( watchRec);
            watchRec.act(sess);
        } else {
            watchNext = false;
        }
    } else {
        error("invalid input");
    }
}
//endregion

//region PrintActionsLog
/* Action that prints the action log for the active user
 * <action_tostring> <action status>  */
void PrintActionsLog::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.empty()) {
        error("Usage: log");
    } else {
        User *activeUser = sess.getActiveUser();
        std::vector<BaseAction*> userActions = sess.getActionLog();

        for(unsigned long i=0; i < userActions.size(); i=i+1){
            std::cout << userActions[i]->toString() << " " << userActions[i]->getStatus() << "\n";
        }
        complete();
    }
}

/* Return a string representation of the action*/
std::string PrintActionsLog::toString() const {
    if(getStatus() != ERROR){
        return "PrintActionsLog" + std::to_string(getStatus());
    } else {
        return  "PrintActionsLog" + getErrorMsg();
    }
}
//endregion

//region Exit
/* Action that end the session of the user */
void Exit::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.empty()) {
        error("Usage: Exit");
    } else {
        sess.raiseExistFlag();
        complete();
    }
}

/* Return a string representation of the action*/
std::string Exit::toString() const {
    if(getStatus() != ERROR){
        return "Exit" + std::to_string(getStatus());
    } else {
        return  "Exit" + getErrorMsg();
    }
}
//endregion