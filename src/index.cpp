/*
 * This file was created by Aozak using CLion 2017.2 EAP as my end-term OOP project at Jun 20 2017.
 * 1032 lines on total.
 */

#include "functions.h"

int main(){
    ftpOpt          ftp;
    ftpOptTerminal  ter;
    ter.init(ftp);

    //it's magic!
    while(ter.parse(ter.wait()) != _OOP_FTPCLIENT_TERMINAL_EXIT);

    return 0;
}