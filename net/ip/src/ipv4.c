
#include "ipv4.h"

static ipv4_addr_t local_ip;

bool ipv4_addr_local(ipv4_addr_t ip)
{
    return (uint32_t) ip.bytes == (uint32_t)local_ip.bytes;
}
