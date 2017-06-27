//
// Created by Aozak on 2017/6/20.
//
// Target: 800 lines of code
//

#include "functions.h"
#include "wininet_errno.h"

int ftpOpt::connect(string url, string username, string password, int port, bool ftpPassive, bool keepDir){

    //Open Internet session
    hInternet = InternetOpen(nullptr, INTERNET_OPEN_TYPE_DIRECT,
                             nullptr, nullptr, 0);
    if(hInternet == nullptr){
        //On error goto
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }

    //Attempt to connect
    hFtpSession = InternetConnect(hInternet, url.c_str(), (INTERNET_PORT)port,
                                  username.c_str(), password.c_str(),
                                  INTERNET_SERVICE_FTP, ftpPassive?INTERNET_FLAG_PASSIVE:0, 0);
    if(hFtpSession == nullptr){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }

    //OK
    ftpOpt_currHost = url;
    ftpOpt_currUsr = username;
    ftpOpt_currPass = password;
    ftpOpt_currPort = port;
    ftpOpt_ftpPassive = ftpPassive;
    ftpOpt_connected = true;
    if(!keepDir){
        updateCurrDir();
    }
    return 0;
}

int ftpOpt::updateCurrDir(){
    /*
     * Private function.
     * Used for update current directory.
     */
    char tmp[_OOP_FTPCLIENT_PATH_SIZE];
    DWORD currDirSize = _OOP_FTPCLIENT_PATH_SIZE;

    if(!FtpGetCurrentDirectory(hFtpSession, tmp, &currDirSize)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        ftpOpt_currDir = "undefined";
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }

    ftpOpt_currDir = tmp;
    return 0;
}

int ftpOpt::ls(){

    /*
     * List all item in this directory.
     * It is VERY complex when we made it with wininet.
     */

    //Define temp container
    HINTERNET hFind;
    WIN32_FIND_DATA fd;
    char currDir[_OOP_FTPCLIENT_PATH_SIZE];
    strcpy(currDir, ftpOpt_currDir.c_str());

    //Get file!
    hFind = FtpFindFirstFile(hFtpSession, currDir, &fd, INTERNET_FLAG_RESYNCHRONIZE, 0);

    if(hFind != NULL){
        do{
            console(fd.cFileName);
        }while(InternetFindNextFile(hFind, &fd));
    }else{
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }

    /*
     * After calling FtpFindFirstFile and until calling
     * InternetCloseHandle, the application cannot call
     * FtpFindFirstFile again on the given FTP session
     * handle. If a call is made to FtpFindFirstFile on
     * that handle, the function fails with
     * ERROR_FTP_TRANSFER_IN_PROGRESS. After the calling
     * application has finished using the HINTERNET
     * handle returned by FtpFindFirstFile, it must be
     * closed using the InternetCloseHandle function.
     */

    //Do some clean works (also means reconnect)
    disconnect();
    connect(ftpOpt_currHost, ftpOpt_currUsr, ftpOpt_currPass, ftpOpt_currPort, ftpOpt_ftpPassive, true);
    cd(ftpOpt_currDir);
    cdHappened = false;

    return 0;
}

int ftpOpt::cd(string dir){

    /*
     * Change the current working directory.
     * Seems easy than ls xd.
     */

    char charDir[_OOP_FTPCLIENT_PATH_SIZE];
    strcpy(charDir, dir.c_str());

    if(!FtpSetCurrentDirectory(hFtpSession, charDir)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    };

    //We must get current directory anytime.
    updateCurrDir();
    return 0;
}

int ftpOpt::ftpGet(string pRemote, string pLocal, bool overwrite){

    /*
     * Get files from remote server.
     */

    char pathRemote[_OOP_FTPCLIENT_PATH_SIZE];
    char pathLocal[_OOP_FTPCLIENT_PATH_SIZE];
    strcpy(pathRemote, pRemote.c_str());
    strcpy(pathLocal, pLocal.c_str());
    if(!FtpGetFile(
            hFtpSession,
            pathRemote,
            pathLocal,
            (BOOL)!overwrite,
            FILE_ATTRIBUTE_ARCHIVE,
            (DWORD)ftpOpt_transferMode,
            0)
      )
    {
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    return 0;
}

int ftpOpt::ftpPut(string pLocal, string pRemote){

    /*
     * Put files to remote server.
     */

    char pathRemote[_OOP_FTPCLIENT_PATH_SIZE];
    char pathLocal[_OOP_FTPCLIENT_PATH_SIZE];
    strcpy(pathRemote, pRemote.c_str());
    strcpy(pathLocal, pLocal.c_str());
    if(!FtpPutFile(
            hFtpSession,
            pathLocal,
            pathRemote,
            (DWORD)ftpOpt_transferMode,
            0)
      )
    {
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    return 0;
}

int ftpOpt::mkdir(string dir){

    /*
     * Make directory on the remote server.
     */

    char dirName[_OOP_FTPCLIENT_PATH_SIZE];
    strcpy(dirName, dir.c_str());
    if(!FtpCreateDirectory(hFtpSession, dirName)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    return 0;
}

int ftpOpt::rm(string file){

    /*
     * Remove files from remote server.
     */

    char fileName[_OOP_FTPCLIENT_PATH_SIZE];
    strcpy(fileName, file.c_str());
    if(!FtpDeleteFile(hFtpSession, fileName)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    return 0;
}

int ftpOpt::rmdir(string dir){

    /*
     * Remove directory from remote server.
     */

    char dirName[_OOP_FTPCLIENT_PATH_SIZE];
    strcpy(dirName, dir.c_str());
    if(!FtpRemoveDirectory(hFtpSession, dirName)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    return 0;
}

int ftpOpt::rename(string oldFile, string newFile){

    /*
     * Rename files on the remote server.
     */

    char oldFileName[_OOP_FTPCLIENT_PATH_SIZE];
    char newFileName[_OOP_FTPCLIENT_PATH_SIZE];
    strcpy(oldFileName, oldFile.c_str());
    strcpy(newFileName, newFile.c_str());
    if(!FtpRenameFile(hFtpSession, oldFileName, newFileName)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    return 0;
}

int ftpOpt::disconnect(){

    /*
     * Disconnect from the server.
     */

    if(!(bool)InternetCloseHandle(hFtpSession)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    if(!(bool)InternetCloseHandle(hInternet)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    };
    ftpOpt_connected = false;
    return 0;
}

string ftpOpt::currDir(){
    return ftpOpt_currDir;
}

string ftpOpt::currHost(){
    return ftpOpt_currHost;
}

string ftpOpt::currUsr(){
    return ftpOpt_currUsr;
}

bool ftpOpt::isConnected(){
    return ftpOpt_connected;
}

int ftpOptTerminal::init(ftpOpt obj){
    object = obj;
}

string ftpOptTerminal::wait(){
    if(!firstLine){
        cout << endl;
    }else{
        firstLine = false;
    }
    if(object.isConnected()){
        console(object.currDir() + ">", false);
    }else{
        console("ftp>", false);
    }
    char str[_OOP_FTPCLIENT_TERMINAL_SIZE];
    cin.getline(str,_OOP_FTPCLIENT_TERMINAL_SIZE);
    string strObj = str;
    return strObj;
}

int ftpOptTerminal::parse(string cmdStr){
    vector<string> strArray = split(cmdStr, ' ');
    if (strArray[0] == "exit") {
        return _OOP_FTPCLIENT_TERMINAL_EXIT;
    }
    if (isElementOf(strArray[0], {"connect", "link"})) {
        //default argument init
        string url = "ftp.example.com";
        int port = 21;
        string username;
        string password;
        bool ftpPassive = true;
        for (int i=1;i<strArray.size();i++) {
            if (strArray[i].substr(0, 1) == "-" && i + 1 < strArray.size() && strArray[i+1].substr(0, 1) != "-") {
                //if it is an argument
                if (isElementOf(strArray[i].substr(1),
                                {"username", "user", "usr", "u"})) {
                    username = strArray[i + 1];
                } else if (isElementOf(strArray[i].substr(1),
                                {"password", "pass", "pw", "p"})) {
                    password = strArray[i + 1];
                } else if (isElementOf(strArray[i].substr(1),
                                {"port", "pt", "v", ":", ";"})) {
                    port = toNumber(strArray[i + 1]);
                } else {
                    console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                    return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
                }
                i++;
            } else if (strArray[i].substr(0, 1) == "-") {
                if (isElementOf(strArray[i].substr(1),
                                 {"active"})) {
                    //or it is an option
                    ftpPassive = false;
                } else {
                    console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                    return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
                }
            } else if (strArray[i].substr(0, 1) != "-"){
                //it can only be an url
                if (url == "ftp.example.com") {
                    url = strArray[i];
                } else {
                    console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                    return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
                }
            } else {
                console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
            }
        }
        console("Sit back and relax when we are attempt to connect to:");
        console(url);
        //call this func
        object.connect(url, username, password, port, ftpPassive);
        return 0;
    }
    if (isElementOf(strArray[0], {"dc", "disconnect"})) {
        //Accept no argument
        if (strArray.size() > 1) {
            console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
            return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
        }
        //call this func here
        object.disconnect();
        return 0;
    }
    if (isElementOf(strArray[0], {"cd"})) {
        //Accept {1,1} string argument
        if (strArray.size() != 2) {
            console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
            return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
        }
        //default argument init
        string target_dir;
        if (strArray[1].substr(0,1) == "-") {
            console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
            return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
        }
        //it can only be target dir
        target_dir = strArray[1];
        //call this func here
        object.cd(target_dir);
        return 0;
    }
    if (isElementOf(strArray[0], {"ls", "dir", "ll"})) {
        //Accept no argument
        if (strArray.size() > 1) {
            console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
            return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
        }

        //call this func here
        object.ls();
        return 0;
    }
    if (isElementOf(strArray[0], {"download", "get", "pull"})) {
        //default argument init
        string fileRemote;
        string fileLocal;
        bool overwrite = false;
        for (int i = 1; i < strArray.size(); i++) {
            if (strArray[i].substr(0, 1) == "-" && i + 1 < strArray.size() && strArray[i + 1].substr(0, 1) != "-") {
                //if it is an argument
                if (isElementOf(strArray[i].substr(1),
                                {"local", "l"})) {
                    fileLocal = strArray[i + 1];
                } else if (isElementOf(strArray[i].substr(1),
                                       {"remote", "r"})) {
                    fileRemote = strArray[i + 1];
                } else {
                    console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                    return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
                }
                i++;
            } else if (strArray[i].substr(0, 1) == "-") {
                if (isElementOf(strArray[i].substr(1),
                                {"overwrite", "o"})) {
                    //or it is an option
                    overwrite = true;
                } else {
                    console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                    return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
                }
            } else if (strArray[i].substr(0, 1) != "-") {
                //it can only be file remote and file local
                if (fileRemote == "") {
                    //if fileRemote is not defined
                    fileRemote = strArray[i];
                } else if (fileLocal == "") {
                    //else if fileLocal is not defined
                    fileLocal = strArray[i];
                } else {
                    //wtf is this
                    console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                    return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
                }
            } else {
                console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
            }
        }
        //call this func here
        object.ftpGet(fileRemote, fileLocal, overwrite);
        return 0;
    }
    if (isElementOf(strArray[0], {"upload", "put", "push"})) {
        //default argument init
        string fileLocal;
        string fileRemote;
        for (int i = 1; i < strArray.size(); i++) {
            if (strArray[i].substr(0, 1) == "-" && i + 1 < strArray.size() && strArray[i + 1].substr(0, 1) != "-") {
                //if it is an argument
                if (isElementOf(strArray[i].substr(1),
                                {"local", "l"})) {
                    fileLocal = strArray[i + 1];
                } else if (isElementOf(strArray[i].substr(1),
                                       {"remote", "r"})) {
                    fileRemote = strArray[i + 1];
                } else {
                    console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                    return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
                }
                i++;
            } else if (strArray[i].substr(0, 1) != "-") {
                //it can only be file local and file remote
                if (fileLocal == "") {
                    //if fileLocal is not defined
                    fileLocal = strArray[i];
                } else if (fileRemote == "") {
                    //else if fileRemote is not defined
                    fileRemote = strArray[i];
                } else {
                    //wtf is this
                    console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                    return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
                }
            } else {
                console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
            }
        }
        //call this func here
        object.ftpPut(fileLocal, fileRemote);
        return 0;
    }
    if (isElementOf(strArray[0], {"mkdir"})) {
        //Accept {1,1} string argument
        if (strArray.size() != 2) {
            console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
            return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
        }
        //default argument init
        string name;
        if (strArray[1].substr(0,1) == "-") {
            console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
            return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
        }
        //it can only be dir name
        name = strArray[1];
        //call this func here
        object.mkdir(name);
        return 0;
    }
    if (isElementOf(strArray[0], {"rm"})) {
        //Accept {1,1} string argument
        if (strArray.size() != 2) {
            console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
            return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
        }
        //default argument init
        string name;
        if (strArray[1].substr(0,1) == "-") {
            console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
            return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
        }
        //it can only be filename
        name = strArray[1];
        //call this func here
        object.rm(name);
        return 0;
    }
    if (isElementOf(strArray[0], {"rmdir"})) {
        //Accept {1,1} string argument
        if (strArray.size() != 2) {
            console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
            return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
        }
        //default argument init
        string name;
        if (strArray[1].substr(0,1) == "-") {
            console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
            return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
        }
        //it can only be dir name
        name = strArray[1];
        //call this func here
        object.rmdir(name);
        return 0;
    }
    if (isElementOf(strArray[0], {"rename", "mv"})) {
        //default argument init
        string oldName;
        string newName;
        for (int i = 1; i < strArray.size(); i++) {
            if (strArray[i].substr(0, 1) == "-" && i + 1 < strArray.size() && strArray[i + 1].substr(0, 1) != "-") {
                //if it is an argument
                if (isElementOf(strArray[i].substr(1),
                                {"old", "o"})) {
                    oldName = strArray[i + 1];
                } else if (isElementOf(strArray[i].substr(1),
                                       {"new", "n"})) {
                    newName = strArray[i + 1];
                } else {
                    console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                    return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
                }
                i++;
            } else if (strArray[i].substr(0, 1) != "-") {
                //it can only be old name and new name
                if (oldName == "") {
                    //if old name is not defined
                    oldName = strArray[i];
                } else if (newName == "") {
                    //else if new name is not defined
                    newName = strArray[i];
                } else {
                    //wtf is this
                    console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                    return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
                }
            } else {
                console(_OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT);
                return _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT;
            }
        }
        //call this func here
        object.rename(oldName, newName);
        return 0;
    }
    if(isElementOf(strArray[0], {"man", "?", "help", "？"})){
        //default argument init
        string helpStr;
        for (int i = 1; i < strArray.size(); i++){
            helpStr = strArray[i];
        }
        //call this func here
        help(helpStr);
        return 0;
    }
    console(_OOP_FTPCLIENT_FTPOPT_BAD_COMMAND);
    return _OOP_FTPCLIENT_FTPOPT_BAD_COMMAND;
}

int ftpOptTerminal::help(string command){
    if(isElementOf(command, {"connect", "link"})){
        console(
            "Usage:\n"
            "    connect|link [-port|:|v <port> -username|u <user> -password|p <pass>]\n"
            "Start a session with a remote ftp server.\n"
        );
    }else if(isElementOf(command, {"dc", "disconnect"})){
        console(
            "Usage:\n"
            "    dc|disconnect\n"
            "Ends session with the current remote computer.\n"
        );
    }else if(isElementOf(command, {"cd"})){
        console(
            "Usage:\n"
            "    cd <dir>\n"
            "Sets the current working location to a specified location.\n"
        );
    }else if(isElementOf(command, {"ls", "dir", "ll"})){
        console(
            "Usage:\n"
            "    ls|dir\n"
            "Gets the items and child items in the current locations.\n"
        );
    }else if(isElementOf(command, {"download", "get", "pull"})){
        console(
            "Usage:\n"
            "    get|pull [-r|remote] <remote_file> [-l|local] <local_file> -[o|overwrite]\n"
            "Gets a specified file from current server.\n"
        );
    }else if(isElementOf(command, {"upload", "put", "push"})){
        console(
            "Usage:\n"
            "    put|push [-l|local] <local_file> [-r|remote] <remote_file>\n"
            "Push a specified file to current server.\n"
        );
    }else if(isElementOf(command, {"mkdir"})){
        console(
            "Usage:\n"
            "    mkdir <dirName>\n"
            "Creates a new directory on the remote server.\n"
        );
    }else if(isElementOf(command, {"rmdir"})){
        console(
            "Usage:\n"
            "    rmdir <dirName>\n"
            "Deletes the specified directory on the remote server.\n"
        );
    }else if(isElementOf(command, {"rm"})){
        console(
            "Usage:\n"
            "    rm <fileName>\n"
            "Deletes the specified file on the remote server.\n"
        );
    }else if(isElementOf(command, {"rename", "mv"})){
        console(
            "Usage:\n"
            "    rename|mv [-o|old] <old_name> [-n|new] <new_name>\n"
            "Rename a file on the remote server.\n"
        );
    }else if(isElementOf(command, {"man"})){
        console(
            "All vaild commands:\n"
            "man\n"
            "exit\n"
            "connect|link\n"
            "dc|disconnect\n"
            "cd\n"
            "ls|ll|dir\n"
            "download|get|pull\n"
            "upload|put|push\n"
            "mkdir\n"
            "rmdir\n"
            "rm\n"
            "rename|mv\n"
        );
    }else if(command == ""){
        console(
            "Which manual page do you want?\n"
            "Type \"man man\" for all available commands\n"
        );
    }else{
        console("Sorry, we have no manual for " + command + ".");
    }
}

int menu::print(){
    int maxLength = 0;
    for(int i=0;i<menuSto.size();i++){
        if(menuSto[i][0].length() > maxLength){
            maxLength = menuSto[i][0].length();
        }
    }
    for(int i=0;i<menuSto.size();i++){
        cout << i+1 << setfill(' ') << setw(maxLength+4-count(i+1))
             << menuSto[i][0] << endl;
    }
    return 0;
}

int menu::create(string arg1){
    menuSto.push_back({arg1,"true"});
    return _OOP_FTPCLIENT_UNDEFINED_ERROR;
}

int count(int arg1){
    if (arg1 == 0) {
        return 1;
    }
    return (int)log10((double)arg1)+1;
}

int console(int arg1) {
    DWORD err = GetLastError();
    //https://support.microsoft.com/en-us/help/193625/info-wininet-error-codes-12001-through-12156
    //https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx
    //00006 0x00000006 ERROR_INVALID_HANDLE
    //12002 0x00002ee2 ERROR_INTERNET_TIMEOUT
    //12007 0x00002ee7 ERROR_INTERNET_NAME_NOT_RESOLVED
    //12014 0x00002eee ERROR_INTERNET_INCORRECT_PASSWORD
    //12015 0x00002eef ERROR_INTERNET_LOGIN_FAILURE
    //12029 0x00002efd ERROR_INTERNET_CANNOT_CONNECT
    //12031 0x00002eff ERROR_INTERNET_CONNECTION_RESET
    //12110 0x00002f4e ERROR_FTP_TRANSFER_IN_PROGRESS
    if (arg1 == _OOP_FTPCLIENT_WININET_ERROR) {
        cout << "Error " << "0x" << hex
             << setw(8) << setfill('0') << err << " " << parse_wininet_errno((int) err) << endl;
        if (err == 6) {
            console("You were not connected.");
            console("You must connect to a remote host to continue.");
        } else if (err == 12002) {
            console("Connection timed out.");
            console("You may try it again later.");
        } else if (err == 12007) {
            console("The given remote host was not resolved.");
        } else if (err == 12014) {
            console("Incorrect password.");
            console("Failed to authorize your session.");
        } else if (err == 12015) {
            console("Failed to authorize your session.");
        } else if (err == 12029) {
            console("Your request has been rejected by remote host.");
            console("Bad argument OR network communicate issue.");
        } else if (err == 12031) {
            console("Bad argument OR network communicate issue.");
        } else if (err == 12110) {
            console("Please wait until transfer terminated");
        } else {
            console("Unknown error. You may contact the author of this application");
        }
    } else if (arg1 == _OOP_FTPCLIENT_UNDEFINED_ERROR) {
        cout << "Error " << "0x" << hex
             << setw(8) << setfill('0') << arg1 << endl;
    } else if (arg1 == _OOP_FTPCLIENT_FTPOPT_ERROR) {
        cout << "Error " << "0x" << hex
             << setw(8) << setfill('0') << arg1 << endl;
    } else if (arg1 == _OOP_FTPCLIENT_FTPOPT_BAD_ARGUMENT) {
        cout << "Error " << "0x" << hex
             << setw(8) << setfill('0') << arg1 << endl;
        console("Bad Argument.");
        console("Type 'help', 'man' or '?' for user manual.");
    } else if (arg1 == _OOP_FTPCLIENT_FTPOPT_BAD_COMMAND) {
        cout << "Error " << "0x" << hex
             << setw(8) << setfill('0') << arg1 << endl;
        console("Bad Command.");
        console("Type 'help', 'man' or '?' for user manual.");
    } else {
        cout << arg1 << endl;
    }
}

int console(string arg1, bool endLine){
    if(endLine){
        cout << arg1 << endl;
    }else{
        cout << arg1;
    }
    return 0;
}

string parse_wininet_errno(int arg1) {
    for(int i=0;i<59;i++){
        if(wininet_errno[i] == arg1){
            return wininet_errstr[i];
        }
    }
    return "STOP_UNKNOWN";
}

vector<string> split(const string& s, char seperator){
    vector<string> output;

    string::size_type prev_pos = 0, pos = 0;

    while((pos = s.find(seperator, pos)) != string::npos)
    {
        string substring( s.substr(prev_pos, pos-prev_pos) );

        output.push_back(substring);

        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word

    return output;
}

bool isElementOf(string arg1, vector<string> arg2){
    for(int i=0;i<arg2.size();i++){
        if(arg2[i] == arg1){
            return true;
        }
    }
    return false;
}

int toNumber(string str){
    return (int)strtol(str.c_str(), nullptr, 0);
}