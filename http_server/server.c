#include"tcpSocket.h"
#include"fileTrans.h"

#define PORT 1234
#define CLI_NUM 3


int main()
{
    WSA_start();
    
    //initialize server
    char buffer[1024]="";
    SOCKET sfd=init_server_socket(PORT);

    //initialize client
    SOCKET cfd=accept(sfd,NULL,NULL);//后两个参数用于指定必须连接的client,一般为空
    if(cfd==INVALID_SOCKET) err("accept");

    //行为
    send_length(cfd,"data.json");
    send_file(cfd,"pic.jpg");

    WSA_clean();

    return 0;
}