//
// Created by Aozak on 2017/6/20.
//

#include "functions.h"

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
    return 0;
}

int count(int arg1){
    if(arg1 == 0){
        return 1;
    }else{
        return (int)log10((double)arg1)+1;
    }
}