#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h"
#include "stm32f4x7_eth_bsp.h"

// Enable LCD.
#define USE_LCD         1

// Enable DHCP, if disabled static address defined below is used.
#define USE_DHCP        1
 
// MAC address.
#define MAC_ADDR0       0xDE
#define MAC_ADDR1       0xAD
#define MAC_ADDR2       0xBE
#define MAC_ADDR3       0xEF
#define MAC_ADDR4       0x12
#define MAC_ADDR5       0x34
 
// Static IP address.
#define IP_ADDR0        192
#define IP_ADDR1        168
#define IP_ADDR2        0
#define IP_ADDR3        65
   
// Gateway address.
#define GW_ADDR0        192
#define GW_ADDR1        168
#define GW_ADDR2        0
#define GW_ADDR3        1  

// Netmask.
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

// MII and RMII mode selection.
#define RMII_MODE

#endif /* __MAIN_H */
