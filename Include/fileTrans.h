#ifndef _FILEOP_H
#define _FILEOP_H

#include "tcpSocket.h"
/****server****/
// 发送文件
bool sendfile(SOCKET f, const char *file_name);

/****client****/
// 接收文件
bool recvfile(SOCKET s, const char *file_name);

#endif