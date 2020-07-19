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
                            mConfig["mysqldb"].asString());
    cout<<"sys finish"<<endl;


}

homeAppSys::~homeAppSys()
{
    delete mComm;
    delete mCore;
    
}


void homeAppSys::run(){

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
        resultjson=mCore->doTask(jsontask);

        //结果json传输回去
        cout<<"回传数据"<<endl;
        mComm->sendjson(resultjson);
        close(mComm->socket_fd);

    }
}