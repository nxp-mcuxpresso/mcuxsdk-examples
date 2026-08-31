/* -------------------------------------------------------------------------- */
/*                           Copyright 2026 NXP                               */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "board.h"
#include "pin_mux.h"
#include "fwk_board_lp.h"
#include "fwk_platform_lowpower.h"
#include "fsl_pm_core.h"
#include "fwk_debug.h"
#include "fsl_usart.h"
#include "fsl_debug_console.h"
#include "fsl_flexspi.h"
#include "fsl_rtc.h"
#ifdef CONFIG_BT_SETTINGS
#ifdef EDGEFAST_BT_LITTLEFS_MFLASH
#include "mflash_common.h"
#include "mflash_drv.h"
#else
#include "fsl_adapter_flash.h"
#endif
#endif

/* -------------------------------------------------------------------------- */
/*                            Private definitions                             */
/* -------------------------------------------------------------------------- */

/* LUT sequence index used to put flash into deep power-down (OPI). */
#define NOR_CMD_LUT_SEQ_IDX_SLEEP_OPI 11
/* LUT sequence index used to wake flash from deep power-down (OPI). */
#define NOR_CMD_LUT_SEQ_IDX_WKUP_OPI  12

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Callback registered to SDK Power Manager to get notified of entry/exit of low power modes
 *
 * \param[in] eventType event specifying if we entered or exited from low power mode
 * \param[in] powerState low power mode used during low power period
 * \param[in] data Optional data passed when the callback got registered (not used currently)
 * \return status_t
 */
static status_t BOARD_LowpowerCallback(pm_event_type_t eventType, uint8_t powerState, void *data);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

/*
 * Save 2 debug UART pins (USART0/FC0): PIO[0][1] = FC0_TXD, PIO[0][2] = FC0_RXD.
 * Update the pin indices below if your board uses a different IOPCTL mapping.
 */
static uint32_t            s_DebugUart[2];
static uint32_t            s_ISPPin[3];

static pm_notify_element_t boardLpNotifyGroup = {
    .notifyCallback = BOARD_LowpowerCallback,
    .data           = NULL,
};

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void BOARD_LowPowerInit(void)
{
    status_t status;

    status = PM_RegisterNotify(kPM_NotifyGroup2, &boardLpNotifyGroup);
    assert(status == kStatus_Success);
    (void)status;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static void BOARD_FlashOperate(FLEXSPI_Type *base, uint32_t baseAddr, uint8_t index)
{
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = kFLEXSPI_PortB1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = index;
    FLEXSPI_TransferBlocking(base, &flashXfer);
}

static void BOARD_FlashEnterDPMode(FLEXSPI_Type *base, uint32_t baseAddr)
{
    /* After calling this function all code/data accessed must reside in RAM
     * until BOARD_FlashExitDPMode() is called. */
    BOARD_FlashOperate(base, baseAddr, NOR_CMD_LUT_SEQ_IDX_SLEEP_OPI);
    for (uint32_t i = 0; i < 500; i++)
    {
        __NOP();
    }
}

static void BOARD_FlashExitDPMode(FLEXSPI_Type *base, uint32_t baseAddr)
{
    BOARD_FlashOperate(base, baseAddr, NOR_CMD_LUT_SEQ_IDX_WKUP_OPI);
    for (uint32_t i = 0; i < 1500; i++)
    {
        __NOP();
    }
}

static void BOARD_SetLowPowerMEM(void)
{
#if defined(CONFIG_BT_SETTINGS) && (CONFIG_BT_SETTINGS > 0)
    BOARD_FlashEnterDPMode(FLEXSPI, 0);
#endif
}

static void BOARD_RestoreLowPowerMEM(void)
{
#if defined(CONFIG_BT_SETTINGS) && (CONFIG_BT_SETTINGS > 0)
    BOARD_FlashExitDPMode(FLEXSPI, 0);
#endif
}

static void BOARD_SetLowPowerISPPins(void)
{
    /* ISP/Boot Mode pins: PIO[1][15], PIO[1][16], PIO[1][17].
     * Disable pull-ups and input buffers to avoid current leakage during deep sleep.
     * Reference: board.c BOARD_SetDeepSleepPinConfig(). */
    s_ISPPin[0]        = IOPCTL->PIO[1][15];
    s_ISPPin[1]        = IOPCTL->PIO[1][16];
    s_ISPPin[2]        = IOPCTL->PIO[1][17];
    IOPCTL->PIO[1][15] = 0;
    IOPCTL->PIO[1][16] = 0;
    IOPCTL->PIO[1][17] = 0;
}

static void BOARD_RestoreLowPowerISPPins(void)
{
    IOPCTL->PIO[1][15] = s_ISPPin[0];
    IOPCTL->PIO[1][16] = s_ISPPin[1];
    IOPCTL->PIO[1][17] = s_ISPPin[2];
}

static void BOARD_SetLowPowerDebugUartPins(void)
{
    /* Wait for debug console output to finish. */
    while (((uint32_t)kUSART_TxIdleFlag & USART_GetStatusFlags(USART0)) == 0U)
    {
    }

    /* Save and de-assert USART0 (FLEXCOMM0) debug UART pins:
     * PIO[0][1] = FC0_TXD_SCL_MISO_WS, PIO[0][2] = FC0_RXD_SDA_MOSI_DATA. */
    s_DebugUart[0]    = IOPCTL->PIO[0][1];
    s_DebugUart[1]    = IOPCTL->PIO[0][2];
    IOPCTL->PIO[0][1] = 0;
    IOPCTL->PIO[0][2] = 0;
}

static void BOARD_RestoreLowPowerDebugUartPins(void)
{
    IOPCTL->PIO[0][1] = s_DebugUart[0];
    IOPCTL->PIO[0][2] = s_DebugUart[1];
}

static void BOARD_EnterDeepSleepCb(void)
{
    BOARD_SetLowPowerDebugUartPins();
    BOARD_SetLowPowerISPPins();
    BOARD_SetLowPowerMEM();
}

static void BOARD_ExitDeepSleepCb(void)
{
    BOARD_RestoreLowPowerMEM();
    BOARD_RestoreLowPowerISPPins();
    BOARD_RestoreLowPowerDebugUartPins();
}

/* -------------------------------------------------------------------------- */
/*                              Low Power callbacks                           */
/* -------------------------------------------------------------------------- */

static status_t BOARD_LowpowerCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    status_t ret = kStatus_Success;

    if (powerState >= PLATFORM_DEEP_SLEEP_STATE)
    {
        if (eventType == kPM_EventEnteringSleep)
        {
            BOARD_EnterDeepSleepCb();
        }
        else
        {
            BOARD_ExitDeepSleepCb();
        }
    }
    else
    {
        /* Nothing to do when entering WFI or Sleep low power state. */
    }

    return ret;
}
