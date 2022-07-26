#include "socketNetwork.hpp"
#include "log.hpp"

#define SOCKET_NW           "Socket Network"

void SocketNetwork::createSocket(int &sockfd)
{
    logSetThreadName(SOCKET_NW);
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        LOG(ERROR, "socket creation failed");
        exit(EXIT_FAILURE);
    }
}

void SocketNetwork::setMem(sockaddr_in &servaddr, sockaddr_in &cliaddr)
{
    logSetThreadName(SOCKET_NW);
    bzero(&servaddr, sizeof(servaddr));
    bzero(&cliaddr, sizeof(cliaddr));
}

void SocketNetwork::setServerInfo(sockaddr_in &seraddr)
{
    logSetThreadName(SOCKET_NW);
    seraddr.sin_addr.s_addr = INADDR_ANY;
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORT);
}

void SocketNetwork::bindServ(int &sockfd, sockaddr_in &seraddr)
{
    logSetThreadName(SOCKET_NW);
    int len = sizeof(seraddr);
    if ( bind(sockfd, (const struct sockaddr *)&seraddr, sizeof(seraddr)) < 0 )
    {
        LOG(ERROR, "Bind failed");
        exit(EXIT_FAILURE);
    }
}

void SocketNetwork::sendMsg(int &sockfd, char* msg, sockaddr_in &addr)
{
    logSetThreadName(SOCKET_NW);
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

void SocketNetwork::receiveMsg(int &sockfd, sockaddr_in &addr)
{
    logSetThreadName(SOCKET_NW);
    int len = sizeof(addr);
    int size = 0;
    bufferNetwork = new char [LENGTH];
    size = recvfrom(sockfd, (char *) bufferNetwork, LENGTH, MSG_WAITALL, (sockaddr *)&addr, (socklen_t *)&len);
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