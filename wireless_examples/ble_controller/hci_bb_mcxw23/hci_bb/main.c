/*
 * Copyright 2020-2025 NXP
 * NXP Confidential and Proprietary.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "app.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "ble_controller.h"
#include "fsl_iap_ffr.h"
#include "hci_transport.h"
#include "hci_uart.h"
#include "hci_custom_cmds.h"
#include "ll_error.h"

#ifdef OSA_USED
#include "fsl_os_abstraction.h"
#endif /*OSA_USED*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NVIC_PRIO_BASE (0u)

#ifndef MAX_TX_POWER_DBM
#define MAX_TX_POWER_DBM (0u)
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Static functions
 ******************************************************************************/

/*******************************************************************************
 * Public functions
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    flash_config_t flashInstance;
    uint8_t bdAddr[6];
    status_t status;
    uint32_t location;

    BOARD_InitHardware();

    /* Make sure link layer interrupts have highest priority */
    for (int irq = 0; irq <= WAKE_PAD_IRQn; irq++)
    {
        if (irq == BLE_LL_IRQn || irq == BLE_SLP_TMR_IRQn)
        {
            /* Link layer IRQs need highest priority */
            NVIC_SetPriority(irq, NVIC_PRIO_BASE + 2);
        }
        else
        {
            /* Medium priority for all other interrupts.*/
            NVIC_SetPriority(irq, NVIC_PRIO_BASE + 3);
        }
    }

    /* Init LL*/
    blec_result_t ret = BLEController_Init(NULL, MAX_TX_POWER_DBM, NULL);
    if (ret != kBLEC_Success)
    {
        return ret;
    }
    hci_transport_init();

    ret = BLEController_SetConnectionInitialTxPowerDbm(MAX_TX_POWER_DBM);
    if (ret != kBLEC_Success)
    {
        return ret;
    }

    ret = BLEController_SetTxPowerDbm(MAX_TX_POWER_DBM);
    if (ret != kBLEC_Success)
    {
        return ret;
    }

    /* Set BLE address*/
    status = FLASH_Init(&flashInstance);
    if (status != kStatus_Success)
    {
        return status;
    }

    location = kFFR_BdAddrLocationCmpa | kFFR_BdAddrLocationNmpa | kFFR_BdAddrLocationUuid;
    status   = FFR_GetBdAddress(&flashInstance, bdAddr, &location);
    if (status != kStatus_Success)
    {
        return status;
    }

    ret = BLEController_WriteBdAddr(bdAddr);
    if (ret != kBLEC_Success)
    {
        return status;
    }

    /* Register ble_ext_custm_cb callback */
    hci_rgstr_ble_external_custom_cbk(HCICUSTOMCMD_Callback);

    /* Start listening for HCI commands */
    while (1)
    {
        /* Handle link layer events */
        BLEController_EmngrHandleAllEvents();

#ifdef OSA_USED
        /* Process all tasks */
        OSA_ProcessTasks();
#endif /*OSA_USED*/
    }
    return 0;
}
