
#include "ethernet.h"
#include "assert.h"

static mac_addr_t mymac; // change name

static bool mac_is_valid(mac_addr_t mac);

bool ethernet_get_mac(mac_addr_t *mac)
{
    ASSERT(mac != NULL);

    if (!mac_is_valid(mymac))
    {
        return false;
    }

    *mac = mymac;

    return true;
}

bool ethernet_send_packet(netbuf_t *nbuf)
{
    return true; // placeholder implementation
}

static bool mac_is_valid(mac_addr_t mac)
{
    return true; // change this later or maybe remove completely
}
