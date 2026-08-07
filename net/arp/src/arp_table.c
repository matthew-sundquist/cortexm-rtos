
#include <string.h>

#include "arp_table.h"
#include "assert.h"
#include "scheduler.h"

#define ARP_TABLE_MAX_SIZE 30

#define ARP_CACHE_TIMEOUT_MS (60 * 1000) // hopefully replace 1000 with macro in future

static arp_entry_t *find_entry(ipv4_addr_t ip);

static arp_entry_t *insert_entry(ipv4_addr_t ip);

static inline void reset_entry(arp_entry_t *entry);

// initial implementation to just be an array
typedef struct arp_table
{
    arp_entry_t entries[ARP_TABLE_MAX_SIZE];
} arp_table_t;

static arp_table_t arp_table;

void arp_table_init()
{
    for (int i = 0; i < ARP_TABLE_MAX_SIZE; i++)
    {
        reset_entry(&arp_table.entries[i]);
    }
}

arp_entry_t *arp_table_lookup(ipv4_addr_t ip)
{
    return find_entry(ip);
}

arp_entry_t *arp_table_insert(ipv4_addr_t ip)
{
    return insert_entry(ip);
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

static arp_entry_t *insert_entry(ipv4_addr_t ip)
{
    arp_entry_t *oldest_entry = &arp_table.entries[0];
    for (int i = 0; i < ARP_TABLE_MAX_SIZE; i++)
    {
        ipv4_addr_t arp_entry_ip = arp_table.entries[i].ip;
        if (!(uint32_t) arp_entry_ip.bytes)
        {
            arp_table.entries[i].ip = ip;
            return &arp_table.entries[i];
        }

        if ((uint32_t) arp_entry_ip.bytes == (uint32_t)ip.bytes)
        {
            return &arp_table.entries[i];
        }

        if ((uint32_t)arp_entry_ip.bytes == 0 && arp_table.entries[i].last_used_tick < oldest_entry->last_used_tick)
        {
            oldest_entry = &arp_table.entries[i];
        }
    }

    reset_entry(oldest_entry);

    oldest_entry->ip = ip;

    return oldest_entry;
}

static inline void reset_entry(arp_entry_t *entry)
{
    memset(entry, 0, sizeof(arp_entry_t));
}

