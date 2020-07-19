# 有关Json数据解析说明
从**客户端**发送请求json文件，由**服务端**解析处理后向**客户端**发送结果json文件  
以下对json内容进行说明

## 1.特殊的json请求------请求返回服务端的命令队列
客户端需要不断发送请求1，这样提供了服务端主动联系客户端的机会。  
* 客户端发送：
```json
{
    "head":"request",  
    "part":"basic",
    "func":"return",
    "user":"zzq",       //用户名称
}
```
* 服务端返回
```json
{
    "head":"result",  
    "part":"basic",
    "func":"return",
    "user":"zzq",       //用户名称
    "ip":"",            //用户的ip地址
    "data":[            //data放入服务端对客户端的命令列表
        {
            "part":"chat",
            "func":"msgupdate"   //聊天更新，有人发话了
        },
        {
            //·····
        }
    ]
}
```
## 2.登录返回
* 客户端发送：
```json
{
    "head":"request",  
    "part":"basic",
    "func":"login",
    "user":"zzq",       //用户名称
}
```
* 服务端返回
```json
{
    "head":"result",  
    "part":"basic",
    "func":"login",
    "user":"zzq",       //用户名称
    "data":"ok"
}
```


## 3-1.pay功能请求：余额变动
* 客户端发送：
```json
{
    "head":"request",  
    "part":"pay",
    "func":"change",
    "user":"root",       //用户名称
    "ip":"",            //用户的ip地址
    "data":{
        "changeNumber":-1.5,    //变动数额
        "ps":"说明",
        "star":false            //标记，一般为false
    }
}
```
* 服务端返回
```json
{
    "head":"result",  
    "part":"pay",
    "func":"change",
    "user":"root",       //用户名称
    "ip":"",            //用户的ip地址
    "data":{
        "result":"ok",    //  ok/error:error content
        "time":"2020-07-14 17:50:23",
        "changeNumber":-1.5,
        "money":1536.6,
        "ps":"说明",
        "star":false
    }
}
```

## 3-2.pay功能请求：余额变动返回账单
```json
{
    "head":"request",  
    "part":"pay",
    "func":"return",
    "user":"kk",       //用户名称
    "time":"2020-07-14 17:50:23"
}
```
* 服务端返回
```json
{
    "head":"result",  
    "part":"pay",
    "func":"return",
    "user":"kk",       //用户名称
    "data":[
        {
            "time":"",
            "moneychange":"",
            "money":"",
            "ps":"",
            "star":""
        },
        {
            "time":"",
            "moneychange":"",
            "money":"",
            "ps":"",
            "star":""
        }
    ]
}
```

## 4.万元计划：改变当前应存
* 客户端发送：
```json
{
    "head":"request",  
    "part":"pay",
    "func":"change",
    "user":"zzq",       //用户名称
    "ip":"",            //用户的ip地址
    "data":{
        "changeNumber":-1.5,    //变动数额
        "ps":"说明",
        "star":false            //标记，一般为false
    }
}
```
* 服务端返回
```json
{
    "head":"result",  
    "part":"pay",
    "func":"change",
    "user":"zzq",       //用户名称
    "ip":"",            //用户的ip地址
    "data":{
        "result":"ok",    //  ok/error:error content
        "time":"2020-07-14 17:50:23",
        "changeNumber":-1.5,
        "money":1536.6,
        "ps":"说明",
        "star":false
    }
}
```
## 3.万元计划：改变当前储蓄

