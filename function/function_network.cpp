#include <string>
#include <mutex>
#include "function_network.hpp"
#include "log.hpp"

#define FUNC_NETWORK                "Function Network"

void * bufferNetwork;

mutex m_funcNw;

void * FunctionNetwork::encodeObject(asn_TYPE_descriptor_t *type_descriptor, void *struct_ptr)
{
    logSetThreadName(FUNC_NETWORK);
    asn_per_constraints_s *constraints = NULL;

    m_funcNw.lock();
    const ssize_t ec = uper_encode_to_new_buffer(type_descriptor, constraints, struct_ptr, &bufferNetwork);
    m_funcNw.unlock();

    if (ec == -1)
    {
        LOG(ERROR, "Encode Network failed");
    }
    else 
    {
        LOG(INFO, "Encode Network succeeded with Message size(Byte): %d", ec);
    }

    this->ec = ec;
}

void * FunctionNetwork::decodeObject(asn_TYPE_descriptor_t *type_descriptor)
{
    logSetThreadName(FUNC_NETWORK);
    void *decoded_object = 0;

    m_funcNw.lock();
    const asn_dec_rval_t rval = uper_decode(0, type_descriptor, &decoded_object, bufferNetwork, strlen((char*)bufferNetwork), 0, 0);
    m_funcNw.unlock();

    if (rval.code != RC_OK)
    {
        LOG(ERROR, "Decode Network failed");
    } 
    else
    {
        LOG(INFO, "Decode Network succeeded with Message size(Byte): %d", strlen((char*)bufferNetwork));
        return decoded_object;
    }

    return NULL;
}