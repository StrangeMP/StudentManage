#include"fileTrans.h"
#include<stdlib.h>

#define FILE_LENGTH 30

char* server_buffer;
char* client_buffer;

long get_size(const char *file_name)
{
    FILE *read=fopen(file_name,"rb");
    if(!read)
    {
        perror("fopen failed:\n");
        return false;
    }

    fseek(read,0,SEEK_END);
    long fsize=ftell(read);
    fclose(read);
    return fsize;
}

bool send_length(SOCKET c, const char *file_name)
{
    char length[30];
    sprintf(length,"%ld",get_size(file_name));
    if(send(c,length,FILE_LENGTH,0)==SOCKET_ERROR)
    {
        err("sendFile");
        return false;
    }
    printf("send file success\n");
    return true;
}

long read_file(const char *file_name)
{
    long fsize=get_size(file_name);
    FILE *read=fopen(file_name,"rb");
    if(!read)
    {
        perror("fopen failed:\n");
        return false;
    }

    //clear and alloc memory
    server_buffer=(char*)calloc(fsize,sizeof(char));
    if(!server_buffer)
    {
        perror("calloc failed:\n");
        return false;
    }

    //read file content into buffer
    fread(server_buffer,sizeof(char),fsize,read);//将文件中的内容读入到内存中

    fclose(read);
    return fsize;
}

bool send_file(SOCKET c,const char *file_name)
{
    long fsize=read_file(file_name);
    if(fsize==0) return false;

    if(send(c,server_buffer,fsize,0)==SOCKET_ERROR)
    {
        err("sendFile");
        return false;
    }

    printf("send file success\n");
    return true;
}

bool sendfile(SOCKET c,const char *file_name)
{
    if(!send_length(c,file_name)) return false;
    if(!send_file(c,file_name)) return false;
    return true;
}



long recv_length(SOCKET s)
{
    char length[30];

    if(recv(s,length,FILE_LENGTH,0)==SOCKET_ERROR)
    {
        err("recvFile");
        return false;
    }

    long fsize=atol(length);
    return fsize;
}

bool save_file(const char *file_name,long fsize)
{
    FILE *write=fopen(file_name,"wb");
    if(!write)
    {
        perror("fopen failed:\n");
        return false;
    }

    //read file content into buffer
    fwrite(client_buffer,sizeof(char),fsize,write);//将内存中的内容写入文件中
    fclose(write);
    return true;
}

bool recv_file(SOCKET s,const char *file_name,long fsize)
{
    client_buffer=(char*)calloc(fsize,sizeof(char));
    if(!client_buffer)
    {
        perror("calloc failed:\n");
        return false;
    }

    if(recv(s,client_buffer,fsize,0)==SOCKET_ERROR)
    {
        err("recvFile");
        return false;
    }
    printf("receive file success\n");
    if(!save_file(file_name,fsize))
    {
        return false;
    }
    return true;
}

bool recvfile(SOCKET s,const char *file_name)
{
    long fsize=recv_length(s);
    if(fsize==0) return false;
    if(!recv_file(s,file_name,fsize)) return false;
    return true;
}
