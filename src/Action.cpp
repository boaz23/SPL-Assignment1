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
    this->errorMsg = errorMsg;
}

// TODO: figure out how to print the action's error when it happens: figure out how to pass it to session or print in BaseAction
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
        return "CreateUser " + std::to_string(getStatus());
    } else {
        return  "CreateUser ERROR: " + getErrorMsg();
    }
}

BaseAction *CreateUser::clone() {
    CreateUser *newAction = new CreateUser();
    copyData(*newAction);
    return newAction;
}
//endregion

//region ChangeActiveUser
/* Action that changes the active user if it exist.
*/
void ChangeActiveUser::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.size() != 1){
        error("Usage: changeuser <user_name>");
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
        return "ChangeActiveUser " + std::to_string(getStatus());
    } else {
        return  "ChangeActiveUser ERROR: " + getErrorMsg();
    }
}

BaseAction *ChangeActiveUser::clone() {
    ChangeActiveUser *newAction = new ChangeActiveUser();
    copyData(*newAction);
    return newAction;
}
//endregion

//region DeleteUser
/* Action that deletes the active user if it exist. */
void DeleteUser::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.size() != 1){
        error("Usage: deleteuser <user_name>");
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
        return "DeleteUser " + std::to_string(getStatus());
    } else {
        return  "DeleteUser ERROR: " + getErrorMsg();
    }
}

BaseAction *DeleteUser::clone() {
    DeleteUser *newAction = new DeleteUser();
    copyData(*newAction);
    return newAction;
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
        return "DuplicateUser " + std::to_string(getStatus());
    } else {
        return  "DuplicateUser ERROR: " + getErrorMsg();
    }
}

BaseAction *DuplicateUser::clone() {
    DeleteUser *newAction = new DeleteUser();
    copyData(*newAction);
    return newAction;
}
//endregion

//region PrintContentList
/* Action that prints the content list
 * <content_id> <content_name> <content_length>minutes[<tag_1>, <tag_2>, ..., <tag_n>] */
void PrintContentList::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(!vArgs.empty()) {
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
        return "PrintContentList " + std::to_string(getStatus());
    } else {
        return  "PrintContentList ERROR: " + getErrorMsg();
    }
}

BaseAction *PrintContentList::clone() {
    PrintContentList *newAction = new PrintContentList();
    copyData(*newAction);
    return newAction;
}
//endregion

//region PrintWatchHistory
/* Action that prints the watch history for the active user
 * <content_id> <content_name>  */
void PrintWatchHistory::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(!vArgs.empty()) {
        error("Usage: watchlist");
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
        return "PrintWatchHistory " + std::to_string(getStatus());
    } else {
        return  "PrintWatchHistory ERROR: " + getErrorMsg();
    }
}

BaseAction *PrintWatchHistory::clone() {
    PrintWatchHistory *newAction = new PrintWatchHistory();
    copyData(*newAction);
    return newAction;
}
//endregion

//region Watch
/* Action that prints the content list
 * <content_id> <content_name> <content_length>minutes[<tag_1>, <tag_2>, ..., <tag_n>] */
void Watch::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(vArgs.size() != 1) {
        error("Usage: watch <content_id>");
    } else {
        User &activeUser = *(sess.getActiveUser());
        std::vector<Watchable *> content = sess.getContent();

        std::string::size_type sz;
        unsigned long contentId = -1;
        try {
            contentId = std::stoi(vArgs[0], &sz);
        } catch (std::exception& e){
            error("The content id isn't valid");
        }

        // TODO: call session.getContentById(contentId)
        if(contentId >= 0 & contentId < content.size()) {
            std::cout << "Watching " << content[contentId]->toString() << "\n";
            activeUser.addToHistory(content[contentId]);
            complete();

            watchRecommendation(sess, activeUser);
        }
    }
}

/* Return a string representation of the action*/
std::string Watch::toString() const {
    if(getStatus() != ERROR){
        return "PrintWatchHistory " + std::to_string(getStatus());
    } else {
        return  "PrintWatchHistory ERROR: " + getErrorMsg();
    }
}

// TODO: call Watchable.getNextWatchable(Session) instead
void Watch::watchRecommendation(Session &sess, User &activeUser) {
    bool watchNext = true;
    Watchable *recommendation = activeUser.getRecommendation(sess);
    std::string input;

    // TODO: add null check (because there might be nothing recommended)
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
        } else if (input[0] == 'x') {
            watchNext = false;
        } else {
            error("invalid input");
        }
    } else {
        error("invalid input");
    }
}

BaseAction *Watch::clone() {
    Watch *newAction = new Watch();
    copyData(*newAction);
    return newAction;
}
//endregion

//region PrintActionsLog
/* Action that prints the action log for the active user
 * <action_tostring> <action status>  */
void PrintActionsLog::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(!vArgs.empty()) {
        error("Usage: log");
    } else {
        std::vector<BaseAction*> actions = sess.getActionLog();

        for(unsigned long i=0; i < actions.size(); i= i + 1){
            std::cout << actions[i]->toString() << " " << actions[i]->getStatus() << "\n";
        }
        complete();
    }
}

/* Return a string representation of the action*/
std::string PrintActionsLog::toString() const {
    if(getStatus() != ERROR){
        return "PrintActionsLog " + std::to_string(getStatus());
    } else {
        return  "PrintActionsLog ERROR: " + getErrorMsg();
    }
}

BaseAction *PrintActionsLog::clone() {
    PrintActionsLog *newAction = new PrintActionsLog();
    copyData(*newAction);
    return newAction;
}
//endregion

//region Exit
/* Action that end the session of the user */
void Exit::act(Session &sess) {

    const std::vector<std::string>& vArgs = getArgs();

    if(!vArgs.empty()) {
        error("Usage: exit");
    } else {
        sess.raiseExistFlag();
        complete();
    }
}

/* Return a string representation of the action*/
std::string Exit::toString() const {
    if(getStatus() != ERROR){
        return "Exit " + std::to_string(getStatus());
    } else {
        return  "Exit ERROR: " + getErrorMsg();
    }
}

BaseAction *Exit::clone() {
    Exit *newAction = new Exit();
    copyData(*newAction);
    return newAction;
}
//endregion