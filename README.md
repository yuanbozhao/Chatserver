# chatserver
可以工作在nginx tcp负载均衡环境中的集群聊天服务器和客户端源码  基于muduo实现。

# 环境
 ubuntu 版本为: 18.04.1-Ubuntu 
 		mysql  版本为： 14.14-mysql 
 		nginx  版本为： 1.12
 		redis  版本为： 4.0.9

# 框架

主要由网络层、应用层、服务层以及交互层组成。

## 网络层
  利用muduo网络库实现网络层与应用层解耦
## 应用层
 使用C++11 map 以及包装器和绑定器实现网路层对不同功能的回调消息，主要通过消息id来实现
 ### 主要功能：
> *  LOGIN_MSG,         // 登录消息
> *  LOGIN_MSG_ACK,     // 登录响应消息
> *  LOGINOUT_MSG,      // 注销消息
> *  REG_MSG,           // 注册消息
> *  REG_MSG_ACK,       // 注册响应消息
> *  ONE_CHAT_MSG,      // 聊天消息
> *  ADD_FRIEND_MSG,    // 添加好友消息
> *  CREATE_GROUP_MSG,  // 创建群组
> *  ADD_GROUP_MSG,     // 加入群组
> *  GROUP_CHAT_MSG,    // 群聊天
使用私有协议 JSON来进行配置

## 数据层

  ORM ： Object-Relational Mapping 对象关系映射

主要负责MySQL中表的调用，表如下：

  user
  		friend
  		groupuser
  		offlinemessage
  		group

## 交互层
### nginx
  实现负载均衡配置，采用轮询的方式。

### redis

不同线程通信采用redis中的发布-订阅功能，每个connection的id 订阅subscribe到消息队列中，不同线程之间的交互采用发布publish对方id实现

# 使用步骤

##  数据库配置信息

在 /src/server/db/db.cpp

> * static string server = "127.0.0.1";
> * static string user = "root";
> * static string password = "123456";
> * static string dbname = "chat";

## 配置表信息

```
create databse chat;
use chat;
create table user(
    -> id int primary key auto_increment comment '主键',
    -> name varchar(10) not null unique comment '用户名',
    -> password varchar(50) not null comment '用户密码',
    -> state enum('online','offline') default 'offline' comment '状态'
    -> )comment 'User表';
create table friend(
    -> userid int not null,
    -> friendid int not null comment '好友id',
    -> primary key (userid, friendid)
    -> )engine=innodb default charset=utf8;
create table allgroup(
    -> id int not null primary key auto_increment,
    -> groupname varchar(50) not null unique,
    -> groupdesc varchar(200) default '' comment '组功能描述'
    -> )engine=innodb default charset=utf8;
create table groupuser(
    -> groupid int not null,
    -> userid int not null,
    -> grouprole enum('creator','normal') default 'normal',
    -> primary key (groupid, userid)
    -> )engine=innodb default charset=utf8;
```

## 配置nginx

**!!! 在超级用户下运行**

1、解压nginx包

```bash
tar -axvf name
```

2、配置nginx

```bash
./configure --with-stream
make && make install
```

3、修改配置参数
		在 nginx中的 conf中配置需要的参数，打开以下文件，加入

```
 vim /usr/local/nginx/conf/nginx.conf
```

  ```bash
  stream{
      upstream MyServer {
          hash $remote_addr consistent;  // 哈希配置
          server 127.0.0.1:6000 weight max_fails=3 fail_timeout = 30s;
          server 127.0.0.1:6001 weight max_fails=3 fail_timeout = 30s;
        }
    server {
      proxy_connect_timeout 1s;
      listen 8000;    // 连接该端口
      proxy_pass MySserver;
      tcp_nodelay on;
    }
  }
  ```
  4、重新加载配置文件

 ```bash
 nginx -s reload 
 ```

 5、启动nginx

```bash
 /usr/local/nginx/sbin/
 ./nginx
```

## 配置redis
  1、 安装服务端 apt-get install redis-server
  		2、 安装客户端(C++) git clone https://github.com/redis/hiredis
 		 3、 cd hiredis && make && sudo make install
  		4、 更新配置
    		sudo ldconfig /usr/local/lib

## 编译

```
sh ./build.sh
```

## 运行

```

// 启动server
./ChatServer 127.0.0.1 6000
//client连接nginx
./chatserver 127.0.0.1 8000
```






