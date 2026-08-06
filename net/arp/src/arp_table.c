
#include <string.h>

#include "arp_table.h"
#include "assert.h"
#include "scheduler.h"

#define ARP_TABLE_MAX_SIZE 30

#define ARP_CACHE_TIMEOUT_MS (60 * 1000) // hopefully replace 1000 with macro in future

static arp_entry_t *find_entry(ipv4_addr_t ip);

static inline bool entry_expired(const arp_entry_t *entry);

// initial implementation to just be an array
typedef struct arp_table
{
    arp_entry_t entries[ARP_TABLE_MAX_SIZE];
} arp_table_t;

static arp_table_t arp_table;

void arp_table_init()
{
    memset(arp_table.entries, 0, sizeof(arp_table.entries));
}

bool arp_table_lookup(ipv4_addr_t ip, mac_addr_t *mac)
{
    ASSERT(mac != NULL);

    arp_entry_t *entry = find_entry(ip);

    if (!entry)
    {
        return false;
    }

    if (entry_expired(entry))
    {
        return false;
    }

    memcpy(mac, entry->mac.bytes, MAC_ADDR_LEN_BYTES);

    return true;
}

static arp_entry_t *find_entry(ipv4_addr_t ip)
{
    for (int i = 0; i < ARP_TABLE_MAX_SIZE; i++)
    {
        if ((uint32_t)arp_table.entries[i].ip.bytes == (uint32_t)ip.bytes)
        {
            return &arp_table.entries[i];
        }
    }

    return NULL;
}

static inline bool entry_expired(const arp_entry_t *entry)
{
    ASSERT(entry != NULL);

    if (get_ticks() - entry->last_used_tick > ARP_CACHE_TIMEOUT_MS)
    {
        return true;
    }
    
    return false;
}

