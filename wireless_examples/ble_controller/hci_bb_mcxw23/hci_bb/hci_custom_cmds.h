/**
 * Copyright 2024,2025 NXP.
 * NXP Confidential and Proprietary.
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef _CUSTOMHCICMDS_H_
#define _CUSTOMHCICMDS_H_

#include "hci.h"

#define HCI_OGF_EXT           0x30
#define HCI_OGF_SHIFT         (10)
#define GET_OP_CODE(OCF, OGF) ((uint16_t)OCF | (OGF << HCI_OGF_SHIFT))

/*! @brief Function pointer type for custom operations. */
typedef ble_stat_t (*custom_operation_func_t)(uint8_t nbOfInParams,
                                              uint8_t *inParams,
                                              uint8_t *evntPcktP,
                                              uint8_t *paramsLength,
                                              hci_return_command_type *returnCommandType);

/*! @brief Structure representing a custom HCI command. */
typedef struct
{
    uint16_t opcode;
    uint8_t nbOfInParams;
    custom_operation_func_t func;
} CustomHciCmd;

/*! @brief Enumeration of custom HCI commands. */
typedef enum
{
    HCI_OCF_DCDC_BUCK_CONFIG      = 0x00, /**< Configure DC-DC buck mode. */
    HCI_OCF_START_CW_TX           = 0x01, /**< Start continuous wave transmission. */
    HCI_OCF_START_CW_RX           = 0x02, /**< Start continuous wave reception. */
    HCI_OCF_STOP_CW               = 0x03, /**< Stop continuous wave operation. */
    HCI_OCF_READ_CW_RSSI          = 0x04, /**< Read RSSI during continuous wave operation. */
    HCI_OCF_START_CW_TX_MODULATED = 0x05, /**< Start modulated continuous wave transmission. */
    HCI_OCF_SET_SUPPLY_MODE       = 0x08, /**< Set supply mode for the device. */
    HCI_OCF_PRINT_STRING          = 0x1D  /**< Print a custom string. */
} hci_custom_commands_t;

/*!
 * @brief Register custom command list.
 * param cmdList                   Pointer to command list
 * param *cmdCount                 Amount of items in the list
 * return 0 in case of success
 */
int HCICUSTOMCMD_Register(const CustomHciCmd *cmdList, int cmdCount);

/*!
 * @brief External custom callback for HCI commands that are unknown to link layer.
 * param ocf                     Ocf received within HCI command.
 * param *pckt_p                 Pointer to received packet.
 * param *evnt_pckt_p            Pointer to event packet.
 * param *params_length          Pointer to parameters total length.
 * param *return_command_type    Command to be returned within the HCI event packet.
 * return status
 */
ble_stat_t HCICUSTOMCMD_Callback(uint16_t ocf,
                                 uint8_t *pcktPtr,
                                 uint8_t *evntPcktPtr,
                                 uint8_t *paramsLength,
                                 hci_return_command_type *returnCommandType);
#endif /* _CUSTOMHCICMDS_H_ */
