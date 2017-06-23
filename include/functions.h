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

const int _OOP_FTPCLIENT_PATH_SIZE = 4096;
const int _OOP_FTPCLIENT_TERMINAL_SIZE = 1024;

const int _OOP_FTPCLIENT_TERMINAL_EXIT = 5461654;

//on error CONSTs:
const int _OOP_FTPCLIENT_UNDEFINED_ERROR = 1577267;
const int _OOP_FTPCLIENT_WININET_ERROR = 25443;
const int _OOP_FTPCLIENT_FTPOPT_ERROR = 416541;
const int _OOP_FTPCLIENT_FTPOPT_FILEEXIST = 8765456;

class ftpOpt{
private:
    HINTERNET hInternet;
    HINTERNET hFtpSession;
    bool ftpOpt_ftpPassive;
    bool ftpOpt_connected;
    string ftpOpt_currHost;
    string ftpOpt_currUsr;
    string ftpOpt_filename;
    string ftpOpt_currDir;
    int ftpOpt_transferMode = FTP_TRANSFER_TYPE_UNKNOWN;
    /*
     * transferMode can be:
     * FTP_TRANSFER_TYPE_ASCII
     * FTP_TRANSFER_TYPE_BINARY
     * FTP_TRANSFER_TYPE_UNKNOWN = FTP_TRANSFER_TYPE_BINARY
     */
public:
    ftpOpt();
    int connect(string url, int port, string username, string password, bool ftpPassive = true);
    int updateCurrDir();
    int ls();
    int cd(string dir);
    int ftpGet(string pRemote, string pLocal, bool overwrite = false);//
    int ftpPut(string pLocal, string pRemote);                        //
    int mkdir(string dir);                                            //
    int rm(string file);                                              //
    int rmdir(string dir);                                            //
    int rename(string oldFile, string newFile);                       //
    int disconnect();                                                 //
    string currDir();
    string currHost();
    string currUsr();
    bool isConnected();                                               //
};

class ftpOptTerminal{
private:
    string command;
    ftpOpt object;
public:
    int init(ftpOpt obj);
    //print dir, return command
    string wait();
    //parse usr command
    int parse(string cmdStr);

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
int console(string arg1, bool endLine = true);

string parse_wininet_errno(int arg1);

vector<string> split(const string& s, char seperator);