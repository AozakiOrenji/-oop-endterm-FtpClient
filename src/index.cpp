/*
 * This file was created by Aozak using CLion 2017.2 EAP as my end-term OOP project at Jun 20 2017.
 */

#include "functions.h"

int main(){
    ftpOpt          ftp;
    ftpOptTerminal  ter;
    ter.init(ftp);

    while(ter.parse(ter.wait()) != _OOP_FTPCLIENT_TERMINAL_EXIT);

    return 0;
}

/*
console("ftp>connect ct.ftp.zeus.smartgslb.com -port 21 -usr ftp@touko.host.smartgslb.com -pw jstaccit");
ftp.connect("ct.ftp.zeus.smartgslb.com", 21, "ftp@touko.host.smartgslb.com", "jstaccit", true);

console(ftp.currDir() + ">ls");
ftp.ls();
*/