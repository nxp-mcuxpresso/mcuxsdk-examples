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
 *  device read and write
 */
static bool Device_ReadWrite(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint32_t ddr_writeBuffer[DDR_EXAMPLE_DATALEN]; /* ddr write buffer */
uint32_t ddr_readBuffer[DDR_EXAMPLE_DATALEN];  /* ddr read buffer */
mmdc_device_timing_t timing = {
    .tCKE_Clocks      = 3U,
    .tCKSRE_Clocks    = 2U,
    .tCKSRX_Clocks    = 2U,
    .tRFC_Clocks      = 40U,
    .tXSR_Clocks      = 43U,
    .tXP_Clocks       = 3U,
    .tFAW_Clocks      = 16U,
    .tCL_Clocks       = 3U,
    .tMRD_Clocks      = 3U,
    .tRAS_Clocks      = 13U,
    .tWR_Clocks       = 5U,
    .tRTP_Clocks      = 3U,
    .tWTR_Clocks      = 4U,
    .tRRD_Clocks      = 4U,
    .ralat_Clocks     = 5U,
    .walat_Clocks     = 0U,
    .tRC_Clocks       = 19U,
    .tRCD_Clocks      = 6U,
    .tRP_Clocks       = 6U,
    .tRPA_Clocks      = 7U,
    .tCWL_Clocks      = 1U,
    .tRSTtoCKE_Clocks = 14U,
    .tXPR_Clocks      = 159U,
    .tRTWSAME_Clocks  = 2U,
    .tWTRDIFF_Clocks  = 3U,
    .tWTWDIFF_Clocks  = 3U,
    .tRTWDIFF_Clocks  = 2U,
    .tRTRDIFF_Clocks  = 2U,
    .tDAI_Clocks      = 3001U,
};

mmdc_fine_tuning_config_t tuning = {
    .caDelay       = 0x47U,
    .rDQOffset0    = 0x33333333U,
    .rDQOffset1    = 0x33333333U,
    .wDQOffset0    = 0xf3333333U,
    .wDQOffset1    = 0xf3333333U,
    .rDQDuty0      = kMMDC_DutyHighPercent50,
    .rDQDuty1      = kMMDC_DutyHighPercent50,
    .ddrCKDutyCtl0 = kMMDC_DutyHighPercent50,
    .ddrCKDutyCtl1 = kMMDC_DutyHighPercent50,
    .wDQDuty0      = kMMDC_DutyHighPercent50,
    .wDQDuty1      = kMMDC_DutyHighPercent50,
};

mmdc_zq_config_t zqCal = {
    .mode             = kMMDC_ZQCaltoIODeviceLongShort,
    .earlyCompTimer   = 21U,
    .hwZQFreq         = kMMDC_ZQCalFreq1ms,
    .cmpOutSample     = 15U,
    .tZQCl_Clocks     = 144U,
    .tZQCs_Clocks     = 112U,
    .tZQInit_Clocks   = 400U,
    .cmpOutSample     = 7,
    .hwPullDownOffset = 0U,
    .hwPullUpOffset   = 0U,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    mmdc_config_t userConfig;
    mmdc_read_calibration_config_t readCal;
    mmdc_write_calibration_config_t writeCal;
    mmdc_device_config_t config;
    mmdc_auto_refresh_t autoRefresh;

    /* Hardware initialize. */
    BOARD_InitHardware();

    if (EXAMPLE_MMDC->MDCTL & MMDC_MDCTL_SDE_0_MASK)
    {
        PRINTF("\r\n MMDC already initialized. \r\n");
    }
    else
    {
        readCal.mode       = kMMDC_CalWithPreSetValue;
        readCal.readDelay0 = 0x4DU;
        readCal.readDelay1 = 0x52U;

        writeCal.mode        = kMMDC_CalWithPreSetValue;
        writeCal.writeDelay0 = 0x42U;
        writeCal.writeDelay1 = 0x36U;

        config.MR1 = 0x82U;
        config.MR2 = 0x01U;
        config.MR3 = 0x01U;

        autoRefresh.refreshCnt     = 0;
        autoRefresh.refreshRate    = 4;
        autoRefresh.refreshTrigSrc = kMMDC_RefreshTrigBy64K;

        /*
         *  config->bankInterleave = true;
         *  config->secondDDRClock = true;
         *  config->enableOnlyCS0 = true;
         *  config->devType = kMMDC_DDR3;
         *  config->devSize = 0x40000000U;
         *  config->devBank = kMMDC_Bank8;
         *  config->rowWidth = kMMDC_Row16Bits;
         *  config->colWidth = kMMDC_Col10Bits;
         *  config->burstLen = kMMDC_BurstLen8;
         *  config->odtByte1Config = kMMDC_RttNom60ohm;
         *  config->odtByte0Config = kMMDC_RttNom60ohm;
         *  config->enableActiveReadOdt = false;
         *  config->enableInactiveReadOdt = true;
         *  config->enableActiveWriteOdt = true;
         *  config->enableInactiveWriteOdt = true;
         */
        MMDC_GetDefaultConfig(&userConfig);
        userConfig.devType             = kMMDC_LPDDR2_S4;
        userConfig.devSize             = EXAMPLE_DDR_SIZE;
        userConfig.rowWidth            = EXAMPLE_DDR_ROW_WIDTH;
        userConfig.colWidth            = EXAMPLE_DDR_COL_WIDTH;
        userConfig.burstLen            = EXAMPLE_DDR_BURST_LEN;
        userConfig.timing              = &timing;
        userConfig.deviceConfig[0]     = &config;
        userConfig.readCalibration[0]  = &readCal;
        userConfig.writeCalibration[0] = &writeCal;
        userConfig.tuning              = &tuning;
        userConfig.autoRefresh         = &autoRefresh;
        userConfig.zqCalibration       = &zqCal;

        /* MMDC Initialization. */
        if (MMDC_Init(EXAMPLE_MMDC, &userConfig) != kStatus_Success)
        {
            PRINTF("\r\n MMDC Init Failed. \r\n");
        }
    }

    if (Device_ReadWrite())
    {
        PRINTF("\r\n DDR Write Data and Read Data Succeed.\r\n");
    }
    else
    {
        PRINTF("\r\n DDR Write Data and Read Data Failed.\r\n");
    }

    PRINTF("\r\n DDR Example End.\r\n");

    while (1)
    {
    }
}

static bool Device_ReadWrite(void)
{
    uint32_t index, i;
    uint32_t datalen       = DDR_EXAMPLE_DATALEN;
    uint32_t ddrSize       = EXAMPLE_DDR_SIZE / 4;                  /* Calculate DDR size in 4 bytes. */
    uint32_t *ddrStartAddr = (uint32_t *)MMDC_DEVICE_START_ADDRESS; /* DDR start address. */
    bool result            = true;

    /* Disable Data cache. */
    SCB_DisableDCache();

    /* Prepare data and write to DDR. */
    for (i = 0; i < ddrSize / datalen; i++)
    {
        for (index = 0; index < datalen; index++)
        {
            ddr_writeBuffer[index]              = index;
            *(uint32_t *)(ddrStartAddr + index) = ddr_writeBuffer[index];
        }
        ddrStartAddr += datalen;
    }

    PRINTF("\r\n DDR Write finished!\r\n");

    PRINTF("\r\n DDR Read Start, Start Address 0x%x, Data Length %d !\r\n", ddrStartAddr, datalen);

    ddrStartAddr = (uint32_t *)MMDC_DEVICE_START_ADDRESS;
    /* Read data from the DDR. */
    for (i = 0; i < ddrSize / datalen; i++)
    {
        for (index = 0; index < datalen; index++)
        {
            ddr_readBuffer[index] = *(uint32_t *)(ddrStartAddr + index);
        }
        ddrStartAddr += datalen;
    }
    PRINTF("\r\n DDR Read finished!\r\n");

    PRINTF("\r\n DDR Write Data and Read Data Compare Start!\r\n");

    ddrStartAddr = (uint32_t *)MMDC_DEVICE_START_ADDRESS;
    /* Compare the two buffers. */
    for (i = 0; i < ddrSize / datalen; i++)
    {
        /* Compare the two buffers. */
        if (memcmp(ddrStartAddr, ddr_writeBuffer, datalen))
        {
            result = false;

            for (i = 0; i < datalen; i++)
            {
                if (ddrStartAddr[i] != ddr_writeBuffer[i])
                {
                    PRINTF("\r\n DDR Read/write fail @ 0x%08X!\r\n", ddrStartAddr + i);
                }
            }
            break;
        }
        else
        {
            result = true;
        }
        ddrStartAddr += datalen;
    }

    return result;
}
