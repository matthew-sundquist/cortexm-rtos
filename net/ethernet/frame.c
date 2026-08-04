
#include <string.h>

#include "ethernet.h"

#define SFD ((uint8_t) 0b10101011)

#define SFD_OFFSET_BYTES 0
#define SFD_SIZE_BYTES 1

#define DST_MAC_OFFSET_BYTES (SFD_OFFSET + SFD_SIZE_BYTES)
#define DST_MAC_SIZE_BYTES 6

#define SRC_MAC_OFFSET_BYTES (DST_MAC_OFFSET_BYTES + DST_MAC_SIZE_BYTES)
#define SRC_MAC_SIZE_BYTES 6

#define LENGTH_OFFSET_BYTES (SRC_MAC_OFFSET_BYTES + SRC_MAC_SIZE_BYTES)
#define LENGTH_SIZE_BYTES 2

static inline void add_sfd(uint8_t *hdr, const uint8_t sfd);

static inline void add_dst(uint8_t *hdr, const uint8_t *dst);

static inline void add_src(uint8_t *hdr, const uint8_t *src);

bool ethernet_add_header(const ethernet_header_t *eth_header, netbuf_t *nb)
{
    if (!eth_header || !nb)
    {
        return false;
    }

    // cant copy the data from eth_header directly into the netbuf because
    // of potential padding inside the struct adding unexpected bytes
    // because of this, we need to reconstruct the data that needs to be added into
    // a truly contigous data structure
    
    uint8_t e_hdr[15];

    add_sfd(e_hdr, SFD);
    add_dst(e_hdr, eth_header->dst_mac);
    add_src(e_hdr, eth_header->src_mac);

    // todo: implement getting and adding length to header
    //       implement somehow adding the crc behind the data field
    return true;
}

static inline void add_sfd(uint8_t *hdr, const uint8_t sfd)
{
    hdr[SFD_OFFSET_BYTES] = sfd;
}

static inline void add_dst(uint8_t *hdr, const uint8_t *dst)
{
    memcpy(hdr + DST_MAC_OFFSET_BYTES, dst, DST_MAC_SIZE_BYTES);
}

static inline void add_src(uint8_t *hdr, const uint8_t *src)
{
    memcpy(hdr + SRC_MAC_OFFSET_BYTES, src, SRC_MAC_SIZE_BYTES);
}

