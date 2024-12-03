/* -------------------------------------------------------------------------- */
/*                           Copyright 2020-2022 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "board.h"
#include "pin_mux.h"
#include "board_lp.h"
#include "fwk_platform_lowpower.h"
#include "fsl_pm_core.h"
#include "fwk_debug.h"
#include "fsl_usart.h"
#include "fsl_debug_console.h"
#include "clock_config.h"

#ifdef CONFIG_BT_SETTINGS
#include "mflash_drv.h"
#endif

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * @brief Basic lowpower entry call to flush serial transaction and disable peripherals pin to avoid leakage in lowpower
 *    typically called from wakeup from deep sleep
 */
static void BOARD_EnterLowPower(void);

/*!
 * @brief Basic lowpower exit callback to reinitialize clock and pin mux configuration,
 *    typically called from wakeup from deep sleep and other lowest power mode
 */
static void BOARD_ExitLowPower(void);

/*!
 * \brief This function called after exiting Power Down mode on main domain. It should be used to
 *        restore peripherals in main domain used by the application that need specific restore
 *        procedure such as LPUART1, etc..
 * \note  Peripherals in wakeup domain are not concerned. Wakeup domain always remains in Deep Sleep, Sleep,
 *        or Active mode so the HW registers are always retained.
 *
 */
static void BOARD_ExitPowerDown(void);

/*!
 * @brief Configures debug console for low power entry
 *
 */
static void BOARD_UninitDebugConsole(void);

/*!
 * \brief Reinitializes the debug console after low power exit
 *
 */
static void BOARD_ReinitDebugConsole(void);

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

static pm_notify_element_t boardLpNotifyGroup = {
    .notifyCallback = &BOARD_LowpowerCallback,
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

static void BOARD_EnterLowPower(void)
{
    /* Configure debug console for low power entry */
    BOARD_UninitDebugConsole();
}

static void BOARD_ExitLowPower(void)
{
    /* Reinitialize debug console */
    BOARD_ReinitDebugConsole();
}

static void BOARD_ExitPowerDown(void)
{
    /* Clocks and pins need to be reinitialized after wake up from power down */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

#ifdef CONFIG_BT_SETTINGS
    /* Reinit mflash driver after exit from power down mode */
    mflash_drv_init();
#endif
}

static void BOARD_UninitDebugConsole(void)
{
    /* Wait for debug console output finished. */
    while (((uint32_t)kUSART_TxFifoEmptyFlag & USART_GetStatusFlags(BOARD_DEBUG_UART)) == 0U)
    {
    }
    (void)DbgConsole_EnterLowpower();
}

static void BOARD_ReinitDebugConsole(void)
{
    CLOCK_SetFRGClock(BOARD_DEBUG_UART_FRG_CLK);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    (void)DbgConsole_ExitLowpower();
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
            BOARD_EnterLowPower();
        }
        else
        {
            if (powerState >= PLATFORM_POWER_DOWN_STATE)
            {
                BOARD_ExitPowerDown();
            }

            BOARD_ExitLowPower();
        }
    }
    else
    {
        /* Nothing to do when entering WFI or Sleep low power state
         * NVIC fully functional to trigger upcoming interrupts */
    }

    return ret;
}
