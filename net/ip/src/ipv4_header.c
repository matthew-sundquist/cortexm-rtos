
#include <stddef.h>

#include "ipv4_header.h"
#include "assert.h"

typedef struct __attribute__(aligned(4)) ipv4_header_wire 
{
    uint8_t ver_ihl;
    uint8_t dscp_ecn;
    uint16_t len;
    uint16_t id;
    uint16_t flags_fragoffset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t src_addr;
    uint32_t dst_addr;
} ipv4_header_wire_t;

#define VER_BIT_OFFSET 0
#define VER_LEN_BITS 4

#define IHL_BIT_OFFSET VER_LEN_BITS
#define IHL_LEN_BITS 4

#define DSCP_BIT_OFFSET 0
#define DSCP_LEN_BITS 6

#define ECN_BIT_OFFSET DSCP_LEN_BITS
#define ECN_LEN_BITS 2

#define LEN_BIT_OFFSET 0
#define LEN_LEN_BITS 16

#define ID_BIT_OFFSET 0
#define ID_LEN_BITS 16

#define FLAGS_BIT_OFFSET 0
#define FLAGS_LEN_BITS 3

#define FRAGOFFSET_BIT_OFFSET FLAGS_LEN_BITS
#define FRAGOFFSET_LEN_BITS 13

#define TTL_BIT_OFFSET 0
#define TTL_LEN_BITS 8

#define PROTOCOL_BIT_OFFSET 0
#define PROTOCOL_LEN_BITS 8

#define CHECKSUM_BIT_OFFSET 0
#define CHECKSUM_LEN_BITS 16

#define SRC_ADDR_BIT_OFFSET 0
#define SRC_ADDR_LEN_BITS 32

#define DST_ADDR_BIT_OFFSET 0
#define DST_ADDR_LEN_BITS 32

#define MIN_IPV4_PACKET_LEN 20
#define MAX_IPV4_PACKET_LEN 65535

#define IPV4_HEADER_FLAG_RSVD (1 << 0)
#define IPV4_HEADER_FLAG_DF (1 << 1)
#define IPV4_HEADER_FLAG_MF (1 << 2)

ipv4_header_status_t ipv4_parse_header(const uint8_t *data, size_t len, ipv4_header_t *header)
{
    ASSERT(header != NULL);
    ASSERT(data != NULL);
    ASSERT(len > 0);

    // subsequent accesses from this struct should be safe due to this struct being aligned to a 4 byte boundry
    // should be no risk of unaligned accesses
    ipv4_header_wire_t *wire_header = (ipv4_header_wire_t *) data;

    header->version = wire_header->ver_ihl & 0x0F;
    
    if (version != 4)
    {
        return IPV4_HEADER_ERR_INVALID_VERSION;
    }

    header->ihl = (wire_header->ver_ihl & 0xF0) >> 4;

    if (header->ihl < 5)
    {
        return IPV4_HEADER_ERR_INVALID_IHL;
    }

    header->dscp = wire_header->dscp_ecn >> 2;

    header->ecn = wire_header->dscp_ecn & 0x03;

    header->total_length = wire_header->len;

    if (header->total_length < MIN_IPV4_PACKET_LEN)
    {
        return IPV4_HEADER_ERR_SHORT;
    }

    header->id = wire_header->id;

    header->flags = wire_header->flags_fragoffset >> 13;
    
    header->fragment_offset = wire_header->flags_fragoffset & ~0xE000;

    return IPV4_HEADER_OK;
}
