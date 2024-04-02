#include"tcpSocket.h"

#define PORT 1234

int main()
{
    WSA_start();

    SOCKET fd=connect_client_to("127.0.0.1",PORT);//use AF_INET——ipv4
    
    //行为
    printf("tap any key to disconnect\n");
    getchar();
    send(fd,"disconnect",10,0);

    WSA_clean();
}
