/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${standard_header:start}*/
#include <stdbool.h>
/*${standard_header:end}*/
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_power.h"
#include "usbd_rom_api.h"
#include "fsl_ctimer.h"
#include "fsl_fro_calib.h"
/*${header:end}*/
/*${macro:start}*/
#define CTIMER CTIMER0
/*${macro:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    POWER_DisablePD(kPDRUNCFG_PD_USB0_PHY); /*< Turn on USB Phy */
    CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 1, false);
    CLOCK_AttachClk(kFRO_HF_to_USB0_CLK);
    /* enable usb0 host clock */
    CLOCK_EnableClock(kCLOCK_Usbhsl0);
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    *((uint32_t *)(USBFSH_BASE + 0x5C)) |= USBFSH_PORTMODE_DEV_ENABLE_MASK;
    /* disable usb0 host clock */
    CLOCK_DisableClock(kCLOCK_Usbhsl0);
    /* enable USB IP clock */
    CLOCK_EnableUsbfs0DeviceClock(kCLOCK_UsbSrcFro, CLOCK_GetFreq(kCLOCK_FroHf));
}
void HW_TimerInit(void)
{
    uint32_t timerFreq;
    ctimer_config_t config;
    /* Enable the asynchronous bridge */
    SYSCON->ASYNCAPBCTRL = 1;
    /* Use main clock for ctimer3/4 */
    CLOCK_AttachClk(kMAIN_CLK_to_ASYNC_APB);

    /* Initialize ctimer */
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER, &config);
    /* Get ctimer clock frequency */
    if ((CTIMER == CTIMER0) || (CTIMER == CTIMER1) || (CTIMER == CTIMER2))
    {
        timerFreq = CLOCK_GetFreq(kCLOCK_BusClk);
    }
    else
    {
        timerFreq = CLOCK_GetAsyncApbClkFreq();
    }

    /* Return the version of the FRO Calibration library */
    if (fro_calib_Get_Lib_Ver() == 0)
    {
        while (1U)
            ;
    }

    /* pass ctimer instance & ctimer clock frquency in KHz */
    Chip_TIMER_Instance_Freq(CTIMER, timerFreq / 1000);
}
/*${function:end}*/
