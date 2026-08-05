
#ifndef ARP_TABLE_H
#define ARP_TABLE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum arp_state 
{
    ARP_NONE,
    ARP_VALID,
    ARP_PENDING
} arp_state_t;

typedef struct mac_addr
{
    uint8_t bytes[6];
} mac_addr_t;

typedef struct ipv4_addr
{
    uint8_t bytes[4];
} ipv4_addr_t;

typedef struct arp_entry
{
    ipv4_addr_t ip;
    mac_addr_t mac;

    uint32_t last_used_tick;

    arp_state_t state;
} arp_entry_t;

// arp table implemented as opaque object

bool arp_lookup(ipv4_addr_t ip, mac_addr_t *mac);


#endif
