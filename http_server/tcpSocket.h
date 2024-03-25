#ifndef _TCP_DOCKET_H
#define _TCP_DOCKET_H

#include <winsock2.h>
#include <stdbool.h>
#include <stdio.h>

#define err(msg) printf("[error] %s code: %d line: %d\n", msg, WSAGetLastError(), __LINE__)

// 启动windows 的socket asynchronous/异步 库
bool WSA_start();
// close WSA 的库
bool WSA_clean();

// init server
SOCKET init_server_socket(u_short port);
// init client
SOCKET init_client_socket(const char *ip, u_short port);

#endif