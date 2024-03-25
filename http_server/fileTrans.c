#include"fileTrans.h"
long server_fsize;
long client_fsize=1032833;
char* server_buffer;
char* client_buffer;

bool read_file(const char *file_name)
{
    FILE *read=fopen(file_name,"rb");
    if(!read)
    {
        perror("fopen failed:\n");
        return false;
    }

    //get file size
    fseek(read,0,SEEK_END);
    server_fsize=ftell(read);
    fseek(read,0,SEEK_SET);
    printf("file size: %d\n",server_fsize);

    //clear and alloc memory
    server_buffer=(char*)calloc(server_fsize,sizeof(char));
    if(!server_buffer)
    {
        perror("calloc failed:\n");
        return false;
    }

    //read file content into buffer
    fread(server_buffer,sizeof(char),server_fsize,read);//将文件中的内容读入到内存中

    fclose(read);
    return true;
}

bool send_file(SOCKET c,const char *file_name)
{
    if(!read_file(file_name))
    {
        return false;
    }

    if(send(c,server_buffer,server_fsize,0)==SOCKET_ERROR)
    {
        err("sendFile");
        return false;
    }
    printf("send file success\n");
    return true;
}

bool save_file(const char *file_name)
{
    FILE *write=fopen(file_name,"wb");
    if(!write)
    {
        perror("fopen failed:\n");
        return false;
    }

    //read file content into buffer
    fwrite(client_buffer,sizeof(char),client_fsize,write);//将内存中的内容写入文件中
    fclose(write);
    return true;
}

bool recv_file(SOCKET s,const char *file_name)
{
    client_buffer=(char*)calloc(client_fsize,sizeof(char));
    if(!client_buffer)
    {
        perror("calloc failed:\n");
        return false;
    }
    if(recv(s,client_buffer,client_fsize,0)==SOCKET_ERROR)
    {
        err("recvFile");
        return false;
    }
    printf("receive file success\n");
    if(!save_file(file_name))
    {
        return false;
    }
    return true;
}

