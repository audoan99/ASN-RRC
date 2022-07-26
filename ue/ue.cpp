#include <string>
#include <cstring>
#include "unistd.h"
#include "ue.hpp"
#include "log.hpp"

#define THREAD_UE       "Thread Ue"
#define RESEND_TIMES    5

void ue::operator()()
{
    int iResendTimes = 0;
    logSetThreadName(THREAD_UE);
    FunctionUe * funcUE = new FunctionUe();
    SocketUE * socketUe = new SocketUE();

    int sockfd;
    struct sockaddr_in servaddr;
    
    socketUe->createSocket(sockfd);
    socketUe->setMem(servaddr);
    socketUe->setServerInfo(servaddr);

    LOG(INFO, "[... RRC request message ...]");

RESEND:
    requestMsg();

    socketUe->sendMsg(sockfd, (char *)bufferUe, servaddr);

    sleep(0.5);

    bufferUe = NULL;

    socketUe->receiveMsg(sockfd,servaddr);

    RRCConnection_t *decodedMsg = (RRCConnection_t *) funcUE->decodeObject(&asn_DEF_RRCConnection);

    if (decodedMsg != NULL)
    {
        LOG(INFO, "Message: {ID: %d, Name: %s, Payload: %s}", decodedMsg->id, decodedMsg->name.buf, decodedMsg->payload.buf);

        if((strcmp((char*)decodedMsg->payload.buf, "Reject")) == 0)
        {
            iResendTimes += 1;

            if(iResendTimes > RESEND_TIMES)
            {
                iResendTimes = 0;
                LOG(INFO, "------CONNECT UE FAIL!!!------");
            }
            else
            {
                bufferUe = NULL;
                LOG(INFO, "[... Resend RRC request message ...]-[%d]", iResendTimes);
                goto RESEND;

            }
        }
        else
        {
            bufferUe = NULL;
            
            LOG(INFO, "[... RRC complete message ...]");
            completeMsg();
            socketUe->sendMsg(sockfd, (char *)bufferUe, servaddr);
            sleep(0.5);
        }
    }

    ASN_STRUCT_FREE(asn_DEF_RRCConnection, decodedMsg);
    free(bufferUe);
    bufferUe = NULL;
    delete[] funcUE;
    funcUE = NULL;
    delete[] socketUe;
    socketUe = NULL;

}

void ue::requestMsg()
{
    FunctionUe * funcUe = new FunctionUe();

    RRCConnection_t *Msg = (RRCConnection_t *) calloc(1, sizeof(RRCConnection_t));

    if (Msg == NULL)
    {
      LOG(ERROR, "calloc() failed");
      exit(EXIT_FAILURE);
    }
    //Set ID
    Msg->id = 1;
    //Set Name device
    std::string sMsgId = "Ue";
    char *cpMsgId = new char [sizeof(sMsgId)];
    strcpy(cpMsgId,sMsgId.c_str());
    Msg->name.buf = (uint8_t*)cpMsgId;
    Msg->name.size = 2;
    //Set Payload
    std::string sPayload = "Request";
    char *cpPayload = new char [sizeof(sPayload)]; 
    strcpy(cpPayload,sPayload.c_str());
    Msg->payload.buf = (uint8_t*)cpPayload;
    Msg->payload.size = 7;

    //Encode
    (RRCConnection_t *) funcUe->encodeObject(&asn_DEF_RRCConnection, Msg);

    delete[] funcUe;
    funcUe = NULL;
    ASN_STRUCT_FREE(asn_DEF_RRCConnection, Msg);  
}

void ue::completeMsg()
{
    FunctionUe * funcUe = new FunctionUe();
    RRCConnection_t *Msg = (RRCConnection_t *) calloc(1, sizeof(RRCConnection_t));

    if (Msg == NULL)
    {
        LOG(ERROR,"calloc( failed");
        exit(EXIT_FAILURE);
    }
    //Set ID
    Msg->id = 3;
    //Set Name device
    std::string sMsgId = "Ue";
    char *cpMsgId = new char [sizeof(sMsgId)];
    strcpy(cpMsgId,sMsgId.c_str());
    Msg->name.buf = (uint8_t*)cpMsgId;
    Msg->name.size = 2;
    //Set Payload
    std::string sPayload = "Complete";
    char *cpPayload = new char [sizeof(sPayload)];
    strcpy(cpPayload,sPayload.c_str());
    Msg->payload.buf = (uint8_t*)cpPayload;
    Msg->payload.size = 8;

    //Encode
    (RRCConnection_t *) funcUe->encodeObject(&asn_DEF_RRCConnection,Msg);

    delete[] funcUe;
    funcUe = NULL;
    ASN_STRUCT_FREE(asn_DEF_RRCConnection,Msg);
}