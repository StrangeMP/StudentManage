#include<processthreadsapi.h>
#include"tcpSocket.h"
#include"fileTrans.h"


#define PORT 1234
#define CLI_NUM 1000

SOCKET cfd[CLI_NUM];
bool cfd_status[CLI_NUM];
DWORD thread_id[CLI_NUM];
char buffer[CLI_NUM][64];

//thread
DWORD WINAPI *file_trans(int i) 
{
    sendfile(cfd[i],"pic.jpg"); 

    recv(cfd[i],buffer[i],64,0);
    printf("client %d:%s\n",i,buffer[i]);
    closesocket(cfd[i]);//关闭连接
    printf("client %d disconnected\n",i);
    cfd_status[i]=false;
    return 0;
}


int main()
{
    WSA_start();
    
    //initialize server
    char buffer[1024]="";
    SOCKET sfd=init_server_socket(PORT);

    //initialize client
    for(int i=0;i<CLI_NUM;i++)
    {
        thread_id[i]=i;
        if(cfd_status[i]==false)
        {
            cfd[i]=accept(sfd,NULL,NULL);//后两个参数用于指定必须连接的client,一般为空
            if(cfd[i]==INVALID_SOCKET) err("accept");
            cfd_status[i]=true;
        }
        else printf("server busy\n");
        //create thread
        //参数依次是：指向SECURITY_ATTRIBUTES结构的指针,线程栈的最大大小
        //指向线程函数的指针,传递给线程函数的参数
        //线程创建的标记,线程标识符的地址
        CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)file_trans,(LPVOID)i,0,thread_id+i);
        if(i==CLI_NUM-1) i=-1;
    }


    closesocket(sfd);
    WSA_clean();

    return 0;
}