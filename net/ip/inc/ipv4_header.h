#ifndef IPV4_HEADER_H
#define IPV4_HEADER_H

#include <stdint.h>

#include "ipv4.h"

typedef enum {
    ICMP = 1,
    TCP = 6,
    UDP = 17
} ipv4_protocol_t;

typedef enum {
    IPV4_HEADER_OK,
    IPV4_HEADER_ERR_SHORT,
    IPV4_HEADER_ERR_INVALID_CHECKSUM,
    IPV4_HEADER_ERR_INVALID_VERSION,
    IPV4_HEADER_ERR_INVALID_IHL
} ipv4_header_status_t;

typedef struct ipv4_header
{
    uint8_t version;
    uint8_t ihl;
    uint8_t dscp;
    uint8_t ecn;
    uint16_t total_length;
    uint16_t identification;
    uint8_t flags;
    uint16_t fragment_offset;
    uint8_t ttl;
    ipv4_protocol_t protocol;
    uint16_t checksum;
    ipv4_addr_t src_addr;
    ipv4_addr_t dst_addr;
} ipv4_header_t;

ipv4_header_status_t ipv4_parse_header(const uint8_t *data, size_t len, ipv4_header_t *header);

#endif
