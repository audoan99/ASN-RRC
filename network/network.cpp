#include <string>
#include <cstring>
#include "unistd.h"
#include "network.hpp"
#include "log.hpp"

#define THREAD_NETWORK       "Thread Network"
#define RESEND_TIMES         5

void network::operator()()
{
    int iResendTimes = 0;
    logSetThreadName(THREAD_NETWORK);
    FunctionNetwork * funcNetwork = new FunctionNetwork();
    SocketNetwork * socketNw = new SocketNetwork();

    int sockfd;
    struct sockaddr_in servaddr, clieaddr;

    socketNw->createSocket(sockfd);
    socketNw->setMem(servaddr, clieaddr);
    socketNw->setServerInfo(servaddr);
    socketNw->bindServ(sockfd, servaddr);

CHECK_RESEND:
    socketNw->receiveMsg(sockfd, clieaddr);

    //Decode
    RRCConnection_t *decodedMsg = (RRCConnection_t *) funcNetwork->decodeObject(&asn_DEF_RRCConnection);

    if (decodedMsg != NULL)
    {
        LOG(INFO, "Message: {ID: %d, Name: %s, Payload: %s}", decodedMsg->id, decodedMsg->name.buf, decodedMsg->payload.buf);

        if((strcmp((char*)decodedMsg->payload.buf, "Request")) != 0)
        {
            iResendTimes += 1;

            if(iResendTimes > RESEND_TIMES)
            {
                iResendTimes = 0;
                bufferNetwork = NULL;

                LOG(INFO, "[... RRC reject message ...]-[Network will disconnect!]");
                rejectMsg();
                socketNw->sendMsg(sockfd, (char *)bufferNetwork, clieaddr);
                sleep(0.5);

                bufferNetwork = NULL;

                LOG(INFO, "------CONNECT NETWORK FAIL!!!------");
            }
            else
            {
                bufferNetwork = NULL;

                LOG(INFO, "[... RRC reject message ...]-[%d]", iResendTimes);
                rejectMsg();
                socketNw->sendMsg(sockfd, (char *)bufferNetwork, clieaddr);
                sleep(0.5);

                bufferNetwork = NULL;
                goto CHECK_RESEND;

            }
        }
        else
        {
            bufferNetwork = NULL;
            ASN_STRUCT_FREE(asn_DEF_RRCConnection, decodedMsg);

            LOG(INFO, "[... RRC setup message ...]");
            setupMsg();
            socketNw->sendMsg(sockfd, (char *)bufferNetwork, clieaddr );
            sleep(0.5);

            bufferNetwork = NULL;
            socketNw->receiveMsg(sockfd, clieaddr);

            RRCConnection_t *decodedMsg = (RRCConnection_t *) funcNetwork->decodeObject(&asn_DEF_RRCConnection);

            if (decodedMsg != NULL)
            {
                LOG(INFO, "Message: {ID: %d, Name: %s, Payload: %s}", decodedMsg->id, decodedMsg->name.buf, decodedMsg->payload.buf);
                LOG(INFO, "------COMPLETE!!!------");
            }

            ASN_STRUCT_FREE(asn_DEF_RRCConnection, decodedMsg);

        }
    }

    free(bufferNetwork);
    bufferNetwork = NULL;
    delete[] funcNetwork;
    funcNetwork = NULL;
    close(sockfd);
    delete[] socketNw;
    socketNw = NULL;
}

void network::setupMsg()
{
    FunctionNetwork * funcNw = new FunctionNetwork();

    RRCConnection_t *Msg = (RRCConnection_t *) calloc(1, sizeof(RRCConnection_t));

    if (Msg == NULL)
    {
      LOG(ERROR, "calloc() failed");
      exit(EXIT_FAILURE);
    }
    //Set ID
    Msg->id = 2;
    //Set Name device
    std::string sMsgId = "Network";
    char *cpMsgId = new char [sizeof(sMsgId)];
    strcpy(cpMsgId,sMsgId.c_str());
    Msg->name.buf = (uint8_t*)cpMsgId;
    Msg->name.size = 7;
    //Set Payload
    std::string sPayload = "Setup";
    char *cpPayload = new char [sizeof(sPayload)]; 
    strcpy(cpPayload,sPayload.c_str());
    Msg->payload.buf = (uint8_t*)cpPayload;
    Msg->payload.size = 5;

    //Encode
    (RRCConnection_t *) funcNw->encodeObject(&asn_DEF_RRCConnection, Msg);

    delete[] funcNw;
    funcNw = NULL;
    ASN_STRUCT_FREE(asn_DEF_RRCConnection, Msg);  
}

void network::rejectMsg()
{
    FunctionNetwork * funcNw = new FunctionNetwork();

    RRCConnection_t *Msg = (RRCConnection_t *) calloc(1, sizeof(RRCConnection_t));

    if (Msg == NULL)
    {
      LOG(ERROR, "calloc() failed");
      exit(EXIT_FAILURE);
    }
    //Set ID
    Msg->id = 2;
    //Set Name device
    std::string sMsgId = "Network";
    char *cpMsgId = new char [sizeof(sMsgId)];
    strcpy(cpMsgId,sMsgId.c_str());
    Msg->name.buf = (uint8_t*)cpMsgId;
    Msg->name.size = 7;
    //Set Payload
    std::string sPayload = "Reject";
    char *cpPayload = new char [sizeof(sPayload)]; 
    strcpy(cpPayload,sPayload.c_str());
    Msg->payload.buf = (uint8_t*)cpPayload;
    Msg->payload.size = 6;

    //Encode
    (RRCConnection_t *) funcNw->encodeObject(&asn_DEF_RRCConnection, Msg);

    delete[] funcNw;
    funcNw = NULL;
    ASN_STRUCT_FREE(asn_DEF_RRCConnection, Msg);  
}