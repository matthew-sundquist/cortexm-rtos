
#include <arpa/inet.h>
#include <string.h>

#include "arp.h"
#include "frame.h"
#include "assert.h"
#include "scheduler.h"

#define ARP_EXPIRY_TIME_TICKS (1000*60) // 1000 ticks/s for 60s

#define HARDWARE_ADDR_SPACE_ETHERNET 1

typedef enum {
    REQUEST = 1,
    REPLY = 2
} arp_opcode_t;

typedef struct __attribute__((packed)) arp_header {
    uint16_t hrd; // hardware address space
    uint16_t pro; // protocol address space
    uint8_t hln; // byte length of each hardware addr
    uint8_t pln; // byte length of each protocol addr
    uint16_t opcode; // req or reply
    uint8_t sha[MAC_ADDR_LEN_BYTES]; // hardware addr of sender of this packet (length of hln)
    uint8_t spa[IPV4_ADDR_LEN_BYTES];
    uint8_t tha[MAC_ADDR_LEN_BYTES];
    uint8_t tpa[IPV4_ADDR_LEN_BYTES];
} arp_header_t;

static arp_header_t arp_build_header(const uint8_t *sha, const uint8_t *spa, const uint8_t *tha, const uint8_t *tpa);

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

// assumes ipv4
static arp_header_t arp_build_header(const uint8_t *sha, const uint8_t *spa, const uint8_t *tha, const uint8_t *tpa)
{
    ASSERT(sha != NULL);
    ASSERT(spa != NULL);
    ASSERT(tha != NULL);
    ASSERT(tpa != NULL);
    arp_header_t ret = (arp_header_t) {
        .hrd = htons(HARDWARE_ADDR_SPACE_ETHERNET),
        .pro = htons(ETHERTYPE_IPV4),
        .hln = 6,
        .pln = 4,
        .opcode = htons(REQUEST),
    };

    memcpy(ret.sha, sha, ret.hln);
    memcpy(ret.spa, spa, ret.pln);
    memcpy(ret.tha, tha, ret.hln);
    memcpy(ret.tpa, tpa, ret.pln);

    return ret;
}
