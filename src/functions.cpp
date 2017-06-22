//
// Created by Aozak on 2017/6/20.
//

#include "functions.h"

int ftpOpt::connect(string url, int port, string username, string password) {
    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT,
                             NULL, NULL, 0);
    if(hInternet == NULL){
        console(_OOP_FTPCLIENT_WININET_ERROR);
    }else{
        hFtpSession = InternetConnect(hInternet, url.c_str(), port,
                                      username.c_str(), password.c_str(),
                                      INTERNET_SERVICE_FTP, 0, 0);
        if(hFtpSession == NULL){
            console(_OOP_FTPCLIENT_WININET_ERROR);
        }else{
            return 0;
        }
    }
    return _OOP_FTPCLIENT_FTPOPT_ERROR;
}

int ftpOpt::requestCurrDir(){
    char tmp[256];
    DWORD currDirSize = 256;
    if(!FtpGetCurrentDirectory(hFtpSession, tmp, &currDirSize)){
        console(_OOP_FTPCLIENT_WININET_ERROR);
        currDir = "undefined";
        return _OOP_FTPCLIENT_FTPOPT_ERROR;
    }else{
        currDir = tmp;
        return 0;
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
    if(arg1 == 0){
        return 1;
    }else{
        return (int)log10((double)arg1)+1;
    }
}

int console(int arg1){
    //00006 0x00000006 not connected
    //12007 0x00002ee7 net error
    //12014 0x00002eee unauthorized
    //12029 0x00002efd rejected
    if(arg1 == _OOP_FTPCLIENT_WININET_ERROR){
        cout << "Fatal error: " << "0x" << hex
             << setw(8) << setfill('0') << GetLastError() << endl;
        if(GetLastError() == 6){
            console("You were not connected.");
            console("You must connect to a remote host to continue.");
        }else if(GetLastError() == 12007){
            console("The given remote host was not resolved.");
        }else if(GetLastError() == 12014){
            console("Failed to authorize your session.");
        }else if(GetLastError() == 12029){
            console("Your request was rejected by remote host. Check your argument.");
        }else{
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
    return 0;
}

int console(string arg1){
    cout << arg1 << endl;
    return 0;
}