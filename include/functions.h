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
public:
    string filename;
    string currDir;
    int connect(string url, int port, string username, string password);
    int requestCurrDir();
    int ls();
    int cd();
    int download();
    int upload();
    int mkdir();
    int rm();
    int rename();
    int disconnect();
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