
#include <stddef.h>

#include "ipv4_header.h"

#define IPV4_FIELD_TYPE(field) \
    typeof(((ipv4_header_t *)0)->field)

#define IPV4_COMPUTE_MASK(bit_len, bit_offset) \
    (((1u << (bit_len)) - 1) << bit_offset)

#define IPV4_HEADER_PARSE_FIELD(field, data, byte_offset, bit_offset, bit_len) \
    ((*(IPV4_FIELD_TYPE *)((data) + (byte_offset)) & IPV4_COMPUTE_MASK(bit_offset, bit_len)))

typedef struct __attribute__((packed)) ipv4_header_wire 
{
    uint8_t ver_ihl;
    uint8_t dscp_ecn;
    uint16_t total_len;
    uint16_t id;
    uint16_t flags_fragoffset;
    uint16_t ttl_protocol;
    uint32_t src_addr;
    uint32_t dst_addr;
} ipv4_header_wire_t;

#define VER_BIT_OFFSET 0
#define VER_LEN_BITS 4

#define IHL_BIT_OFFSET (8 - VER_LEN_BITS)
#define IHL_LEN_BITS 4

#define DSCP_OFFSET_BYTES offsetof(ipv4_header_wire_t, dscp_ecn)
#define DSCP_BIT_OFFSET 0
#define DSCP_LEN_BITS 6



static inline IPV4_FIELD_TYPE(version) parse_version(const uint8_t *data);

static inline IPV4_FIELD_TYPE(ihl) parse_ihl(const uint8_t *data);

ipv4_header_status_t ipv4_parse_header(const uint8_t *data, size_t len, ipv4_header_t *header)
{

}

static inline IPV4_FIELD_TYPE(version) parse_version(const uint8_t *data)
{
    return *(data + offsetof(ipv4_header_wire_t, ver_ihl)) >> VER_LEN_BITS;
}
