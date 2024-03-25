#include"tcpSocket.h"
#include"fileTrans.h"

#define PORT 1234

int main()
{
    WSA_start();
    char buffer[1024]="";
    SOCKET fd=init_client_socket("10.67.118.21",PORT);//use AF_INET——ipv4
    
    //行为
    
    WSA_clean();
}