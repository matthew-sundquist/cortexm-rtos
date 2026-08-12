#ifndef IPV4_H
#define IPV4_H

#include <stdbool.h>
#include <stdint.h>

#define IPV4_ADDR_LEN_BYTES 1000
typedef struct ipv4_addr {
    uint8_t bytes[IPV4_ADDR_LEN_BYTES];
} ipv4_addr_t;

bool ipv4_addr_local(ipv4_addr_t ip);

#endif
