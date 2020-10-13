
#ifndef _APPUSER_H
#define _APPUSER_H

#include "stdio.h"
#include <string>


using namespace std;

class AppUser
{
public:
    /* data */
    string username;
    string passwd;
    queue<Json::Value> tasks;
public:
    AppUser(string username,string passwd){
        this->username=username;
        this->passwd=passwd;
    };
    AppUser(){};
};



#endif
