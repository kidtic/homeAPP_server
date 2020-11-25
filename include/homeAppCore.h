/*************************************************************************
    > File Name: homwAppCore.h
    > Author:zouzhiqiang-kidtic
    > Mail:592113107@qq.com 
    > github:kidtic
    > 说明：从json解析从网络监听到的消息，并根据消息进行数据库处理，并回复json
 ************************************************************************/
#ifndef HOMEAPPCORE_H
#define HOMEAPPCORE_H


#include <queue>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <time.h>
#include "dbsql.h"
#include "AppUser.h"


using namespace std;

class homeAppCore
{
private:   //priv
    /* data */
    
    vector<AppUser> users;
    //

    //database 
    string mysqlurl,mysqluser,mysqlpasswd,mysqldb;
    //系统版本号
    string version;
    //文件库
    string filelibPath;
public:
    homeAppCore(vector<AppUser>users,string mysqlurl,
                string mysqluser,string mysqlpasswd,
                string mysqldb,string version,string filelibPath);
    ~homeAppCore();
    
    
    


    //--------------执行json任务
    Json::Value doTask(Json::Value task);
    Json::Value doBasicTask(Json::Value task);
    Json::Value doBasicLoginTask(Json::Value task);
    Json::Value doBasicRetrunTask(Json::Value task);//重要函数，客户端周期查询

    Json::Value doPayTask(Json::Value task);
    Json::Value doPayChangeTask(Json::Value task);
    Json::Value doPayReturnTask(Json::Value task);
    Json::Value doPayReturnLastTask(Json::Value task);
    Json::Value doPayReturnMonthbillTask(Json::Value task);

    Json::Value doSaveTask(Json::Value task);
    Json::Value doSaveChangeMoneyTask(Json::Value task);
    Json::Value doSaveChangeTargetTask(Json::Value task);
    Json::Value doSaveReturnTask(Json::Value task);
    Json::Value doSaveReturnLastTask(Json::Value task);

    Json::Value doPostTask(Json::Value task);
    Json::Value doPostUploadTask(Json::Value task);

    //万元计划执行
    bool save_changeTarget(float change,string ps,bool star=false);
    bool save_changeMoney(float change,string ps,bool star=false);
    vector<string> save_returnlast();


    //pay功能
    bool pay_change(float change,string ps,bool star=false);




};








//-------------------------------时间
class DateTime
{
public:
    /* data */
    int16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t sec;
    

    string timestr;
public:
    DateTime(/* args */);
    DateTime(string time);
    ~DateTime();

    string toString();
    void setNow();

    bool operator >(const DateTime& a);
    bool operator <(const DateTime& a);
    bool operator >=(const DateTime& a);
    bool operator <=(const DateTime& a);
    bool operator ==(const DateTime& a);
};

#endif