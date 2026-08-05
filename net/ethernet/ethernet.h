
#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include <stdbool.h>

#include "netbuf.h"

// USING ETHERNET II FRAME FORMATTING

typedef struct ethernet_header {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype;
} ethernet_header_t;

// funcs defined in frame.c

#endif
