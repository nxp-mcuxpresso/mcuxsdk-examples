/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_mmdc.h"
#include "app.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*
 * LPDDR2 device read and write
 */
static bool LPDD2_ReadWrite(void);

/*
 * LPDDR2 device init function
 * @param mmdc base address
 * @param bus frequency value
 */
static status_t LPDDR2_Init(MMDC_Type *base, uint32_t busClock_Hz);

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint32_t ddr_writeBuffer[DDR_EXAMPLE_DATALEN]; /* ddr write buffer */
uint32_t ddr_readBuffer[DDR_EXAMPLE_DATALEN];  /* ddr read buffer */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t clockSrc, mr4;

    mmdc_lpddr2_timing_t lpddr2Timing;

    /* Hardware initialize. */
    BOARD_InitHardware();

    /* DDR initialize. */
    clockSrc = BUS_CLK_FREQ;

    PRINTF("\r\n DDR Example Start!\r\n");
    if (LPDDR2_Init(EXAMPLE_MMDC, clockSrc) != kStatus_Success)
    {
        PRINTF("\r\n DDR Init Failed\r\n");
    }

    if (LPDD2_ReadWrite())
    {
        PRINTF("\r\n DDR Write Data and Read Data Succeed.\r\n");
    }
    else
    {
        PRINTF("\r\n DDR Write Data and Read Data Failed.\r\n");
    }

    /* new a timing structure and config it */
    lpddr2Timing.tCKE_Clocks      = CLOCKSNUM(3U);
    lpddr2Timing.tCKSRE_Clocks    = CLOCKSNUM(2U);
    lpddr2Timing.tCKSRX_Clocks    = CLOCKSNUM(2U);
    lpddr2Timing.tCL_Clocks       = CLOCKSNUM(3U);
    lpddr2Timing.tCWL_Clocks      = CLOCKSNUM(2U); /* meet MR2 RL&WL setting*/
    lpddr2Timing.tFAW_Clocks      = CLOCKSNUM(0x0DU);
    lpddr2Timing.tMRD_Clocks      = CLOCKSNUM(1U); /* should set to max(tMRR,tMRW) */
    lpddr2Timing.tRAS_Clocks      = CLOCKSNUM(0x11U);
    lpddr2Timing.tRC_Clocks       = CLOCKSNUM(0U);
    lpddr2Timing.tRCD_Clocks      = CLOCKSNUM(0U);
    lpddr2Timing.tRFC_Clocks      = CLOCKSNUM(0x32U);
    lpddr2Timing.tRP_Clocks       = CLOCKSNUM(0U);
    lpddr2Timing.tRPA_Clocks      = CLOCKSNUM(0U);
    lpddr2Timing.tRRD_Clocks      = CLOCKSNUM(2U);
    lpddr2Timing.tRSTtoCKE_Clocks = CLOCKSNUM(0x10U);
    lpddr2Timing.tRTP_Clocks      = CLOCKSNUM(2U);
    lpddr2Timing.tWR_Clocks       = CLOCKSNUM(1U);
    lpddr2Timing.tWTR_Clocks      = CLOCKSNUM(2U);
    lpddr2Timing.tXP_Clocks       = CLOCKSNUM(1U);
    lpddr2Timing.tXSR_Clocks      = CLOCKSNUM(0x36U);
    lpddr2Timing.ralat_Clocks     = CLOCKSNUM(0U);
    lpddr2Timing.walat_Clocks     = CLOCKSNUM(0U);

    /* change freq to target */
    if (MMDC_SetFrequency(EXAMPLE_MMDC, &lpddr2Timing, DDR_CHANGE_FREQ))
    {
        PRINTF("\r\n LPDDR2 change frequency success.\r\n");
    }
    else
    {
        PRINTF("\r\n LPDDR2 change frequency fail.\r\n");
    }

    if (LPDD2_ReadWrite())
    {
        PRINTF("\r\n DDR Write Data and Read Data Succeed.\r\n");
    }
    else
    {
        PRINTF("\r\n DDR Write Data and Read Data Failed.\r\n");
    }

    MMDC_MonitorLPDDR2OperationTemp(EXAMPLE_MMDC, &mr4);

    if (mr4 == kLPDDR2_TempNotChange)
    {
        PRINTF("\r\n DDR temp not change.\r\n");
    }
    else if (mr4 == kLPDDR2_TempExceedLow)
    {
        PRINTF("\r\n DDR temp exceed low limit.\r\n");
    }
    else if (mr4 == kLPDDR2_TempLow85)
    {
        PRINTF("\r\n DDR temp lower than 85.\r\n");
    }
    else if (mr4 == kLPDDR2_TempHigh85)
    {
        PRINTF("\r\n DDR temp higher than 85.\r\n");
    }
    else if (mr4 == kLPDDR2_TempHigh85Derate)
    {
        PRINTF("\r\n DDR temp higher than 85,need derating.\r\n");
        if (MMDC_LPDDR2UpdateDerate(EXAMPLE_MMDC, NULL, kMMDC_DerateTiming))
        {
            PRINTF("\r\n DDR derating success.\r\n");
        }
        else
        {
            PRINTF("\r\n DDR derating fail.\r\n");
        }
    }
    else if (mr4 == kLPDDR2_TempExceedHigh)
    {
        PRINTF("\r\n DDR temp exceed high.\r\n");
    }

    PRINTF("\r\n DDR Example End.\r\n");

    while (1)
    {
    }
}

static status_t LPDDR2_Init(MMDC_Type *base, uint32_t busClock_Hz)
{
    assert(busClock_Hz <= LPDDR2_DEVICE_MAX_FREQ);

    status_t errorCheck = kStatus_Success;

    mmdc_config_t config;
    mmdc_basic_config_t basicConfig;
    mmdc_auto_refresh_t autoRefresh;
    mmdc_power_config_t pwr;
    mmdc_calibration_config_t cal;
    mmdc_zq_config_t zq;

    /* user can new a global mmdc_lpddr2_timing_t to save the timing when change freq */
    mmdc_lpddr2_timing_t lpddr2Timing;

    /* reset all structure */
    memset(&config, 0U, sizeof(mmdc_config_t));
    memset(&basicConfig, 0U, sizeof(mmdc_basic_config_t));
    memset(&pwr, 0U, sizeof(mmdc_power_config_t));
    memset(&cal, 0U, sizeof(mmdc_calibration_config_t));
    memset(&zq, 0U, sizeof(mmdc_zq_config_t));
    memset(&lpddr2Timing, 0U, sizeof(mmdc_lpddr2_timing_t));

    /* Init the MMDC CONFIG structure
     * pointer to the device init function
     * set the bus clock
     * assign the dev/misc pointer
     */
    config.busFreq_HZ  = busClock_Hz;
    config.basicConfig = &basicConfig;

    /*
     * init the device structure
     */
    basicConfig.deviceFreq_HZ = busClock_Hz;
    basicConfig.deviceSize    = LPDDR2_DEVICE_SIZE;
    basicConfig.devType       = LPDDR2_DEVICE_TYPE;
    basicConfig.devBank       = LPDDR2_DEVICE_BANK;
    basicConfig.rowWidth      = LPDDR2_DEVICE_ROW_WIDTH;
    basicConfig.colWidth      = LPDDR2_DEVICE_COL_WIDTH;
    basicConfig.burstLen      = LPDDR2_DEVICE_BURST_LEN;

    /* auto fresh scheme config */
    autoRefresh.refreshCnt     = LPDDR2_REFRESH_CNT;
    autoRefresh.refreshRate    = LPDDR2_REFRESH_RATE;
    autoRefresh.refreshTrigSrc = LPDDR2_REFRESH_TRIGGER_SRC;

    basicConfig.autoRefresh = &autoRefresh;
    basicConfig.power       = &pwr;
    basicConfig.cal         = &cal;
    basicConfig.zq          = &zq;
    basicConfig.timing      = &lpddr2Timing;

    /* get the default config of timing/ZQ/PWR */
    MMDC_GetDefaultConfig(&config);

    /* init the module */
    errorCheck = MMDC_Init(base, &config);

    cal.rMode = kMMDC_CalWithPreDefine;
    cal.wMode = kMMDC_CalWithPreDefine;
    /* do read/write calibration */
    errorCheck = MMDC_DeviceCalibration(base, &basicConfig);

    return errorCheck;
}

static bool LPDD2_ReadWrite(void)
{
    uint32_t index;
    uint32_t datalen       = DDR_EXAMPLE_DATALEN;
    uint32_t *ddrStartAddr = (uint32_t *)MMDC_DEVICE_START_ADDR; /* DDR start address. */
    bool result            = true;

    /* Prepare data and write to DDR. */
    for (index = 0; index < datalen; index++)
    {
        ddr_writeBuffer[index]              = index;
        *(uint32_t *)(ddrStartAddr + index) = ddr_writeBuffer[index];
    }
    PRINTF("\r\n DDR Write finished!\r\n");

    PRINTF("\r\n DDR Read Start, Start Address 0x%x, Data Length %d !\r\n", ddrStartAddr, datalen);
    /* Read data from the DDR. */
    for (index = 0; index < datalen; index++)
    {
        ddr_readBuffer[index] = *(uint32_t *)(ddrStartAddr + index);
    }
    PRINTF("\r\n DDR Read finished!\r\n");

    PRINTF("\r\n DDR Write Data and Read Data Compare Start!\r\n");

    /* Compare the two buffers. */
    if (memcmp(ddr_readBuffer, ddr_writeBuffer, DDR_EXAMPLE_DATALEN))
    {
        result = false;
        PRINTF("\r\n DDR Read/write fail!\r\n");
    }
    else
    {
        result = true;
        PRINTF("\r\n DDR Read/write pass!\r\n");
    }

    return result;
}
