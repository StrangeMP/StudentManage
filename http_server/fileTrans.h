#ifndef _FILEOP_H
#define _FILEOP_H

#include "tcpSocket.h"
/****server****/
// 读取文件
//long read_file(const char *file_name);
// 发送长度
bool send_length(SOCKET c, const char *file_name);
// 发送文件
bool send_file(SOCKET c,const char *file_name);

/****client****/
// 接收长度
long recv_length(SOCKET s);
// 接收(&保存)文件
bool recv_file(SOCKET s,const char *file_name,long client_fsize);
// 保存文件
//bool save_file(const char *file_name);

#endif