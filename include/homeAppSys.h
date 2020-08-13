
#ifndef HOMEAPPSYS_H
#define HOMEAPPSYS_H

#include "homeAppCore.h"
#include "homeAppComm.h"
#include <thread>


using namespace std;

class homeAppSys
{
public:  //priv
    /* data */
    mutex mCoreMutex;
    homeAppCore* mCore;
    homeAppComm* mComm;
    vector<AppUser> users;
    Json::Value mConfig;

    //万元计划利率（万份收益）如果超过target按照正利率，如果低于target按照负利率
    float mInterestRate=0.71;
    
public:
    homeAppSys(string configPath);
    ~homeAppSys();

    //主线程
    void run();
    //定时线程
    void timerun();
};


#endif