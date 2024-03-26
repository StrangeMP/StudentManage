#include"tcpSocket.h"
#include"fileTrans.h"

#define PORT 1234

int main()
{
    WSA_start();

    SOCKET fd=init_client_socket("10.67.118.21",PORT);//use AF_INET——ipv4
    
    //行为
    long length =recv_length(fd);
    printf("length:%ld\n",length);
    recv_file(fd,"pic_received.jpg",54731);

    
    WSA_clean();
}