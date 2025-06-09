/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_qspi_soc.h"
#include "fsl_smc.h"
#include "clock_config.h"
/*${header:end}*/

/*${function:start}*/
static const scg_sys_clk_config_t g_sysClkConfig_BOARD_ClockHSRUN =
    {
        .divSlow = kSCG_SysClkDivBy3, /* Slow Clock Divider: divided by 3 */
        .divBus = kSCG_SysClkDivBy2,  /* Bus Clock Divider: divided by 2 */
        .divCore = kSCG_SysClkDivBy2, /* Core Clock Divider: divided by 2 */
        .src = kSCG_SysClkSrcSysPll,  /* FIRC as System Clock Source */
    };
static const scg_sosc_config_t g_scgSysOscConfig_BOARD_ClockHSRUN =
    {
        .freq = 8000000U,                         /* System Oscillator frequency: 8000000Hz */
        .enableMode = kSCG_SysOscEnable,          /* Enable System OSC clock */
        .monitorMode = kSCG_SysOscMonitorDisable, /* Monitor disabled */
        .div2 = kSCG_AsyncClkDivBy1,              /* System OSC Clock Divider 2: divided by 1 */
        .workMode = kSCG_SysOscModeOscLowPower,   /* Oscillator low power */
    };
static const scg_sirc_config_t g_scgSircConfig_BOARD_ClockHSRUN =
    {
        .enableMode = kSCG_SircEnable | kSCG_SircEnableInLowPower,/* Enable SIRC clock, Enable SIRC in low power mode */
        .div2 = kSCG_AsyncClkDivBy1,              /* Slow IRC Clock Divider 2: divided by 1 */
        .range = kSCG_SircRangeHigh,              /* Slow IRC high range clock (8 MHz) */
    };
const scg_spll_config_t g_scgSpllConfig_BOARD_ClockHSRUN =
    {
        .enableMode = kSCG_SysPllEnable,
        .div1 = kSCG_AsyncClkDivBy1,
        .prediv = 0,
        .mult = 24,
    };

/* Use HSRUN 80 mode, 160MHz SPLL, 80MHz core/sys clock, 40MHz bus clock. */
void BOARD_InitClockHSRUN(void)
{
    scg_sys_clk_config_t curConfig;

    /* Init SOSC according to board configuration. */
    CLOCK_InitSysOsc(&g_scgSysOscConfig_BOARD_ClockHSRUN);
    /* Set the XTAL0 frequency based on board settings. */
    CLOCK_SetXtal0Freq(g_scgSysOscConfig_BOARD_ClockHSRUN.freq);
    /* Set HSRUN power mode. */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    SMC_SetPowerModeHsrun(SMC);
    while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateHsrun)
    {
    }
    /* Init SPLL. */
    CLOCK_InitSysPll(&g_scgSpllConfig_BOARD_ClockHSRUN);
    /* Init SIRC. */
    CLOCK_InitSirc(&g_scgSircConfig_BOARD_ClockHSRUN);
    /* Set SCG to LPFLL mode. */
    CLOCK_SetHsrunModeSysClkConfig(&g_sysClkConfig_BOARD_ClockHSRUN);
    /* Wait for clock source switch finished. */
    do
    {
         CLOCK_GetCurSysClkConfig(&curConfig);
    } while (curConfig.src != g_sysClkConfig_BOARD_ClockHSRUN.src);
    /* Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKHSRUN_CORE_CLOCK;
}

uint32_t lut[FSL_FEATURE_QSPI_LUT_DEPTH] =
{
    /* Seq0 :Quad Read */
    /* CMD:        0x6B - Quad Read, Single pad */
    /* ADDR:       0x18 - 24bit address, Single pads */
    /* DUMMY:      0x08 - 8 clock cyles, Quad pads */
    /* READ:       0x80 - Read 128 bytes, Quad pads */
    [0] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x6B, QSPI_ADDR, QSPI_PAD_1, 0x18),
    [1] = QSPI_LUT_SEQ(QSPI_DUMMY, QSPI_PAD_4, 0x08, QSPI_READ, QSPI_PAD_4, 0x80),

    /* Seq1: Write Enable */
    /* CMD:      0x06 - Write Enable, Single pad */
    [4] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x06, 0, 0, 0),

    /* Seq2: Erase All */
    /* CMD:    0x60 - Erase All chip, Single pad */
    [8] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x60, 0, 0, 0),

    /* Seq3: Read Status */
    /* CMD:    0x05 - Read Status, single pad */
    /* READ:   0x01 - Read 1 byte */
    [12] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x05, QSPI_READ, QSPI_PAD_1, 0x1),

    /* Seq4: Page Program */
    /* CMD:    0x02 - Page Program, Single pad */
    /* ADDR:   0x18 - 24bit address, Quad pad */
    /* WRITE:  0x80 - Write 128 bytes at one pass, Quad pad */
    [16] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x32, QSPI_ADDR, QSPI_PAD_1, 0x18),
    [17] = QSPI_LUT_SEQ(QSPI_WRITE, QSPI_PAD_4, 0x80, 0, 0, 0),

    /* Seq5: Write Register */
    /* CMD:    0x01 - Write Status Register, single pad */
    /* WRITE:  0x01 - Write 1 byte of data, single pad */
    [20] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x01, QSPI_WRITE, QSPI_PAD_1, 0x1),

    /* Seq6: Read Config Register */
    /* CMD:  0x15 - Read Config register, single pad */
    /* READ: 0x01 - Read 1 byte */
    [24] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x15, QSPI_READ, QSPI_PAD_1, 0x1),

    /* Seq7: Erase Sector */
    /* CMD:  0x20 - Sector Erase, single pad */
    /* ADDR: 0x18 - 24 bit address, single pad */
    [28] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0xD8, QSPI_ADDR, QSPI_PAD_1, 0x18),

    /* Seq8: Read ID */
    [32] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x90, QSPI_ADDR, QSPI_PAD_1, 0x18),
    [33] = QSPI_LUT_SEQ(QSPI_READ, QSPI_PAD_1, 0x2, 0, 0, 0),

    /* Match MISRA rule */
    [63] = 0
};

qspi_flash_config_t single_config = {.flashA1Size       = FLASH_SIZE,
                                     .flashA2Size       = 0,
                                     .dataHoldTime      = 0,
                                     .CSHoldTime        = 3,
                                     .CSSetupTime       = 3,
                                     .cloumnspace       = 0,
                                     .endian            = kQSPI_64LittleEndian,
                                     .enableWordAddress = false};

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitClockHSRUN();
    BOARD_InitDebugConsole();
    BOARD_InitFlashPins();
}

void BOARD_QspiSocConfigure(QuadSPI_Type *base)
{
    qspi_soc_config_t config = {0};

    config.inputBufEnable = true;
    config.divEnable      = true;
    config.internalClk    = kQSPI_PllDiv1Clock;
    config.clkMode        = kQSPI_SysClock;
    config.clkDiv         = 8;

    QSPI_Enable(base, false);
    QSPI_SocConfigure(base, &config);
    QSPI_Enable(base, true);
}
/*${function:end}*/
