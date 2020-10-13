#include "homeAppCore.h"

using namespace std;

homeAppCore::homeAppCore(vector<AppUser> users,string mysqlurl,
                        string mysqluser,string mysqlpasswd,
                        string mysqldb,string version)
{
    this->users=users;
    //mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb);
    this->mysqlurl=mysqlurl;
    this->mysqluser=mysqluser;
    this->mysqlpasswd=mysqlpasswd;
    this->mysqldb=mysqldb;
    this->version=version;
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
        else if(task["part"].asString()=="save"){
            res=doSaveTask(task);
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
        res=doBasicRetrunTask(task);
    }

    return res;

}
Json::Value homeAppCore::doBasicLoginTask(Json::Value task){
    cout<<"request:BasicLogin"<<endl;
    Json::Value res;
    string us=task["user"].asString();
    res["head"]="result";
    res["part"]="basic";
    res["func"]="login";
    res["user"]=us;
    Json::Value data;
    data["version"]=version;
    res["data"]=data;
    //检测用户
    for(auto e:users){
        cout<<e.username<<" "<<us<<endl;
        if(e.username==us){  
            res["result"]="ok";
            return res;
        }
    }
    res["result"]="error:no user";
    
    return res;
}
Json::Value homeAppCore::doBasicRetrunTask(Json::Value task){

    //检测用户
    string us=task["user"].asString();
    AppUser m_user("error","0");
    for(auto &e:users){
        cout<<e.username<<" "<<us<<endl;
        if(e.username==us){  
            m_user=e;
        }
    }
    //返回格式
    Json::Value res;
    res["head"]="result";
    res["part"]="basic";
    res["func"]="return";
    res["user"]=us;
    //查看用户信息
    if(m_user.username=="error"){
        res["result"]="error no user";
        return res;
    }
    res["result"]="ok";
    //数据内容
    Json::Value data;
    //取指令
    while(!m_user.tasks.empty()) {
        data.append(m_user.tasks.front());
        m_user.tasks.pop();
    }
    res["data"]=data;


}


Json::Value homeAppCore::doPayTask(Json::Value task){
    Json::Value res;
    if(task["func"].asString()=="change"){
        res=doPayChangeTask(task);
    }
    else if(task["func"].asString()=="return"){
        res=doPayReturnTask(task);
    }
    else if (task["func"].asString()=="returnlast")
    {
        res=doPayReturnLastTask(task);
    }
    else if (task["func"].asString()=="returnmonthbill")
    {
        res=doPayReturnMonthbillTask(task);
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

    if(pay_change(changemoney,psstr,star)==true){
        cout<<"pay_change ok"<<endl;
        res["result"]="ok";
    }
    else
    {
        cout<<"pay_change error"<<endl;
        res["result"]="error";
    }
    


    return res;
}
Json::Value homeAppCore::doPayReturnTask(Json::Value task){
    cout<<"doPayReturnTask"<<endl;
    Json::Value res;
    res["head"]="result";res["part"]="pay";
    res["func"]="return";res["user"]=task["user"].asString(); 

    DBsql mydb;
    if(!mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb)){
        res["result"]="error: mysql initDB error";
        return res;
    }
    vector<vector<string>> data= mydb.getAllData("pay");
    if(data.size()==0){
        res["result"]="error: mysql getAllData return empty";
        return res;
    }

    for(int i=0;i<data.size();i++){
        Json::Value inin;
        inin["time"]=data[i][0];
        inin["moneychange"]=data[i][1];
        inin["money"]=data[i][2];
        inin["ps"]=data[i][3];
        inin["star"]=data[i][4];
        res["data"].append(inin);
    }
    res["result"]="ok";
    return res;

}   
Json::Value homeAppCore::doPayReturnLastTask(Json::Value task){
    cout<<"doPayReturnLastTask"<<endl;
    Json::Value res;
    res["head"]="result";res["part"]="pay";
    res["func"]="returnlast";res["user"]=task["user"].asString();

    DBsql mydb;
    mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb);
    int paytableLen=mydb.getDataLen("pay");

    vector<vector<string>> data= mydb.getData("pay",paytableLen-1,1);
    if(data.size()!=1){
        cout<<"Error:doPayReturnLastTask getData lenght != 1"<<endl;
        res["result"]="Error:doPayReturnLastTask getData lenght != 1";
        return res;
    }

    Json::Value inin;
    inin["time"]=data[0][0];
    inin["moneychange"]=data[0][1];
    inin["money"]=data[0][2];
    inin["ps"]=data[0][3];
    inin["star"]=data[0][4];
    res["data"]=inin;

    res["result"]="ok";
    return res;

}
Json::Value homeAppCore::doPayReturnMonthbillTask(Json::Value task){
    cout<<"doPayReturnMonthbillTask"<<endl;
    Json::Value res;
    res["head"]="result";res["part"]="pay";
    res["func"]="returnmonthbill";res["user"]=task["user"].asString(); 

    DBsql mydb;
    if(!mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb)){
        res["result"]="error: mysql initDB error";
        return res;
    }
    //解析 查询

    string sqlstr="select * from `pay`  where time like ";
    sqlstr=sqlstr+"'"+task["data"]["year"].asString()+"-"+task["data"]["month"].asString()+"%'";
    vector<vector<string>> data= mydb.getDataWithSQL(sqlstr);
    if(data.size()==0){
        res["result"]="error: mysql getDataWithSQL return empty";
        return res;
    }

    for(int i=0;i<data.size();i++){
        Json::Value inin;
        inin["time"]=data[i][0];
        inin["moneychange"]=data[i][1];
        inin["money"]=data[i][2];
        inin["ps"]=data[i][3];
        inin["star"]=data[i][4];
        res["data"].append(inin);
    }
    res["result"]="ok";
    return res;
}


Json::Value homeAppCore::doSaveTask(Json::Value task){
    Json::Value res;
    if(task["func"].asString()=="changetarget"){
        res=doSaveChangeTargetTask(task);
    }
    else if(task["func"].asString()=="changemoney"){
        res=doSaveChangeMoneyTask(task);
    }
    else if(task["func"].asString()=="return"){
        res=doSaveReturnTask(task);
    }
    else if(task["func"].asString()=="returnlast"){
        res=doSaveReturnLastTask(task);
    }

    return res;
}
Json::Value homeAppCore::doSaveChangeMoneyTask(Json::Value task){
    cout<<"doSaveChangeMoneyTask"<<endl;
     Json::Value res;
    res["head"]="result";res["part"]="save";
    res["func"]="changemoney";res["user"]=task["user"].asString();

    float changemoney=task["data"]["changeMoney"].asFloat();
    string psstr=task["data"]["ps"].asString();
    bool star=task["data"]["star"].asBool();
    if(save_changeMoney(changemoney,psstr,star)==true){
        res["result"]="ok";
    }
    else{
        res["result"]="error";
    }


    return res;
}
Json::Value homeAppCore::doSaveChangeTargetTask(Json::Value task){
    cout<<"doSaveChangeTargetTask"<<endl;
     Json::Value res;
    res["head"]="result";res["part"]="save";
    res["func"]="changetarget";res["user"]=task["user"].asString();
    
    
    res["result"]="working ...";
    return res;
}
Json::Value homeAppCore::doSaveReturnTask(Json::Value task){
    cout<<"doSaveReturnTask"<<endl;
    Json::Value res;
    res["head"]="result";res["part"]="save";
    res["func"]="return";res["user"]=task["user"].asString();

    DBsql mydb;
    if(!mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb)){
        res["result"]="error: mysql initDB error";
        return res;
    }

    vector<vector<string>> data= mydb.getAllData("savemoney");
    if(data.size()==0){
        res["result"]="error: mysql getAllData return empty";
        return res;
    }
    
    for(int i=0;i<data.size();i++){
        Json::Value inin;
        inin["time"]=data[i][0];
        inin["target"]=data[i][1];
        inin["money"]=data[i][2];
        inin["targetchange"]=data[i][3];
        inin["moneychange"]=data[i][4];
        inin["ps"]=data[i][5];
        inin["star"]=data[i][6];
        res["data"].append(inin);
    }

    res["result"]="ok";
    return res; 
}
Json::Value homeAppCore::doSaveReturnLastTask(Json::Value task){
    cout<<"doSaveReturnLastTask"<<endl;
    Json::Value res;
    res["head"]="result";res["part"]="save";
    res["func"]="returnlast";res["user"]=task["user"].asString();

    DBsql mydb;
    mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb);
    int tableLen=mydb.getDataLen("savemoney");

    vector<vector<string>> data= mydb.getData("savemoney",tableLen-1,1);
    if(data.size()!=1){
        cout<<"Error:doSaveReturnLastTask getData lenght != 1"<<endl;
        res["result"]="Error:doSaveReturnLastTask getData lenght != 1";
        return res;
    }

    Json::Value inin;
    inin["time"]=data[0][0];
    inin["target"]=data[0][1];
    inin["money"]=data[0][2];
    inin["targetchange"]=data[0][3];
    inin["moneychange"]=data[0][4];
    inin["ps"]=data[0][5];
    inin["star"]=data[0][6];
    res["data"]=inin;

    res["result"]="ok";
    return res;

}




bool homeAppCore::save_changeTarget(float change,string ps,bool star){
    cout<<"save_changeTarget"<<endl;
    DBsql mydb;
    mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb);
    int tableLen=mydb.getDataLen("savemoney");
    vector<vector<string>> data= mydb.getData("savemoney",tableLen-1,1);
    if(data.size()!=1){
        cout<<"Error:save_changeTarget getData lenght != 1"<<endl;
        return false;
    }

    float target= atof(data[0][1].c_str()); 
    cout<<"lasttarget: "<<target<<endl;
    float curtarget=target+change;
    cout<<"curtarget: "<<curtarget<<endl;
    //addData
    vector<string> inin;
    DateTime t=DateTime();
    inin.push_back(t.toString());
    inin.push_back(to_string(curtarget));//target
    inin.push_back(data[0][2]);//money
    inin.push_back(to_string(change));//targetchange
    inin.push_back("0");//moneychange
    inin.push_back(ps);
    inin.push_back(star?"true":"false");
    bool resb= mydb.addData("savemoney",inin); 

    return resb;
}
bool homeAppCore::save_changeMoney(float change,string ps,bool star){
    cout<<"save_changeMoney"<<endl;
    DBsql mydb;
    mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb);
    int tableLen=mydb.getDataLen("savemoney");
    vector<vector<string>> data= mydb.getData("savemoney",tableLen-1,1);
    if(data.size()!=1){
        cout<<"Error:save_changeMoney getData lenght != 1"<<endl;
        return false;
    }

    float money= atof(data[0][2].c_str()); 
    cout<<"lastmoney: "<<money<<endl;
    float curmoney=money+change;
    cout<<"curmoney: "<<curmoney<<endl;
    //addData
    vector<string> inin;
    DateTime t=DateTime();
    inin.push_back(t.toString());
    inin.push_back(data[0][1]);//target
    inin.push_back(to_string(curmoney));//money
    inin.push_back("0");//targetchange
    inin.push_back(to_string(change));//moneychange
    inin.push_back(ps);
    inin.push_back(star?"true":"false");
    bool resb= mydb.addData("savemoney",inin); 

    return resb;
}
vector<string> homeAppCore::save_returnlast(){
    vector<string> res;
    DBsql mydb;
    mydb.initDB(mysqlurl,mysqluser,mysqlpasswd,mysqldb);
    int tableLen=mydb.getDataLen("savemoney");
    vector<vector<string>> data= mydb.getData("savemoney",tableLen-1,1);
    if(data.size()!=1){
        cout<<"Error:save_returnlast getData lenght != 1"<<endl;
        return res;
    }
    return data[0];
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
    cout<<"goin mydb.addData"<<endl;
    bool resb= mydb.addData("pay",inin); 
    cout<<"finish mydb.addData"<<endl;
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
