#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "function_network.hpp"

#define PORT 7878
#define LENGTH 1024

class SocketNetwork
{
public:
    void createSocket(int &sockfd);
    void setMem(sockaddr_in &seraddr, sockaddr_in &cliaddr);
    void setServerInfo(sockaddr_in &servaddr);
    void bindServ(int &sockfd, sockaddr_in &seraddr);
    void sendMsg(int &sockfd, char* msg, sockaddr_in &addr);
    void receiveMsg(int &sockfd, sockaddr_in &addr);
    
private:

};