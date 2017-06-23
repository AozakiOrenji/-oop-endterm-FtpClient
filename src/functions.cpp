//
// Created by Aozak on 2017/6/20.
//

#include "functions.h"
#include "wininet_errno.h"

int ftpOpt::connect(string url, int port, string username, string password, bool ftpPassive){
    hInternet = InternetOpen(nullptr, INTERNET_OPEN_TYPE_DIRECT,
                             nullptr, nullptr, 0);
    if(hInternet == nullptr){
        console(_OOP_FTPCLIENT_WININET_ERROR);
    }else{
        hFtpSession = InternetConnect(hInternet, url.c_str(), (INTERNET_PORT)port,
                                      username.c_str(), password.c_str(),
                                      INTERNET_SERVICE_FTP, ftpPassive?INTERNET_FLAG_PASSIVE:0, 0);
        if(hFtpSession == nullptr){
            console(_OOP_FTPCLIENT_WININET_ERROR);
        }else{
            ftpOpt_currHost = url;
            ftpOpt_currUsr = username;
            ftpOpt_ftpPassive = ftpPassive;
            updateCurrDir();
            return 0;
        }
    }
    return _OOP_FTPCLIENT_FTPOPT_ERROR;
}

int ftpOpt::updateCurrDir(){
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
    HINTERNET hFind;
    WIN32_FIND_DATA fd;
    char currDir[_OOP_FTPCLIENT_PATH_SIZE];
    strcpy(currDir, ftpOpt_currDir.c_str());
    hFind = FtpFindFirstFile(hFtpSession, currDir, &fd, 0, 0);

    if(hFind != nullptr){
        do{
            console(fd.cFileName);
        }while(InternetFindNextFile(hFind, &fd));
    }else{
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    return 0;
}

int ftpOpt::cd(string dir){
    char charDir[_OOP_FTPCLIENT_PATH_SIZE];
    strcpy(charDir, dir.c_str());
    if(!FtpSetCurrentDirectory(hFtpSession, charDir)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    };
    updateCurrDir();
    return 0;
}

int ftpOpt::ftpGet(string pRemote, string pLocal, bool overwrite){
    char pathRemote[_OOP_FTPCLIENT_PATH_SIZE];
    char pathLocal[_OOP_FTPCLIENT_PATH_SIZE];
    pathRemote = pRemote.c_str();
    pathLocal = pLocal.c_str();
    if(!FtpGetFile(
            hFtpSession,
            pathRemote,
            pathLocal,
            !overwrite,
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
    char pathRemote[_OOP_FTPCLIENT_PATH_SIZE];
    char pathLocal[_OOP_FTPCLIENT_PATH_SIZE];
    pathRemote = pRemote.c_str();
    pathLocal = pLocal.c_str();
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
    char dirName[_OOP_FTPCLIENT_PATH_SIZE];
    dirName = dir.c_str();
    if(!FtpCreateDirectory(hFtpSession, dirName)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    return 0;
}

int ftpOpt::rm(string file){
    char fileName[_OOP_FTPCLIENT_PATH_SIZE];
    fileName = file.c_str();
    if(!FtpDeleteFile(hFtpSession, fileName)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    return 0;
}

int ftpOpt::rmdir(string dir){
    char dirName[_OOP_FTPCLIENT_PATH_SIZE];
    dirName = dir.c_str();
    if(!FtpRemoveDirectory(hFtpSession, dirName)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    return 0;
}

int ftpOpt::rename(string oldFile, string newFile){
    char oldFileName[_OOP_FTPCLIENT_PATH_SIZE];
    char newFileName[_OOP_FTPCLIENT_PATH_SIZE];
    oldFileName = oldFile.c_str();
    newFileName = newFile.c_str();
    if(!FtpRenameFile(hFtpSession, oldFileName, newFileName)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    return 0;
}

int ftpOpt::disconnect(){
    if(!(bool)InternetCloseHandle(hFtpSession)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }
    if(!(bool)InternetCloseHandle(hInternet)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    };
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
    if(arg1 == 0){
        return 1;
    }
    return (int)log10((double)arg1)+1;
}

int console(int arg1){
    DWORD err = GetLastError();
    //https://support.microsoft.com/en-us/help/193625/info-wininet-error-codes-12001-through-12156
    //https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx
    //00006 0x00000006 ERROR_INVALID_HANDLE
    //12007 0x00002ee7 ERROR_INTERNET_NAME_NOT_RESOLVED
    //12014 0x00002eee ERROR_INTERNET_INCORRECT_PASSWORD
    //12029 0x00002efd ERROR_INTERNET_CANNOT_CONNECT
    //12110 0x00002f4e ERROR_FTP_TRANSFER_IN_PROGRESS
    if(arg1 == _OOP_FTPCLIENT_WININET_ERROR){
        cout << "Error " << "0x" << hex
             << setw(8) << setfill('0') << err << " " << parse_wininet_errno((int)err) << endl;
        if(err == 6){
            console("You were not connected.");
            console("You must connect to a remote host to continue.");
        }else if(err == 12007){
            console("The given remote host was not resolved.");
        }else if(err == 12014){
            console("Failed to authorize your session.");
        }else if(err == 12029) {
            console("Your request was rejected by remote host.");
            console("Bad argument OR network communicate issue.");
        }else if(err == 12110){
            console("Please wait until transfer terminated");
        }else{
            console("Unknown error. You may contact the author of this application");
        }
    }else if(arg1 == _OOP_FTPCLIENT_UNDEFINED_ERROR){
        cout << "Undefined error: " << "0x" << hex
             << setw(8) << setfill('0') << arg1 << endl;
    }else if(arg1 == _OOP_FTPCLIENT_FTPOPT_ERROR){
        cout << "FtpOpt error: " << "0x" << hex
             << setw(8) << setfill('0') << arg1 << endl;
    }else{
        cout << arg1 << endl;
    }
}

int console(string arg1){
    cout << arg1 << endl;
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