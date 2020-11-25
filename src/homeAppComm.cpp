/*************************************************************************
    > File Name: homwAppComm.cpp
    > Author:zouzhiqiang-kidtic
    > Mail:592113107@qq.com 
    > github:kidtic
    > 说明：提供家庭APP的网络通讯协议
 ************************************************************************/
#include "homeAppComm.h"

using namespace std;


homeAppComm::homeAppComm(int port,std::string passwd)
{
    this->port=port;
    passwdinit(passwd);
    initServer();
}

homeAppComm::~homeAppComm()
{
    close(server_fd);
}


bool homeAppComm::passwdinit(const string& passwd) {
    if (passwd.size() > CryptoPP::AES::DEFAULT_KEYLENGTH) {
        return false;
    }

    int len = passwd.length();
    for(int i = 0; i < CryptoPP::AES::DEFAULT_KEYLENGTH; i++) {
        if(i>=len){
            s_key[i]='0';
        }
        else{
            s_key[i] = passwd[i];
        }
        
        
    }

    for(int i = 0; i < CryptoPP::AES::DEFAULT_KEYLENGTH; i++) {
        s_iv[i]='0';
    }
    
 
    //memset(s_key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    //memset(s_iv, 0x00, CryptoPP::AES::BLOCKSIZE);
    return true;
}
 
 
 
string homeAppComm::encrypt(const string& plainText)
{
    /*
    if ((plainText.length() % CryptoPP::AES::BLOCKSIZE) != 0) {
        return "";
    }
    */
 
    string res;
    try {
        string cipherText;
        CryptoPP::AES::Encryption aesEncryption(s_key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, s_iv);
        //CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( cipherText ), CryptoPP::StreamTransformationFilter::NO_PADDING);
        CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( cipherText ));
        stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plainText.c_str() ), plainText.length() );
        stfEncryptor.MessageEnd();
 
        //base64
        res=EncoderBase64((::byte*)cipherText.c_str(), cipherText.size());

    } catch (const std::exception &e) {
        res = "";
    }
    return res;
}
 
string homeAppComm::decrypt(const string& cipherTextHex)
{
    /*
    if(cipherTextHex.empty()) {
        return string();
    }
    if ((cipherTextHex.length() % CryptoPP::AES::BLOCKSIZE) != 0) {
        return string();
    }
    */
 
    string decryptedText;
 
    
    //base64
    string mistr= DecoderBase64((::byte*)cipherTextHex.c_str(),cipherTextHex.length());


 
    try {
        CryptoPP::AES::Decryption aesDecryption(s_key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, s_iv );
        //CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedText ),CryptoPP::StreamTransformationFilter::NO_PADDING);
        CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedText ));
        stfDecryptor.Put( reinterpret_cast<const unsigned char*>( mistr.c_str() ), mistr.size());
 
        stfDecryptor.MessageEnd();
    } catch (const std::exception &e) {
        decryptedText = "";
    }
 
    return decryptedText;
}


string homeAppComm::DecoderBase64(const ::byte* src, const int len)
{
    std::string decoded; 

    CryptoPP::StringSource ss(src, len, true, 
        new CryptoPP::Base64Decoder(new CryptoPP::StringSink(decoded)) 
    ); 
    return decoded;
}

std::string homeAppComm::EncoderBase64(::byte *s, unsigned int s_len)
{
    std::string encoded;
    CryptoPP::StringSource ss(s, s_len, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(encoded),false));
    return encoded;
}



string homeAppComm::readfromSocket()
{
    int n;  
    char buff[MAXLINE];
    //接受客户端传过来的数据  
    n = recv(socket_fd, buff, MAXLINE, 0); 
    string res;
    res.assign(buff,n);
    return res; 
}

bool homeAppComm::sendtoSocket(string msg)
{
    msg+="\r\n";
    //向客户端发送回应数据  
    if(send(socket_fd, msg.c_str(),msg.size(),0) == -1){
        perror("send error"); 
        return false;
    }        
     
    return true;
}

bool homeAppComm::downloadfromSocket(::byte *data,int size,int packageSize)
{
    int fullPackageNum;//完整的包一共有多少
    int lastPackageSize;//最后一个包有多少有效字节，如果该数为0，则代表全部都是完整包。
    int rn;
    fullPackageNum=size/packageSize;
    lastPackageSize=size%packageSize;
    ::byte* ptr=data;
    int recvsize=0;
    while(recvsize<size){
        rn=recv(socket_fd,ptr,packageSize,0);
        recvsize+=rn;
        ptr+=rn;
    }
    
    
    cout<<"recv size:"<<recvsize<<endl;
    return true;
}

bool homeAppComm::initServer(){
    if((server_fd=socket(AF_INET,SOCK_STREAM,0))==-1){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);  
        return false;
    }

    //初始化  
    
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。  
    servaddr.sin_port = htons(port);   //设置的端口为DEFAULT_PORT  
    //将本地地址绑定到所创建的套接字上  
    if( bind(server_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){  
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno); 
        abort(); 
        return false;
    }  
    //开始监听是否有客户端连接  
    if( listen(server_fd, 10) < 0){  
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);  
        return false;  
    }
    return true;
}

bool homeAppComm::waitRequest()
{
    printf("======waiting for client's request======\n");  
    
    //解决Invalid argument(errno: 22)问题
    sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t lenaddr=sizeof(client_addr);
    if( (socket_fd = accept(server_fd, (struct sockaddr*)&client_addr, &lenaddr)) == -1){  
        printf("accept socket error: %s(errno: %d)\n",strerror(errno),errno);  
        return false;  
    }
    return true;  
}

string homeAppComm::readmsg(){
    string res;
    res=decrypt(readfromSocket());
    return res;
}
bool homeAppComm::sendmsg(string msg){
    string outdata;
    
    outdata=encrypt(msg);
    //cout<<outdata<<endl;
    return sendtoSocket(outdata);
}

bool homeAppComm::readjson(Json::Value& root){
    string jsonstr=readmsg();
    Json::Reader rd;
    bool res=rd.parse(jsonstr,root);
    return res;
}

bool homeAppComm::sendjson(Json::Value msg){
    string outstr=msg.toStyledString();
    bool res=sendmsg(outstr);
    return res;
}


