#include <homeAppSys.h>

homeAppSys::homeAppSys(string configPath)
{   
    cout<<"load config file"<<endl;
    Json::Reader jsonreader;
    ifstream fs(configPath,ios::binary);
    if(!jsonreader.parse(fs,mConfig)){
        cout << "config json data open error" << endl;
		fs.close();
    }
    cout<<"users load"<<endl;
    int usersNum = mConfig["users"].size();
    for(int i=0;i<usersNum;i++){
        users.push_back(AppUser(mConfig["users"][i]["username"].asString(),
                                mConfig["users"][i]["passwd"].asString()));
        cout<<users.back().username<<endl;

    }
    cout<<"homeAppComm init"<<endl;
    mComm = new homeAppComm(mConfig["port"].asInt(),users[0].passwd);
    cout<<"homeAppCore init"<<endl;
    mCore = new homeAppCore(this->users,mConfig["mysqlurl"].asString(),
                            mConfig["mysqluser"].asString(),
                            mConfig["mysqlpasswd"].asString(),
                            mConfig["mysqldb"].asString(),
                            mConfig["version"].asString());
    mInterestRate=mConfig["InterestRate"].asFloat();
    cout<<"sys finish"<<endl;


}

homeAppSys::~homeAppSys()
{
    delete mComm;
    delete mCore;
    
}

void homeAppSys::timerun(){

    while (1)
    {
        /* code */
        DateTime t=DateTime();
        //每天8点的定时算利息
        if(t.hour==19 && t.minute==45 && t.sec==50){
            //获得最后一个
            vector<string> savedata=mCore->save_returnlast();
            float target=atof(savedata[1].c_str());
            float money=atof(savedata[2].c_str());
            float rate;
            //判断是正利率还是负利率
            if(money>=target)rate=mInterestRate;
            else rate=-mInterestRate;
            float moneychange=money*rate/10000.0;
            mCoreMutex.lock();
            mCore->save_changeMoney(moneychange,"万元计划利息");
            mCoreMutex.unlock();

            //CD时间两秒
            sleep(2);

        }

        //循环等待时间
        usleep(100000);
    }
    
}


void homeAppSys::run(){

    //线程
    thread th(&homeAppSys::timerun,this);

    while(1){
        if(mComm->waitRequest()==false){
            continue;
        }
        //接受命令  
        Json::Value jsontask;

        if(!mComm->readjson(jsontask)){
            cout<<"无法解析json"<<endl;
            close(mComm->socket_fd);
            continue;
        }
        //根据jsontask来执行相应的指示，并返回结果json
        Json::Value resultjson;
        mCoreMutex.lock();
        resultjson=mCore->doTask(jsontask);
        mCoreMutex.unlock();

        //结果json传输回去
        cout<<"回传数据"<<endl;
        mComm->sendjson(resultjson);
        close(mComm->socket_fd);

    }
}