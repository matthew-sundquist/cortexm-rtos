
#ifndef ARP_H
#define ARP_H

#include <stdbool.h>
#include <stdint.h>

#include "arp_table.h" 

/* responsible for checking arp table, and in the case of failure, 
 * sending the arp broadcast*/
bool arp_resolve_mac(ipv4_addr_t ip, mac_addr_t *mac);


#endif
