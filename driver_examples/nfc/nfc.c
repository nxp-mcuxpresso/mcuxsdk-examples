/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "app.h"
#include "fsl_nfc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* NDEF Type 2 Tag TLV */
#define NDEFT2T_TLV_NULL        0x00
#define NDEFT2T_TLV_LOCK        0x01
#define NDEFT2T_TLV_NDEF        0x03
#define NDEFT2T_TLV_PROPRIETARY 0xFD
#define NDEFT2T_TLV_TERMINATOR  0xFE

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*
 * NDEF format
 *
 * Bit 7     6       5       4       3       2       1       0
 * ------  ------  ------  ------  ------  ------  ------  ------
 * [ MB ]  [ ME ]  [ CF ]  [ SR ]  [ IL ]  [        TNF         ]
 *
 * [                         TYPE LENGTH                        ]
 *
 * [                       PAYLOAD LENGTH                       ]
 *
 * [                          ID LENGTH                         ]
 *
 * [                         RECORD TYPE                        ]
 *
 * [                              ID                            ]
 *
 * [                           PAYLOAD                          ]
 */
SDK_ALIGN(static const uint8_t s_nfcData[], 4);
static const uint8_t s_nfcData[] = {
    /* Proprietary TLV. */
    NDEFT2T_TLV_PROPRIETARY, 0x02, 0, 0,

    /* Message TLV. */
    NDEFT2T_TLV_NDEF, /* Type: Message */
    0x12,             /* Length: From next byte to the byte before terminator TLV. */
    0xD1,             /* MB: 1, ME: 1, CF: 0, SR: 1, IL: 0, TNF: 3, Absolute URI as defined in RFC 3986. */
    0x01,             /* Type length: 1 */
    0x0E,             /* Payload length: 14, length of "enhello world" */
    'T',              /* Record type: text */
    0x02,             /* 2 byte language code, "en" */
    'e', 'n',         /* Language code: "en" */
    'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', /* Actual body text. */

    /* Terminator TLV. */
    NDEFT2T_TLV_TERMINATOR, 0, 0, 0};

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    /* Init hardware*/
    BOARD_InitHardware();

    NFC_Init(DEMO_NFC);

    NFC_WritePage(DEMO_NFC, 0, (const uint32_t *)s_nfcData, sizeof(s_nfcData) / sizeof(uint32_t));

    while (1)
    {
    }
}
