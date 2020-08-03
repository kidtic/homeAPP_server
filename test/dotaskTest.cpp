#include "homeAppSys.h"
using namespace std;

int main(int argc, char const *argv[])
{
    homeAppSys homesys("config/homeAppConfig.json");

    Json::Value root;

    root["head"]="request";
    root["part"]="pay";
    root["func"]="return";
    root["user"]="root";
    /*
    Json::Value data;
    data["month"]="08";
    data["year"]="2020";
    root["data"]=data;
    */


    Json::Value res = homesys.mCore->doTask(root);
    cout<<res.toStyledString()<<endl;




}