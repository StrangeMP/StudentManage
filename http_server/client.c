#include"tcpSocket.h"
#include"fileTrans.h"

#define PORT 1234

int main()
{
    WSA_start();

    SOCKET fd=init_client_socket("127.0.0.1",PORT);//use AF_INET——ipv4
    
    //行为
    recvfile(fd,"pic_received2.jpg");

    
    WSA_clean();
}