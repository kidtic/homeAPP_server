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
    "result":"ok",
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
服务端会有n个通知队列，对应着每一个用户，如果服务器有需要告诉某一用户的消息，则可以在对应的队列中进行入队

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
    "result":"ok",
    "data":{
        "version":"1.5.0"
    }
}
```

---------
## 3-1.pay功能请求：余额变动
* 客户端发送：
```json
{
    "head":"request",  
    "part":"pay",
    "func":"change",
    "user":"root",       //用户名称
    "data":{
        "changeNumber":-1.5,    //变动数额
        "ps":"说明 $[img](path)",  //$后面的全是附加链接，在显示的时候不用管
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
    "result":"ok"    //  ok/error:error content
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
    "result":"ok",   //ok/error:
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
## 3-3.pay功能请求：返回账单最后一行
```json
{
    "head":"request",  
    "part":"pay",
    "func":"returnlast",
    "user":"kk",       //用户名称
    "time":"2020-07-14 17:50:23"
}
```
```json
{
    "head":"result",  
    "part":"pay",
    "func":"returnlast",
    "user":"kk",       //用户名称
    "result":"ok",
    "data":{
            "time":"",
            "moneychange":"",
            "money":"",
            "ps":"",
            "star":""
        }
        
}
```
## 3-4.pay功能请求：返回yyyy年mm月份的账单
```json
{
    "head":"request",  
    "part":"pay",
    "func":"returnmonthbill",
    "user":"kk",       //用户名称
    "time":"2020-07-14 17:50:23",
    "data":{
        "month":"07",
        "year":"2020"
    }
}
```
* 服务端返回
```json
{
    "head":"result",  
    "part":"pay",
    "func":"returnmonthbill",
    "user":"kk",       //用户名称
    "result":"ok",
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

--------------------

## 4-1.万元计划：改变当前应存
* 客户端发送：
```json
{
    "head":"request",  
    "part":"save",
    "func":"changetarget",
    "user":"zzq",       //用户名称
    "ip":"",            //用户的ip地址
    "data":{
        "changeTarget":-1.5,    //变动数额
        "ps":"说明",
        "star":false            //标记，一般为false
    }
}
```
* 服务端返回
```json
{
    "head":"result",  
    "part":"save",
    "func":"changetarget",
    "user":"zzq",       //用户名称
    "ip":"",            //用户的ip地址
    "result":"ok"     //ok/error
}
```
## 4-2.万元计划：改变当前储蓄
* 客户端发送：
```json
{
    "head":"request",  
    "part":"save",
    "func":"changemoney",
    "user":"zzq",       //用户名称
    "ip":"",            //用户的ip地址
    "data":{
        "changeMoney":-1.5,    //变动数额
        "ps":"说明",
        "star":false            //标记，一般为false
    }
}
```
* 服务端返回
```json
{
    "head":"result",  
    "part":"save",
    "func":"changemoney",
    "user":"zzq",       //用户名称
    "ip":"",            //用户的ip地址
    "result":"ok"     //ok/error
}
```

## 4-3.万元计划：返回列表
```json
{
    "head":"request",  
    "part":"save",
    "func":"return",
    "user":"kk",       //用户名称
}
```
* 服务端返回
```json
{
    "head":"result",  
    "part":"save",
    "func":"return",
    "user":"kk",       //用户名称
    "result":"ok",
    "data":[
        {
            "time":"",
            "target":"",
            "money":"",
            "targetchange":"",
            "moneychange":"",
            "ps":"",
            "star":""
        },
        {
            "time":"",
            "target":"",
            "money":"",
            "targetchange":"",
            "moneychange":"",
            "ps":"",
            "star":""
        }
    ]
}
```

## 4-4 万元计划：返回最后一列数据
```json
{
    "head":"request",  
    "part":"save",
    "func":"returnlast",
    "user":"kk",       //用户名称
}
```
* 服务端返回
```json
{
    "head":"result",  
    "part":"save",
    "func":"returnlast",
    "user":"kk",       //用户名称
    "result":"ok",      //ok/error
    "data":{
        "time":"",
        "target":"",
        "money":"",
        "targetchange":"",
        "moneychange":"",
        "ps":"",
        "star":""
    }
    
}
```

## 5-1.上传文件到服务器
分为两个步，第一步一个来回确认，第二部发送文件。
post 一般都会带有一个大文件，所以Post的返回变量中，必须统一带有大容量的byte数组
```json
{
    "head":"request",
    "part":"post",  //表示需要带上大文件
    "func":"upload",
    "user":"root",
    "data":{
        "TargetDir":"xxx/xxx/",
        "filename":"xxx.xx",
        "size":154258,   //字节单位
        "packageSize":1024,
        "packageNum":151 //151个包（最后一个包可能有多余的）
    }
}
```
服务器返回
```json
{
    "head":"result",
    "part":"post",  //表示需要带上大文件
    "func":"upload",
    "user":"kk",
    "result":"ok"
}
```
开始分包传输文件



