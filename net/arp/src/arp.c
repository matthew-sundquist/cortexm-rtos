
#include <string.h>
#include "arp.h"
#include "assert.h"
#include "scheduler.h"

#define ARP_EXPIRY_TIME_TICKS (1000*60) // 1000 ticks/s for 60s

static void arp_send_request();

static inline bool mac_expired(const arp_entry_t *entry);

bool arp_resolve_mac(ipv4_addr_t ip, mac_addr_t *mac)
{
    ASSERT(mac != NULL);

    arp_entry_t *entry = arp_table_lookup(ip);

    if (!entry) // ip not found in table
    {
        entry = arp_table_insert(ip);
    }

    if (entry->state != ARP_VALID || (entry->state == ARP_VALID&& mac_expired(entry)))
    {
        arp_send_request();
        entry->state = ARP_PENDING;

        return false;
    }

    memcpy(mac->bytes, entry->mac.bytes, MAC_ADDR_LEN_BYTES);

    return true;
}

static inline bool mac_expired(const arp_entry_t *entry)
{
    ASSERT(entry != NULL);

    return (get_ticks() > entry->last_used_tick + ARP_EXPIRY_TIME_TICKS);
}

static void arp_send_request() // cant implement til ethernet interface is done
{
    ;
}
