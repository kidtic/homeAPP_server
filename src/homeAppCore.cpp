#include "homeAppCore.h"

using namespace std;

homeAppCore::homeAppCore(vector<AppUser> users,string mysqlurl,string mysqluser,string mysqlpasswd,string mysqldb)
{
    this->users=users;
    //mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb);
    this->mysqlurl=mysqlurl;
    this->mysqluser=mysqluser;
    this->mysqlpasswd=mysqlpasswd;
    this->mysqldb=mysqldb;
}

homeAppCore::~homeAppCore()
{
}

Json::Value homeAppCore::doTask(Json::Value task){
    Json::Value res;
    if(task["head"].asString()=="request"){
        if(task["part"].asString()=="basic"){
            res=doBasicTask(task);
        }
        else if(task["part"].asString()=="pay"){
            res=doPayTask(task);
        }
    }


    return res;

}

Json::Value homeAppCore::doBasicTask(Json::Value task){
    Json::Value res;
    if(task["func"].asString()=="login"){
        res=doBasicLoginTask(task);
    }
    else if(task["func"].asString()=="return"){

    }

    return res;

}
Json::Value homeAppCore::doBasicLoginTask(Json::Value task){
    cout<<"request:BasicLogin"<<endl;

    Json::Value res;
    string us=task["user"].asString();
    //find
    for(auto e:users){
        cout<<e.username<<" "<<us<<endl;
        if(e.username==us){
            res["head"]="result";
            res["part"]="basic";
            res["func"]="login";
            res["user"]=us;
            res["data"]="ok";
            return res;
        }
    }
    res["head"]="result";res["part"]="basic";
    res["func"]="login";res["user"]=us;
    res["data"]="no user";
    return res;
}
Json::Value homeAppCore::doBasicRetrunTask(Json::Value task){

}


Json::Value homeAppCore::doPayTask(Json::Value task){
    Json::Value res;
    if(task["func"].asString()=="change"){
        res=doPayChangeTask(task);
    }
    else if(task["func"].asString()=="return"){
        res=doPayReturnTask(task);
    }

    return res;
}
Json::Value homeAppCore::doPayChangeTask(Json::Value task){
    float changemoney=task["data"]["changeNumber"].asFloat();
    string psstr=task["data"]["ps"].asString();
    bool star=task["data"]["star"].asBool();
    Json::Value res;
    res["head"]="result";res["part"]="pay";
    res["func"]="change";res["user"]=task["user"].asString();
    Json::Value datajson;
    if(pay_change(changemoney,psstr,star)==true){
        datajson["result"]="ok";
    }
    else
    {
        datajson["result"]="error";
    }
    res["data"]=datajson;
    return res;
}
Json::Value homeAppCore::doPayReturnTask(Json::Value task){
    cout<<"doPayReturnTask"<<endl;
    DBsql mydb;
    cout<<"ok1"<<endl;
    mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb);
    cout<<"ok2"<<endl;
    vector<vector<string>> data= mydb.getAllData("pay");
    Json::Value res;
    res["head"]="result";res["part"]="pay";
    res["func"]="return";res["user"]=task["user"].asString();
    for(int i=0;i<data.size();i++){
        Json::Value inin;
        inin["time"]=data[i][0];
        inin["moneychange"]=data[i][1];
        inin["money"]=data[i][2];
        inin["ps"]=data[i][3];
        inin["star"]=data[i][4];
        res["data"].append(inin);
    }

    return res;

}   



bool homeAppCore::pay_change(float change,string ps,bool star){
    DBsql mydb;
    mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb);

    //查看pay表有多少条记录
    int len=mydb.getDataLen("pay");
    
    //返回最后一条记录
    vector<vector<string>> lastdata=mydb.getData("pay",len-1,1);
    if(lastdata.size()!=1){
        perror("mydb.getData(pay,len-1,1); 返回不唯一");
        return false;
    }
    
    //最后一条记录的当前余额
    float money=atof(lastdata[0][2].c_str());
    cout<<"lastmoney: "<<money<<endl;
    float curmoney=money+change;
    cout<<"curmoney: "<<curmoney<<endl;
    vector<string> inin;
    DateTime t=DateTime();
    inin.push_back(t.toString());
    inin.push_back(to_string(change));
    inin.push_back(to_string(curmoney));
    inin.push_back(ps);
    inin.push_back(star?"true":"false");
    bool resb= mydb.addData("pay",inin); 
    return resb;

}










//-------------------time
DateTime::DateTime(/* args */)
{
    time_t tt;
    time( &tt );
    tt = tt + 8*3600;  // transform the time zone
    tm* t= gmtime( &tt );

    year=t->tm_year+1900;
    month=t->tm_mon + 1;
    day=t->tm_mday;
    hour=t->tm_hour;
    minute=t->tm_min;
    sec=t->tm_sec;

    char* p=new char[30];
    sprintf(p,"%d-%02d-%02d %02d:%02d:%02d\n",
           year,month,day,hour,minute,sec);
    timestr=string(p);
    delete p;
}
DateTime::DateTime(string time){
    if(time.size()!=19){
        perror("DateTime Type is error");
    }
    string yyyy=time.substr(0,4);
    string MM=time.substr(5,2);
    string dd=time.substr(8,2);
    string hh=time.substr(11,2);
    string mm=time.substr(14,2);
    string ss=time.substr(17,2);
    year=atoi(yyyy.c_str());
    month=atoi(MM.c_str());
    day=atoi(dd.c_str());
    hour=atoi(hh.c_str());
    minute=atoi(mm.c_str());
    sec=atoi(ss.c_str());

    char* p=new char[30];
    sprintf(p,"%d-%02d-%02d %02d:%02d:%02d\n",
           year,month,day,hour,minute,sec);
    timestr=string(p);
    delete p;

}

DateTime::~DateTime()
{
}

string DateTime::toString(){
    return timestr;
}
void DateTime::setNow(){
    time_t tt;
    time( &tt );
    tt = tt + 8*3600;  // transform the time zone
    tm* t= gmtime( &tt );

    year=t->tm_year+1900;
    month=t->tm_mon + 1;
    day=t->tm_mday;
    hour=t->tm_hour;
    minute=t->tm_min;
    sec=t->tm_sec;

    char* p=new char[30];
    sprintf(p,"%d-%02d-%02d %02d:%02d:%02d\n",
           year,month,day,hour,minute,sec);
    timestr=string(p);
    delete p;
}

bool DateTime::operator >(const DateTime& a){
    if(this->year==a.year){
        if(this->month==a.month){
            if(this->day==a.day){
                if(this->hour==a.hour){
                    if(this->minute==a.minute){
                        if(this->sec==a.sec){
                            return false;
                        }else return this->sec>a.sec;
                    }else return this->minute>a.minute;
                }else return this->hour>a.hour;
            }else return this->day>a.day;
        }else return this->month>a.month;
    }else return this->year>a.year;
}
bool DateTime::operator <(const DateTime& a){
    if(this->year==a.year){
        if(this->month==a.month){
            if(this->day==a.day){
                if(this->hour==a.hour){
                    if(this->minute==a.minute){
                        if(this->sec==a.sec){
                            return false;
                        }else return this->sec<a.sec;
                    }else return this->minute<a.minute;
                }else return this->hour<a.hour;
            }else return this->day<a.day;
        }else return this->month<a.month;
    }else return this->year<a.year;
}
bool DateTime::operator >=(const DateTime& a){
    if(this->year==a.year){
        if(this->month==a.month){
            if(this->day==a.day){
                if(this->hour==a.hour){
                    if(this->minute==a.minute){
                        if(this->sec==a.sec){
                            return true;
                        }else return this->sec>a.sec;
                    }else return this->minute>a.minute;
                }else return this->hour>a.hour;
            }else return this->day>a.day;
        }else return this->month>a.month;
    }else return this->year>a.year;
}
bool DateTime::operator <=(const DateTime& a){
    if(this->year==a.year){
        if(this->month==a.month){
            if(this->day==a.day){
                if(this->hour==a.hour){
                    if(this->minute==a.minute){
                        if(this->sec==a.sec){
                            return true;
                        }else return this->sec<a.sec;
                    }else return this->minute<a.minute;
                }else return this->hour<a.hour;
            }else return this->day<a.day;
        }else return this->month<a.month;
    }else return this->year<a.year;
}
bool DateTime::operator ==(const DateTime& a){
    return this->timestr==a.timestr;
}
