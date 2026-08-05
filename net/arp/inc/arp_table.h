
#ifndef ARP_TABLE_H
#define ARP_TABLE_H

#include <stdint.h>

typedef enum arp_state 
{
    ARP_NONE,
    ARP_VALID,
    ARP_PENDING
} arp_state_t;


typedef struct arp_entry
{
    uint32_t ip;
    uint8_t mac[6];

    uint32_t last_used_tick;

    arp_state_t state;
} arp_entry_t;

#endif
