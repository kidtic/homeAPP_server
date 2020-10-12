
#include "dbsql.h"


DBsql::DBsql()
{
    mysql=mysql_init(NULL);   //初始化数据库连接变量
    if(mysql==NULL)
    {
        errormsg=string(mysql_error(mysql));
        cout << "mysql_init Error: " << errormsg;
        
    }
}

DBsql::~DBsql()
{
    if(mysql!=NULL)  //关闭数据连接
    {
        mysql_close(mysql);
    }
}

/************************************************************
    @brief:
        通过函数mysql_real_connect建立一个数据库连接  
        成功则返回true，失败返回NULL，连接句柄是类内部成员mysql
    @param:
        host:mysql数据库的域名
        user:数据库用户
        passwd:用户密码
        db_name:指定该用户下的哪个数据库
*************************************************************/
bool DBsql::initDB(string host,string user,string passwd,string db_name)
{
    // 函数mysql_real_connect建立一个数据库连接  
    // 成功返回MYSQL*连接句柄，失败返回NULL  
    mysql = mysql_real_connect(mysql, host.c_str(), user.c_str(), passwd.c_str(), db_name.c_str(), 0, NULL, 0);  
    if(mysql == NULL)  
    {  
        errormsg=string(mysql_error(mysql));
        cout << "mysql_real_connect Error: " << errormsg;
        return false;
    }  
    return true;  
}

/************************************************************
    @brief:查询操作
        通过函数mysql_query，来执行sql语句进而对表进行查询操作
        查询操作在字符串前面会自动加上select。
    @param:
        sql:"select "+sql，来对数据表进行查询操作，语法为sql语句
*************************************************************/
bool DBsql::query(string sql)
{
    //+select
    sql="select "+sql;
    

    //mysql_query()执行成功返回0,执行失败返回非0值。
    if (mysql_query(mysql,sql.c_str()))
    {   
        errormsg=string(mysql_error(mysql));
        cout << "Query Error: " << errormsg;
        return false;
    }
    else // 查询成功
    {
        //result 会保存最近一次的查询结果，一旦继续调用，要释放之前的内存
        mysql_free_result(result);
        result = mysql_store_result(mysql);  //获取结果集
        if (result)  // 返回了结果集
        {
           int  num_fields = mysql_num_fields(result);   //获取结果集中总共的字段数，即列数
           int  num_rows=mysql_num_rows(result);       //获取结果集中总共的行数
           for(int i=0;i<num_rows;i++) //输出每一行
            {
                //获取下一行数据
                row=mysql_fetch_row(result);
                if(row<0) break;

                for(int j=0;j<num_fields;j++)  //输出每一字段
                {
                    cout<<row[j]<<"\t\t";
                }
                cout<<endl;
            }

        }
        else  // result==NULL
        {
            if(mysql_field_count(mysql) == 0)   //代表执行的是update,insert,delete类的非查询语句
            {
                // (it was not a SELECT)
                int num_rows = mysql_affected_rows(mysql);  //返回update,insert,delete影响的行数
            }
            else // error
            {
                errormsg=string(mysql_error(mysql));
                cout<<"Get result error: "<<mysql_error(mysql);
                return false;
            }
        }
    }

    return true;

}

/************************************************************
    @brief:查询操作
        与query一样，只不过返回了vecter
    @return:
        res[i]数据消息
        res[i][j]列为第i条消息的j字段
*************************************************************/
vector<vector<string>> DBsql::getDataWithSQL(string sql)
{
    vector<vector<string>> res;
    cout<<sql<<endl;
    //mysql_query()执行成功返回0,执行失败返回非0值。
    if (mysql_query(mysql,sql.c_str()))
    {
        errormsg=string(mysql_error(mysql));
        cout<<"Query Error: "<<mysql_error(mysql);
        return res;
    }
    else // 查询成功
    {
       
        
        //result 会保存最近一次的查询结果，一旦继续调用，要释放之前的内存
        mysql_free_result(result);
        result = mysql_store_result(mysql);  //获取结果集
        if (result)  // 返回了结果集
        {
           int  num_fields = mysql_num_fields(result);   //获取结果集中总共的字段数，即列数
           int  num_rows=mysql_num_rows(result);       //获取结果集中总共的行数
           for(int i=0;i<num_rows;i++) //输出每一行
            {
                //获取下一行数据
                row=mysql_fetch_row(result);
                if(row<0) break;
                vector<string> inin;
                for(int j=0;j<num_fields;j++)  //输出每一字段
                {
                    inin.push_back(row[j]);
                }
                res.push_back(inin);

            }
             

        }
        else  // result==NULL
        {
            if(mysql_field_count(mysql) == 0)   //代表执行的是update,insert,delete类的非查询语句
            {
                // (it was not a SELECT)
                int num_rows = mysql_affected_rows(mysql);  //返回update,insert,delete影响的行数
            }
            else // error
            {
                errormsg=string(mysql_error(mysql));
                cout<<"Get result error: "<<mysql_error(mysql);
                return res;
            }
        }
    }

    return res;
}


/************************************************************
    @brief:返回整个数据表的所有数据  
        
    @param:
        table:数据表的名字
    @return:
        res[i]数据消息
        res[i][j]列为第i条消息的j字段
*************************************************************/
vector<vector<string>> DBsql::getAllData(string table){
     //+select
    string sql="select * from `"+table+"`";
    vector<vector<string>> res;
    
    res=getDataWithSQL(sql);
    
    return res;
}

/************************************************************
    @brief:返回位置从start到end的这段的数据 
    @param:
        start:数据起始位置
        len:数据长度
        dec:是否反序。默认不反序（必须要有id）
    @return:
        res[i]数据消息
        res[i][j]列为第i条消息的j字段
*************************************************************/
vector<vector<string>> DBsql::getData(string table,int start,int len,bool dec){
    
    string sql="select * from `"+table+"` "+(dec?"order by id desc":"")+" limit "+to_string(start)+","+to_string(len)+";";
    vector<vector<string>> res;
    
    res=getDataWithSQL(sql);
    return res;
}
/************************************************************
    @brief:返回数据表有多少行数据
*************************************************************/
int DBsql::getDataLen(string table){
    string sql="select count(*) from "+table;
    vector<vector<string>> res;
    
    res=getDataWithSQL(sql);
    if(res.size()==0)return -1;
    if(res[0].size()==0)return -1;
    return atoi(res[0][0].c_str()); 
}

/************************************************************
    @brief:往数据表里插入数据
    @param:
        table:数据表
        data:数据
    @return:
        是否成功
*************************************************************/
bool DBsql::addData(string table,vector<string> data)
{
    string sql="insert into `"+table+"` values(";
    for (size_t i = 0; i < data.size(); i++)
    {
        sql+="\""+data[i]+"\"";
        if(i!=data.size()-1) sql+=",";
    }
    sql+=")";

    cout<<" do sql :  "<<sql<<endl;
    if (mysql_query(mysql,sql.c_str()))
    {
        errormsg=string(mysql_error(mysql));
        cout<<"Query Error: "<<mysql_error(mysql);
        return false;
    }
    cout<<" finish sql :  "<<sql<<endl;
    return true;
}
/************************************************************
    @brief:往数据表里插入数据
    @param:
        table:数据表
        cols:字段
        data:对应的数据
    @return:
        是否成功
*************************************************************/
bool DBsql::addData(string table,vector<string> cols,vector<string> data)
{
    string sql="insert into `"+table+"` (";
    for (size_t i = 0; i < cols.size(); i++)
    {
        sql+="`"+cols[i]+"`";
        if(i!=cols.size()-1)sql+=",";
    }
    sql+=")";
    sql+=" values(";
    for (size_t i = 0; i < data.size(); i++)
    {
        sql+="\""+data[i]+"\"";
        if(i!=data.size()-1) sql+=",";
    }
    sql+=")";

    if (mysql_query(mysql,sql.c_str()))
    {
        errormsg=string(mysql_error(mysql));
        cout<<"Query Error: "<<mysql_error(mysql);
        return false;
    }
    return true;
}
/************************************************************
    @brief:更新数据
    @param:
        table:数据表
        set:设置修改的值,记得加逗号
        where:定位修改哪一行的数据
    @return:
        是否成功
*************************************************************/
bool DBsql::updateDataWithSQL(string table,string set,string where)
{
    string sql="update `"+table+"` set "+set+" where "+where;
    
    if (mysql_query(mysql,sql.c_str()))
    {
        errormsg=string(mysql_error(mysql));
        cout<<"Query Error: "<<mysql_error(mysql);
        return false;
    }
    return true;
}