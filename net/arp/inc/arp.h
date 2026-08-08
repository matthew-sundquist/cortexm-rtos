
#ifndef ARP_H
#define ARP_H

#include <stdbool.h>
#include <stdint.h>


#include "arp_table.h"

typedef struct arp_header {
    uint16_t hrd; // hardware address space
    uint16_t pro; // protocol address space
    uint8_t hln; // byte length of each hardware addr
    uint8_t pln; // byte length of each protocol addr
    uint16_t opcode; // req or reply
    uint8_t *sha; // hardware addr of sender of this packet (length of hln)
    uint8_t *spa;
    uint8_t *tha;
    uint8_t *tpa;
} arp_header_t;

/* responsible for checking arp table, and in the case of failure, 
 * sending the arp broadcast*/
bool arp_resolve_mac(ipv4_addr_t ip, mac_addr_t *mac);

#endif
