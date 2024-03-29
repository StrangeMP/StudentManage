#include<processthreadsapi.h>
#include<string.h>
#include"tcpSocket.h"
#include"fileTrans.h"


#define PORT 1234
#define CLI_NUM 1000

SOCKET cfd[CLI_NUM];
bool cfd_status[CLI_NUM];
DWORD thread_id[CLI_NUM];
char buffer[CLI_NUM][1000000];

//thread
DWORD WINAPI *file_trans(int i) 
{
    recv(cfd[i],buffer[i],1000000,0);
    char *content_length_str = strstr(buffer[i], "Content-Length:");
    int content_length;
    if (content_length_str) 
    {
        content_length_str += strlen("Content-Length:");
        content_length = atoi(content_length_str);
        printf("内容长度为:%d\n", content_length);
    } 
    else 
    {
        printf("没有找到Content-Length头\n");
    }

    char *content_type_str = strstr(buffer[i], "Content-Type:");
    char type[80];
    char *content = strstr(buffer[i], "\r\n\r\n");
    if (content_type_str) 
    {
        content_type_str += strlen("Content-type: ");
        int len=(int)(content-content_type_str);
        snprintf(type,len,"%s",content_type_str);
        printf("内容类型为:%s\n", type);
    }
    else 
    {
        printf("没有找到Content-Type头\n");
    }

    
    if (content) 
    {
        content += strlen("\r\n\r\n");
        printf("内容为:%s\n", content);
    } 
    else 
    {
        printf("没有找到内容\n");
    }
    //handler


    //send_message
    send(cfd[i],content,content_length,0);

    //关闭连接
    memset(buffer[i],0,sizeof(buffer[i]));
    closesocket(cfd[i]);
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
            else cfd_status[i]=true;
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