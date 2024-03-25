#ifndef _FILEOP_H
#define _FILEOP_H

#include "tcpSocket.h"
/****server****/
// 读取文件
//bool read_file(const char *file_name);
// (&读取)发送文件
bool send_file(SOCKET f,const char *file_name);

/****client****/
// 接收(&保存)文件
bool recv_file(SOCKET s,const char *file_name);
// 保存文件
//bool save_file(const char *file_name);

#endif