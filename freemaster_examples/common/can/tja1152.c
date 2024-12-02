/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "fsl_rgpio.h"
#include "freemaster.h"
#include "freemaster_can.h"

////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////

#ifndef FMSTR_CAN_RESPID
#error FMSTR CAN response message ID not defined!
#endif

// CAN message standard identifier for automatic bit rate detection
#define EXAMPLE_BIT_RATE_DETECTION_ID 0x555

// The default identifier of a vanilla TJA1152
#define EXAMPLE_TJA1152_ID            0x18DA00F1

// GPIO4_IO29 connect to STB pin of TJA1152
#define EXAMPLE_STB_RGPIO             RGPIO4
#define EXAMPLE_STB_RGPIO_PIN         (29U)

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief Initialize and configure TJA1152 CAN transceiver
 */
void TJA1152_Init(void)
{
    // In Vanilla state, it enters Configuration mode after autodetecting
    // the baud rate. It then waits for configuration commands to be loaded locally viat he TXD pin (if STB = HIGH)
    RGPIO_PortSet(EXAMPLE_STB_RGPIO, 1u << EXAMPLE_STB_RGPIO_PIN);

    SDK_DelayAtLeastUs(100U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    // Automatic bit rate detection
    FMSTR_CAN_DRV.Init(0, EXAMPLE_BIT_RATE_DETECTION_ID);

    while(!FMSTR_CAN_DRV.PrepareTxFrame());
    FMSTR_CAN_DRV.SendTxFrame(1);

    // Configuration of spoofing protection, add FMSTR CAN ID to Transmission Whitelist
    FMSTR_CAN_DRV.Init(0, FMSTR_CAN_EXTID | EXAMPLE_TJA1152_ID);

    FMSTR_U8 byte0 = 0x10; //command to set the filter element
    FMSTR_U8 byte1 = 0x00; //filter element index

    // Message bytes initialization
    FMSTR_U8 byte2 = 0x00;
    FMSTR_U8 byte3 = 0x00;
    FMSTR_U8 byte4 = 0x00;
    FMSTR_U8 byte5 = 0x00;

    // Check FMSTR CAN ID format (standard or extended)
    if ((FMSTR_CAN_RESPID & FMSTR_CAN_EXTID) != 0UL)
    {
        byte2 = 0x80;                                                  // filter element type - extended ID
        byte2 |= 0b01000000;                                           // transmission passlist enabled
        byte2 |= (FMSTR_U8)((FMSTR_U32)FMSTR_CAN_RESPID >> 24) & 0x1F; // highest 5 bits of SFID1
        byte3 |= (FMSTR_U8)((FMSTR_U32)FMSTR_CAN_RESPID >> 16);        // next 8 bits of SFID1
        byte4 |= (FMSTR_U8)((FMSTR_U32)FMSTR_CAN_RESPID >> 8);         // next 8 bits of SFID1
        byte5 |= (FMSTR_U8)FMSTR_CAN_RESPID;                           // lowest 8 bits of SFID1
    }
    else
    {
        byte2 = 0x00;                                                  // filter element type - standard ID
        byte2 |= 0b00100000;                                           // dual filter IDs: SFID1 and SFID2
        byte2 |= 0b00010000;                                           // SFID1 to the transmission passlist
        byte2 |= 0b00000000;                                           // do not include SFID1 in the bus blocklist
        byte2 |= (FMSTR_U8)((FMSTR_U16)FMSTR_CAN_RESPID >> 8) & 0x7;   // highest 3 bits of SFID1
        byte3 = (FMSTR_U8)FMSTR_CAN_RESPID;                            // lowest 8 bits of SFID1
        byte4 = 0x00;                                                  // SFID2 not used
        byte5 = 0x00;                                                  // SFID2 not used
    }

    FMSTR_CAN_DRV.PutTxFrameByte(0, byte0);
    FMSTR_CAN_DRV.PutTxFrameByte(1, byte1);
    FMSTR_CAN_DRV.PutTxFrameByte(2, byte2);
    FMSTR_CAN_DRV.PutTxFrameByte(3, byte3);
    FMSTR_CAN_DRV.PutTxFrameByte(4, byte4);
    FMSTR_CAN_DRV.PutTxFrameByte(5, byte5);

    while(!FMSTR_CAN_DRV.PrepareTxFrame());
    FMSTR_CAN_DRV.SendTxFrame(6);

    // Set reconfiguration accepted only locally, keep CONFIG_ID as default value 0x18DA00F1
    FMSTR_CAN_DRV.PutTxFrameByte(0, 0x60); // reconfiguration  command ID
    FMSTR_CAN_DRV.PutTxFrameByte(1, 0x98); // config enable timer running (reconfiguration is not disabled by the command)
    FMSTR_CAN_DRV.PutTxFrameByte(2, 0xDA);
    FMSTR_CAN_DRV.PutTxFrameByte(3, 0x00);
    FMSTR_CAN_DRV.PutTxFrameByte(4, 0xF1);

    while(!FMSTR_CAN_DRV.PrepareTxFrame());
    FMSTR_CAN_DRV.SendTxFrame(5);

    // Turn off flooding protection
    FMSTR_CAN_DRV.PutTxFrameByte(0, 0x40); // tampering and flooding protection command ID
    FMSTR_CAN_DRV.PutTxFrameByte(1, 0x00); // keep default values
    FMSTR_CAN_DRV.PutTxFrameByte(2, 0x03); // flooding prevention turned off

    while(!FMSTR_CAN_DRV.PrepareTxFrame());
    FMSTR_CAN_DRV.SendTxFrame(3);

    // Leaving configuration mode
    FMSTR_CAN_DRV.PutTxFrameByte(0, 0x71); // configuration written into volatile memory only
    FMSTR_CAN_DRV.PutTxFrameByte(1, 0x02); // config enable timer running (reconfiguration is not disabled by the command)
    FMSTR_CAN_DRV.PutTxFrameByte(2, 0x03);
    FMSTR_CAN_DRV.PutTxFrameByte(3, 0x04);
    FMSTR_CAN_DRV.PutTxFrameByte(4, 0x05);
    FMSTR_CAN_DRV.PutTxFrameByte(5, 0x06);
    FMSTR_CAN_DRV.PutTxFrameByte(6, 0x07);
    FMSTR_CAN_DRV.PutTxFrameByte(7, 0x08);

    while(!FMSTR_CAN_DRV.PrepareTxFrame());
    FMSTR_CAN_DRV.SendTxFrame(8);

    while(!FMSTR_CAN_DRV.PrepareTxFrame());

    //STB=L, TJA1152 switch from secure standby mode to normal mode
    RGPIO_PortClear(EXAMPLE_STB_RGPIO, 1u << EXAMPLE_STB_RGPIO_PIN);
}