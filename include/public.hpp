#pragma once

/*
server和client的公共文件
公共方法=> msgid
*/
enum EnMsgType
{
    LOGIN_MSG = 1,     // 登录消息，保证从1开始
    LOGIN_MSG_ACK,     // 登录响应消息
    LOGINOUT_MSG,      // 注销消息
    REG_MSG,           // 注册消息
    REG_MSG_ACK,       // 注册响应消息
    ONE_CHAT_MSG,      // 聊天消息
    ADD_FRIEND_MSG,    // 添加好友消息
    CREATE_GROUP_MSG,  // 创建群组
    ADD_GROUP_MSG,     // 加入群组
    GROUP_CHAT_MSG,    // 群聊天
};
