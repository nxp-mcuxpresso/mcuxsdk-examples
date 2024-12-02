/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_spm.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
static void BOARD_InitDebugConsoleWithSirc(void)
{
    scg_sirc_config_t scgSircConfigStruct = {
        .enableMode =
            kSCG_SircEnable | kSCG_SircEnableInLowPower, /* Enable SIRC clock, Enable SIRC in low power mode */
        .div1  = kSCG_AsyncClkDisable,                   /* Slow IRC Clock Divider 1: Clock output is disabled */
        .div2  = kSCG_AsyncClkDivBy1,                    /* Slow IRC Clock Divider 2: Clock output is disabled */
        .div3  = kSCG_AsyncClkDivBy1,                    /* Slow IRC Clock Divider 3: divided by 1 */
        .range = kSCG_SircRangeHigh,                     /* Slow IRC high range clock (8 MHz) */
    };
    uint32_t uartClkSrcFreq;

    /* Init SIRC with DIV2 enabled for LPUART in low power mode. */
    CLOCK_InitSirc(&scgSircConfigStruct);

    /* Set PCC LPUART0 selection */
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSircAsync);
    uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

static void SetExternalFlashDPMode(void)
{
    uint32_t srcClock_Hz;
    lpspi_master_config_t masterConfig = {0};

    CLOCK_SetIpSrc(kCLOCK_Lpspi1, kCLOCK_IpSrcFircAsync);
    CLOCK_EnableClock(kCLOCK_Lpspi1);

    /*LPSPI config*/
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    srcClock_Hz = FLASH_SPI_MASTER_CLK_FREQ;
    LPSPI_MasterInit(FLASH_SPI_MASTER, &masterConfig, srcClock_Hz);

    /*Send command of entering deep power-down*/
    LPSPI_WriteData(FLASH_SPI_MASTER, DPCommand);

    CLOCK_DisableClock(kCLOCK_Lpspi1);
}

void BOARD_InitHardware(void)
{
    /* Clear the IO lock when reset from VLLSx low power mode. */
    if (SPM_GetPeriphIOIsolationFlag(SPM))
    {
        SPM_ClearPeriphIOIsolationFlag(SPM);
    }
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    /* BOARD_InitDebugConsole(); */
    BOARD_InitDebugConsoleWithSirc();
    /* Set external flash enter deep power-down */
    SetExternalFlashDPMode();
}
/*${function:end}*/
