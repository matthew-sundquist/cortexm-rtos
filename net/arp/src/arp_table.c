
#include "arp_table.h"

#define ARP_TABLE_MAX_SIZE 30

static arp_entry_t *find_entry(ipv4_addr_t *ip);

// initial implementation to just be an array
typedef struct arp_table
{
    arp_entry_t entries[ARP_TABLE_MAX_SIZE];
} arp_table_t;

static arp_table_t arp_table;

void arp_init()
{
    memset(arp_table.entries, 0, sizeof(arp_table.entries));
}

bool arp_lookup(ipv4_addr_t ip, mac_addr_t *mac)
{
    
}

static arp_entry_t *find_entry(ipv4_addr_t *ip)
{
}
