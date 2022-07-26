#include "socketUe.hpp"
#include "log.hpp"

#define SOCKET_UE           "Socket UE"

void SocketUE::createSocket(int &sockfd)
{
    logSetThreadName(SOCKET_UE);
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        LOG(ERROR, "socket creation failed");
        exit(EXIT_FAILURE);
    }
}

void SocketUE::setMem(sockaddr_in &servaddr)
{
    logSetThreadName(SOCKET_UE);
    bzero(&servaddr, sizeof(servaddr));
}

void SocketUE::setServerInfo(sockaddr_in &seraddr)
{
    logSetThreadName(SOCKET_UE);
    seraddr.sin_addr.s_addr = INADDR_ANY;
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORT);
}

void SocketUE::sendMsg(int &sockfd, char* msg, sockaddr_in &addr)
{
    logSetThreadName(SOCKET_UE);
    if(sendto(sockfd, msg, LENGTH, MSG_CONFIRM, (const sockaddr *)&addr, sizeof(addr)) == -1)
    {
        LOG(ERROR, "Sent fail");
        exit(0);
    }
    else
    {        
        LOG(INFO, "Message sending ...");
    }
}

void SocketUE::receiveMsg(int &sockfd, sockaddr_in &addr)
{
    logSetThreadName(SOCKET_UE);
    int len = sizeof(addr);
    int size = 0;
    bufferUe = new char [LENGTH];
    size = recvfrom(sockfd, (char*)bufferUe, LENGTH, MSG_WAITALL, (struct sockaddr *)&addr, (socklen_t *)&len);
    if(size == -1)
    {
        LOG(ERROR, "Received failed");
        exit(0);
    }
    else
    {
        LOG(INFO, "Message has been received!");
    }
}