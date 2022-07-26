/*
*   Author: Team Dev C++ - 5G Team - HCL VN
*   Date: 23/12/2021
*   
*   Describe: 
*   Build a program with 2 different threads:
*   - Thread A: Build RRC request message(ASN) --> Encode RRC message -> send to thread B via UDP protocol.
*   - Thread B: Decode RRC request message --> Check payload: If the payload is correct, send RRC setup message. If the payload is incorrect, send RRC reject message.
*   - Thread A: Decode RRC message --> Check payload: If the payload is "Reject", resend RRC request message. If the payload is "Setup", send RRC complete message. 
*   IF: Thread B sends RRC reject message 5 times ==> B still send RRC setup message wrong ==> Connect Fail!
*    
*    Note: RRC messages need to be built in UDP format where:
*   - First 4 bytes are Message ID
*   - Payload with the size of 20 bytes can be in any format but it has to be received and decoded successfully with no missing info.
*
*   Command line:
*   - Buid: make build
*   - Run: make run
*   - Only delete log files: make clean_log
*   - Delete all log files, .o files and run file: make clean
*
*/
#include <iostream>
#include <thread>
#include "unistd.h"
#include "network.hpp"
#include "ue.hpp"
#include "log.hpp"

#define ASN_LOG         "asn.log"

int main()
{   
    logInit(ASN_LOG);
    std::thread t_Network ((network()));
    sleep(0.5);
    std::thread t_Ue((ue()));
    t_Network.join();
    t_Ue.join();

    return 0;
}