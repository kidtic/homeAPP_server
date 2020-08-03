
#ifndef HOMEAPPSYS_H
#define HOMEAPPSYS_H

#include "homeAppCore.h"
#include "homeAppComm.h"


using namespace std;

class homeAppSys
{
public:  //priv
    /* data */
    homeAppCore* mCore;
    homeAppComm* mComm;
    vector<AppUser> users;
    Json::Value mConfig;
    
public:
    homeAppSys(string configPath);
    ~homeAppSys();

    //主线程
    void run();
};


#endif