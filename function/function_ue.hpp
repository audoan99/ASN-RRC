#include "RRCConnection.h"

class FunctionUe
{
public:
    void *encodeObject(asn_TYPE_descriptor_t *type_descriptor, void *struct_ptr);
    void *decodeObject(asn_TYPE_descriptor_t *type_descriptor);

private:
    ssize_t ec;

};

extern void * bufferUe;
