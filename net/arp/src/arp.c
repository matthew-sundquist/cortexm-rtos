
#include <arpa/inet.h>
#include <string.h>
#include <stddef.h>

#include "arp.h"
#include "frame.h"
#include "assert.h"
#include "scheduler.h"
#include "ethernet.h"
#include "ipv4.h"

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

static void arp_make_reply(arp_header_t *header);

static void arp_parse_header(const uint8_t *bytes, arp_header_t *header);

static void arp_update_mac(mac_addr_t mac, arp_entry_t *entry);

static bool arp_send_request();

static inline bool mac_expired(const arp_entry_t *entry);

bool arp_recv_packet(netbuf_t *nbuf)
{
    ASSERT(nbuf !=  NULL);

    uint8_t data[sizeof(arp_header_t)];

    if (!netbuf_pop_front(nbuf, data, sizeof(arp_header_t)))
    {
        return false;
    }

    arp_header_t header;

    arp_parse_header(data, &header);

    if (ntohs(header.hrd) != HARDWARE_ADDR_SPACE_ETHERNET)
    {
        return false;
    }
    
    if (ntohs(header.pro) != ETHERTYPE_IPV4)
    {
        return false;
    }

    bool merge_flag = false;

    arp_entry_t *table_entry;

    if ((table_entry = arp_table_lookup(*(ipv4_addr_t *)header.spa)) != NULL)
    {
        merge_flag = true;

        arp_update_mac(*(mac_addr_t *)header.sha, table_entry);
    }

    if (!ipv4_addr_local(*(ipv4_addr_t *)header.tpa))
    {
        return false;
    }

    if (!merge_flag)
    {
        arp_entry_t *new_entry = arp_table_insert(*(ipv4_addr_t *)header.spa);

        arp_update_mac(*(mac_addr_t*)header.sha, new_entry);
    }

    if ((arp_opcode_t)ntohs(header.opcode) != REQUEST)
    {
        return false;
    }

    arp_make_reply(&header);

    ASSERT(netbuf_len(nbuf) == 0); //ensure that this is empty (should be at this point)
                               // consider replacing this with something like netbuf_clear

    if (!netbuf_push_front(nbuf, (uint8_t *)&header, sizeof(header)))
    {
        // this should not be able to fail
        ASSERT(0);
    }

    ethernet_send_packet(nbuf);

    return true;
}

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

static bool arp_send_request() // cant implement til ethernet interface is done
{
    netbuf_t *nbuf = netbuf_alloc();

    if (!nbuf)
    {
        return false;
    }

    return true;
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
        .opcode = htons((uint16_t)REQUEST),
    };

    memcpy(ret.sha, sha, ret.hln);
    memcpy(ret.spa, spa, ret.pln);
    memcpy(ret.tha, tha, ret.hln);
    memcpy(ret.tpa, tpa, ret.pln);

    return ret;
}

static void arp_parse_header(const uint8_t *bytes, arp_header_t *header)
{
    ASSERT(bytes != NULL);
    ASSERT(header != NULL);

    memcpy(&header->hrd, bytes + offsetof(arp_header_t, hrd), sizeof(header->hrd));

    memcpy(&header->pro, bytes + offsetof(arp_header_t, pro), sizeof(header->pro));

    memcpy(&header->hln, bytes + offsetof(arp_header_t, hln), sizeof(header->hln));

    memcpy(&header->pln, bytes + offsetof(arp_header_t, pln), sizeof(header->pln));

    memcpy(&header->opcode, bytes + offsetof(arp_header_t, opcode), sizeof(header->opcode));

    memcpy(&header->sha, bytes + offsetof(arp_header_t, sha), sizeof(header->sha));
    
    memcpy(&header->spa, bytes + offsetof(arp_header_t, spa), sizeof(header->spa));

    memcpy(&header->tha, bytes + offsetof(arp_header_t, tha), sizeof(header->tha));

    memcpy(&header->tpa, bytes + offsetof(arp_header_t, tpa), sizeof(header->tpa));
}


static void arp_update_mac(mac_addr_t mac, arp_entry_t *entry)
{
    ASSERT(entry != NULL);
    entry->mac = mac;
    entry->last_used_tick = get_ticks();
    entry->state = ARP_VALID;
}

static void arp_make_reply(arp_header_t *header)
{
    ASSERT(header != NULL);

    uint8_t tmp_ha[sizeof(header->sha)];
    uint8_t tmp_pa[sizeof(header->spa)];

    memcpy(tmp_ha, header->sha, sizeof(header->sha));
    memcpy(tmp_pa, header->spa, sizeof(header->spa));

    memcpy(header->sha, header->tha, sizeof(header->sha));
    memcpy(header->spa, header->tpa, sizeof(header->spa));

    memcpy(header->tha, tmp_ha, sizeof(header->tha));
    memcpy(header->tpa, tmp_pa, sizeof(header->tpa));

    header->opcode = htons((uint16_t) REPLY);
}
