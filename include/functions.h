//
// Created by Aozak on Jun 20 17
//

//    Aozak @ Jun 20 17 14.58
//The following contents was automatic created by clion. I don't know why I need it so I change it into comment.


#ifndef OOP_ENDTERM_FTPCLIENT_FUNCTIONS_H
#define OOP_ENDTERM_FTPCLIENT_FUNCTIONS_H
#include "iostream"
#include "iomanip"
#include "vector"
#include "string"
#include "cmath"
#include "wininet.h"
#pragma comment(lib, "Wininet")
#endif

using namespace std;

//on error CONSTs:
const int _OOP_FTPCLIENT_UNDEFINED_ERROR = 1577267;
const int _OOP_FTPCLIENT_WININET_ERROR = 25443;
const int _OOP_FTPCLIENT_FTPOPT_ERROR = 416541;

class ftpOpt{
private:
    HINTERNET hInternet;
    HINTERNET hFtpSession;
    bool ftpOpt_ftpPassive;
    string ftpOpt_currHost;
    string ftpOpt_currUsr;
    string ftpOpt_filename;
    string ftpOpt_currDir;
public:
    int connect(string url, int port, string username, string password, bool ftpPassive = false);
    int updateCurrDir();
    int ls();
    int cd(string dir);
    int download();
    int upload();
    int mkdir();
    int rm();
    int rename();
    int disconnect();
    string currDir();
    string currHost();
    string currUsr();
};

class menu{
private:
    vector<vector<string>> menuSto;
public:
    int print();                //print all objMenu item to screen
    int create(string arg1);    //create a objMenu item
};

int count(int arg1);            //count digital of an integer

int console(int arg1);
int console(string arg1);

string parse_wininet_errno(int arg1);