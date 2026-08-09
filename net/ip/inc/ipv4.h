#ifndef IPV4_H
#define IPV4_H

#include <stdbool.h>

#include "arp_table.h" // defines ipv4_addr_t for now, should move here eventually

bool ipv4_addr_local(ipv4_addr_t ip);

#endif
