#include <string>
#include <mutex>
#include "function_ue.hpp"
#include "log.hpp"

#define FUNC_UE                "Function UE"

void * bufferUe;

mutex m_funcUe;

void * FunctionUe::encodeObject(asn_TYPE_descriptor_t *type_descriptor, void *struct_ptr)
{
    logSetThreadName(FUNC_UE);
    asn_per_constraints_s *constraints = NULL;

    m_funcUe.lock();
    const ssize_t ec = uper_encode_to_new_buffer(type_descriptor, constraints, struct_ptr, &bufferUe);
    m_funcUe.unlock();

    if (ec == -1)
    {
        LOG(ERROR, "Encode UE failed");
    }
    else 
    {
        LOG(INFO, "Encode UE succeeded with Message size(Byte): %d", ec);
    }

    this->ec = ec;
}

void * FunctionUe::decodeObject(asn_TYPE_descriptor_t *type_descriptor)
{
    logSetThreadName(FUNC_UE);
    void *decoded_object = 0;

    m_funcUe.lock();
    const asn_dec_rval_t rval = uper_decode(0, type_descriptor, &decoded_object, bufferUe, strlen((char*)bufferUe), 0, 0);
    m_funcUe.unlock();

    if (rval.code != RC_OK)
    {
        LOG(ERROR, "Decode UE failed");
    } 
    else
    {
        LOG(INFO, "Decode UE succeeded with Message size(Byte): %d", strlen((char*)bufferUe));
        return decoded_object;
    }

    return NULL;
}