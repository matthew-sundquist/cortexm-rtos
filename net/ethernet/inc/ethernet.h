
#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include <stdbool.h>

#include "netbuf.h"
#include "arp_table.h"

bool ethernet_get_mac(mac_addr_t *mac);

bool ethernet_send_packet(netbuf_t *nbuf);

// funcs defined in frame.c

#endif
