
#ifndef ARP_TABLE_H
#define ARP_TABLE_H

#include <stdint.h>
#include <stdbool.h>

#define MAC_ADDR_LEN_BYTES 6
#define IPV4_ADDR_LEN_BYTES 4

typedef enum arp_state 
{
    ARP_NONE,
    ARP_VALID,
    ARP_PENDING
} arp_state_t;

typedef struct mac_addr
{
    uint8_t bytes[MAC_ADDR_LEN_BYTES];
} mac_addr_t;

typedef struct ipv4_addr
{
    uint8_t bytes[IPV4_ADDR_LEN_BYTES];
} ipv4_addr_t;

typedef struct arp_entry
{
    ipv4_addr_t ip;
    mac_addr_t mac;

    uint32_t last_used_tick;

    arp_state_t state;
} arp_entry_t;

// arp table implemented as opaque object

void arp_table_init();

bool arp_table_lookup(ipv4_addr_t ip, mac_addr_t *mac);


#endif
