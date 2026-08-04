
#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include <stdbool.h>

#include "netbuf.h"

typedef struct ethernet_header {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
} ethernet_header_t;

// funcs defined in frame.c

bool ethernet_add_header(const ethernet_header_t *eth_header, netbuf_t *nb);

#endif
