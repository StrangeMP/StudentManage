#include"tcpSocket.h"

//启动windows 的socket asynchronous/异步 dll库
bool WSA_start()
{
    WSADATA WSAData;
    if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
    {
        err("WSAStartup");
        return false;
    }
    return true;
}

//close WSA 的库
bool WSA_clean()
{
    if(WSACleanup() != 0)
    {
        err("WSACleanup");
        return false;
    }
    return true;
}

//initialize server
SOCKET init_server_socket(u_short port)
{
    //1.create an empty socket
    SOCKET fd=socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);//ipv4地址族,流式传输,tcp协议
    if(fd == INVALID_SOCKET)
    {
        err("init_server_socket");
        return INVALID_SOCKET;
    }

    //2.bind the socket to the address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);//port,转换big ending、little ending
    addr.sin_addr.S_un.S_addr = INADDR_ANY;//local ip
    if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        err("bind");
        return INVALID_SOCKET;
    }
    //3.listen
    listen(fd, 4);

    return fd;
}

//intialize client
SOCKET init_client_socket(const char *ip, u_short port)
{
    //1.create an empty socket
    SOCKET fd=socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);//ipv4地址族,流式传输,tcp协议
    if(fd == INVALID_SOCKET)
    {
        err("init_client_socket");
        return INVALID_SOCKET;
    }
    //2.connect to the server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);//port,转换big ending、little ending
    addr.sin_addr.S_un.S_addr = inet_addr(ip);//ip
    if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        err("connect");
        return INVALID_SOCKET;
    }
    return fd;
}

