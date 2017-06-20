//
// Created by Aozak on 2017/6/20.
//

#include "functions.h"

int objMenu::print(){
    int maxLength = 0;
    for(int i=0;i<menuSto.size();i++){
        if(menuSto[i][0].length() > maxLength){
            maxLength = menuSto[i][0].length();
        }
    }
    for(int i=0;i<menuSto.size();i++){
        cout << i << setfill('.') << setw(maxLength+5-count(i))
             << menuSto[i][0] << endl;
    }
}
int objMenu::create(string arg1){
//    vector<string> temp;
//    temp.push_back(arg1);
//    temp.push_back("true");
    menuSto.push_back({arg1,"true"});
}

int count(int arg1){
    return (int)log10((double)arg1)+1;
}