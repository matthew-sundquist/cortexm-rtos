
#ifndef FRAME_H
#define FRAME_H

#include "netbuf.h"

typedef enum {
    ETHERTYPE_ARP = 0x0806,
    ETHERTYPE_IPV4 = 0x0800
} ethertype_t;

typedef struct ethernet_header {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    ethertype_t ethertype;
} ethernet_header_t;

// funcs defined in frame.c

bool ethernet_add_header(const ethernet_header_t *eth_header, netbuf_t *nb);

bool ethernet_parse_header(netbuf_t *nb, ethernet_header_t *eth_header);


#endif
