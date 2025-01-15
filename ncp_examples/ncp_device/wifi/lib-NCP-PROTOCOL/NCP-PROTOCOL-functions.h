/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef _NCP_PROTOCOL_FUNCTIONS_H_
#define _NCP_PROTOCOL_FUNCTIONS_H_

// clang-format off
#ifndef NCP_LWIP_SOCKET
#  include <pthread.h>
#endif
// clang-format on

#include <NCP-DUMP-functions.h>

#include <ncp_adapter.h>


/* -------------------------------------------------------------------------- */
/*                                 Types                                      */
/* -------------------------------------------------------------------------- */

/*NCP Bridge command header*/
typedef struct bridge_command_header
{
    uint16_t cmd_ID;      // ID of cmd/resp/event
    //uint16_t classes_MT;  // high 4b for class ; 8b for subclass ; low 4b for msg_type
                            // MT=message type: 0=cmd ; 1=cmd+resp ; 2=event ; 3=data (!)
    uint8_t  subcl_MT;    // low 4b for subclass ; 4b for msg_type
    uint8_t  classes;     // 4b for class + high 4b for subclass
    uint16_t size;     // size of the TLV message including CRC32
    uint16_t seqnum;   // sequence number for cmd/resp/event
    uint16_t result;   // result of command being executed
    uint16_t reserved; // !!!
} NCP_TLV_COMMAND;

/* -------------------------------------------------------------------------- */
/*                               Variables                                    */
/* -------------------------------------------------------------------------- */

#ifndef NCP_LWIP_SOCKET
extern pthread_mutex_t wlan_ncp_RX_mutex;
#endif

/* verbosity counter */
extern int NCP_PROTOCOL_dump;

/* -------------------------------------------------------------------------- */
/*                              Constants                                     */
/* -------------------------------------------------------------------------- */

// clang-format off

// DEFINE: known values of ncp_adapter header fields
#define NCP_TLV_HDR_LENGTH sizeof(NCP_TLV_COMMAND)

#define NCP_TLV_HDR_CLASS_WLAN   (uint8_t)0x00    // class=Wi-Fi
#define index_NCP_TLV_HDR_CLASS_WLAN      (uint8_t)(NCP_TLV_HDR_CLASS_WLAN >> 4)
#define NCP_TLV_HDR_CLASS_BLE    (uint8_t)0x10    // class=BT/BLE
#define index_NCP_TLV_HDR_CLASS_BLE       (uint8_t)(NCP_TLV_HDR_CLASS_BLE  >> 4)
#define NCP_TLV_HDR_CLASS_WPAN   (uint8_t)0x20    // class=15.4/OT
#define index_NCP_TLV_HDR_CLASS_WPAN      (uint8_t)(NCP_TLV_HDR_CLASS_WPAN >> 4)

#define NCP_TLV_HDR_SUBCLASS_SOCKET (uint8_t)0x90 // subclass=SOCKET
#define NCP_TLV_HDR_SUBCLASS_INET   (uint8_t)0xC0 // subclass=INET

#define NCP_TLV_HDR_TYPE_CMD     (uint8_t)0x00    // msg_type=cmd only
#define NCP_TLV_HDR_TYPE_CMD_RSP (uint8_t)0x01    // msg_type=cmd+resp
#define NCP_TLV_HDR_TYPE_EVENT   (uint8_t)0x02    // msg_type=event
#define NCP_TLV_HDR_TYPE_DATA    (uint8_t)0x03    // msg_type=data


/* -------------------------------------------------------------------------- */
/*                                 Macros                                     */
/* -------------------------------------------------------------------------- */

// MACRO: Prepare header of a NCP TX buffer
#define M_NCP_PREPARE_WLAN_TLV_HEADER(pHdr, ID)                         \
    pHdr->cmd_ID   = ID;                                                \
    pHdr->subcl_MT = NCP_TLV_HDR_SUBCLASS_INET | NCP_TLV_HDR_TYPE_CMD_RSP; \
    pHdr->classes  = NCP_TLV_HDR_CLASS_WLAN;                            \
    pHdr->size     = 0xDEAD;                                            \
    pHdr->seqnum   = 0x0000;                                            \
    pHdr->result   = 0x0000;                                            \
    pHdr->reserved = 0;

// clang-format on


#endif // _NCP_PROTOCOL_FUNCTIONS_H_
