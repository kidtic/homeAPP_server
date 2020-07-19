/*************************************************************************
    > File Name: dpsql.h
    > Author:zouzhiqiang-kidtic
    > Mail:592113107@qq.com 
    > github:kidtic
    > 说明：提供了通用的c++接口，对于查询结果提供各种数据结构的支持，
           对于通用字段，可以通过自定义结构体，来得到查询值、增加数据、改数据。
           对于特殊数据：声音、图片、等信息，提供形如opencv、pcl、等库的接口。
           方便存储数据
 ************************************************************************/

#ifndef _MYDB_H
#define _MYDB_H
#include<iostream>
#include<string>
#include<mysql/mysql.h>
#include <vector>
using namespace std;

class DBsql
{
    public:
        DBsql();
        ~DBsql();
        bool initDB(string host,string user,string pwd,string db_name); //连接mysql
        //执行sql查询语句
        bool query(string sql); 
        vector<vector<string>> getDataWithSQL(string sql);
        //返回这个数据表的所有数据  
        vector<vector<string>> getAllData(string table);
        vector<vector<string>> getData(string table,int start,int len,bool dec=false);
        int getDataLen(string table);

        //增加数据
        bool addData(string table,vector<string> cols,vector<string> data);
        bool addData(string table,vector<string> data);
        //修改数据用sql语法
        bool updateDataWithSQL(string table,string set,string where);


    private:
        MYSQL *mysql;          //连接mysql句柄指针
        MYSQL_RES *result=NULL;    //指向查询结果的指针
        MYSQL_ROW row;       //按行返回的查询信息
        string errormsg;
};






#endif
