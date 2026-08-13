
#include <stddef.h>

#include "ipv4_header.h"
#include "assert.h"

#define IPV4_FIELD_TYPE(field) \
    typeof(((ipv4_header_t *)0)->field)

#define IPV4_COMPUTE_MASK(bit_len, bit_offset) \
    (((1u << (bit_len)) - 1) << bit_offset)

#define IPV4_HEADER_PARSE_FIELD(field, data, byte_offset, bit_offset, bit_len) \
    ((*(IPV4_FIELD_TYPE(field) *)((data) + (byte_offset)) & IPV4_COMPUTE_MASK(bit_offset, bit_len)))

typedef struct __attribute__((packed)) ipv4_header_wire 
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

    header->version = IPV4_HEADER_PARSE_FIELD(version, data, offsetof(ipv4_header_wire_t, ver_ihl), VER_BIT_OFFSET, VER_LEN_BITS);

    if (header->version != 4)
    {
        return IPV4_HEADER_ERR_INVALID_VERSION;
    }

    header->ihl = IPV4_HEADER_PARSE_FIELD(ihl, data, offsetof(ipv4_header_wire_t, ver_ihl), IHL_BIT_OFFSET, IHL_LEN_BITS);

    if (header->ihl < 5)
    {
        return IPV4_HEADER_ERR_INVALID_IHL;
    }

    header->dscp = IPV4_HEADER_PARSE_FIELD(dscp, data, offsetof(ipv4_header_wire_t, dscp_ecn), DSCP_BIT_OFFSET, DSCP_LEN_BITS);
    header->ecn = IPV4_HEADER_PARSE_FIELD(ecn, data, offsetof(ipv4_header_wire_t, dscp_ecn), ECN_BIT_OFFSET, ECN_LEN_BITS);
    header->total_length = IPV4_HEADER_PARSE_FIELD(total_length, data, offsetof(ipv4_header_wire_t, len), LEN_BIT_OFFSET, LEN_LEN_BITS);
    if (header->total_length < MIN_IPV4_PACKET_LEN)
    {
        return IPV4_HEADER_ERR_SHORT;
    }

    header->id = IPV4_HEADER_PARSE_FIELD(id, data, offsetof(ipv4_header_wire_t, id), ID_BIT_OFFSET, ID_LEN_BITS);
    header->flags = IPV4_HEADER_PARSE_FIELD(flags, data, offsetof(ipv4_header_wire_t, flags_fragoffset), FLAGS_BIT_OFFSET, FLAGS_LEN_BITS);

    header->fragment_offset = IPV4_HEADER_PARSE_FIELD(fragment_offset, data, offsetof(ipv4_header_wire_t, flags_fragoffset), FRAGOFFSET_BIT_OFFSET, FRAGOFFSET_LEN_BITS);
    
    return IPV4_HEADER_OK;
}
