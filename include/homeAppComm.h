/*************************************************************************
    > File Name: homwAppComm.h
    > Author:zouzhiqiang-kidtic
    > Mail:592113107@qq.com 
    > github:kidtic
    > 说明：提供家庭APP的网络通讯协议
 ************************************************************************/
#ifndef CRYPTOPP_H
#define CRYPTOPP_H
 
#include <iostream>
#include <fstream>
#include <sstream>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/base64.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <jsoncpp/json/json.h>


using namespace std;


#define MAXLINE 102400  


class homeAppComm
{
private:
    /* data */
    ::byte s_key[CryptoPP::AES::DEFAULT_KEYLENGTH];
    ::byte s_iv[CryptoPP::AES::DEFAULT_KEYLENGTH];
    
    int port;
    struct sockaddr_in servaddr;  
public:
    int server_fd;
    int socket_fd;
    

public:

/************************************************************
    @brief:
        构造函数完成网络通讯的初始化
    @param:
        port:设置监听端口
        passwd:设置通讯密码
*************************************************************/
    homeAppComm(int port,std::string passwd);
    ~homeAppComm();



    //-----------加密
/************************************************************
    @brief:
        加密初始化，处理密码字符串。
    @param:
        passwd:设置通讯密码
    @return:
        是否成功
*************************************************************/
    bool passwdinit(const std::string& passwd);

/************************************************************
    @brief:
        加密算法，
    @param:
        inputPlainText:输入原文字符串
    @return:
        密文字符串
*************************************************************/
    std::string encrypt(const std::string& inputPlainText);

/************************************************************
    @brief:
        解密算法，
    @param:
        cipherTextHex:输入密文字符串
    @return:
        原文字符串
*************************************************************/
    std::string decrypt(const std::string& cipherTextHex);

/************************************************************
    @brief:
        Base64编码解码
*************************************************************/
    std::string DecoderBase64(const ::byte* src, const int len);
    std::string EncoderBase64(::byte *s, unsigned int s_len);


    //-----网络通讯
/************************************************************
    @brief:
        从连接的套接子读取密文信息
    @return:
        密文字符串
*************************************************************/
    string readfromSocket();

/************************************************************
    @brief:
        将密文通过套接子发送出去
    @param:
        msg：密文字符串
    @return:
        是否发送成功 
*************************************************************/
    bool sendtoSocket(string msg);

/************************************************************
    @brief:
        分包发送一个比较大的文件
    @param:
        data:数据
        size:数据大小
        packageSize:包大小
    @return:
        是否发送成功 
*************************************************************/
    bool uploadtoSocket(::byte *data,int size,int packageSize);

/************************************************************
    @brief:
        分包接受一个比较大的文件
    @param:
        data:数据
        size:数据大小
        packageSize:包大小
    @return:
        是否发送成功 
*************************************************************/
    bool downloadfromSocket(::byte *data,int size,int packageSize);

/************************************************************
    @brief:
        初始化服务器，包括服务器套接子绑定，开启监听
    @return:
        是否成功
*************************************************************/
    bool initServer();

/************************************************************
    @brief:
        调用accept方法，阻塞在函数中，
    @return:
        是否成功
*************************************************************/
    bool waitRequest();

//---------------------读取解密后的msg

/************************************************************
    @brief:
        读取原文，返回的是json格式的字符串
    @return:
        json格式的字符串
*************************************************************/
    string readmsg();
/************************************************************
    @brief:
        发送原文，发送的是json格式的字符串
    @prarm:
        json格式的字符串
*************************************************************/
    bool sendmsg(string msg);

//-----------------读取解密后的json

/************************************************************
    @brief:
        读取原文，返回的是json格式
    @return:
        json格式的字符串
*************************************************************/
    bool readjson(Json::Value& root);
/************************************************************
    @brief:
        发送json对象
    @prarm:
        json对象
*************************************************************/
    bool sendjson(Json::Value msg);

};


#endif

