/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
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
    .tCKE_Clocks        = 3U,
    .tCKSRE_Clocks      = 5U,
    .tCKSRX_Clocks      = 5U,
    .tRFC_Clocks        = 104U,
    .tXSR_Clocks        = 108U,
    .tXP_Clocks         = 3U,
    .tFAW_Clocks        = 16U,
    .tCL_Clocks         = 6U,
    .tMRD_Clocks        = 12U,
    .tRAS_Clocks        = 14U,
    .tWR_Clocks         = 6U,
    .tRTP_Clocks        = 4U,
    .tWTR_Clocks        = 4U,
    .tRRD_Clocks        = 4U,
    .ralat_Clocks       = 5U,
    .walat_Clocks       = 0U,
    .tRC_Clocks         = 20U,
    .tRCD_Clocks        = 6U,
    .tRP_Clocks         = 6U,
    .tRPA_Clocks        = 6U,
    .tCWL_Clocks        = 5U,
    .tRSTtoCKE_Clocks   = 33U,
    .tSDEtoRST_Clocks   = 14U,
    .tXPDLL_Clocks      = 10U,
    .tDLLK_Clocks       = 512U,
    .tXPR_Clocks        = 108U,
    .tAOFPD_Clocks      = 4U,
    .tAONPD_Clocks      = 4U,
    .tODTIdleOff_Clocks = 3U,
    .tRTWSAME_Clocks    = 2U,
    .tWTRDIFF_Clocks    = 3U,
    .tWTWDIFF_Clocks    = 3U,
    .tRTWDIFF_Clocks    = 2U,
    .tRTRDIFF_Clocks    = 2U,
};

mmdc_zq_config_t zqCal = {
    .mode             = kMMDC_ZQCaltoIODeviceLongShort,
    .earlyCompTimer   = 21U,
    .hwZQFreq         = kMMDC_ZQCalFreq1ms,
    .cmpOutSample     = 15U,
    .tZQCl_Clocks     = 256U,
    .tZQCs_Clocks     = 128U,
    .tZQInit_Clocks   = 512U,
    .cmpOutSample     = 7,
    .hwPullDownOffset = 0U,
    .hwPullUpOffset   = 0U,
};

mmdc_fine_tuning_config_t tuning = {
    .rDQOffset0    = 0x33333333U,
    .rDQOffset1    = 0x33333333U,
    .wDQOffset0    = 0xf3333333U,
    .wDQOffset1    = 0xf3333333U,
    .rDQDuty0      = kMMDC_DutyHighPercent50,
    .rDQDuty1      = kMMDC_DutyHighPercent50,
    .ddrCKDutyCtl0 = kMMDC_DutyHighPercent51_5,
    .ddrCKDutyCtl1 = kMMDC_DutyHighPercent51_5,
    .wDQDuty0      = kMMDC_DutyHighPercent48_5,
    .wDQDuty1      = kMMDC_DutyHighPercent48_5,
};

mmdc_writeLeveling_calibration_config_t writeLevelingCalibration = {
    .mode                = kMMDC_CalWithPreSetValue,
    .wLevelingOneDelay0  = 0,
    .wLevelingHalfDelay0 = 0,
    .wLevelingAbsDelay0  = 0x19U,
    .wLevelingOneDelay1  = 0,
    .wLevelingHalfDelay1 = 0,
    .wLevelingAbsDelay1  = 0x15U,
};

mmdc_read_calibration_config_t readCalibration = {
    .mode       = kMMDC_CalWithPreSetValue,
    .readDelay0 = 0x3EU,
    .readDelay1 = 0x3AU,
};

mmdc_write_calibration_config_t writeCalibration = {
    .mode        = kMMDC_CalWithPreSetValue,
    .writeDelay0 = 0x2AU,
    .writeDelay1 = 0x2FU,
};

mmdc_readDQS_calibration_config_t readDQSCalibration = {
    .mode                = kMMDC_CalWithPreSetValue,
    .waitCycles          = kMMDC_Wait32DDRCycles,
    .dqsGatingHalfDelay0 = 1U,
    .dqsGatingAbsDelay0  = 0x53U,
    .dqsGatingHalfDelay1 = 1U,
    .dqsGatingAbsDelay1  = 0x55U,
};

uint32_t ddr3_switchFreqInfo[][2] = {
    {0x14, 0x1C}, /* target freq offset and DLL status offset */
    {0x20, 0x48}, /* timing info offset and calibration offset */
    /* target freq 24000000 */
    {0, 24000000},
    /* dll status */
    {0, 1},
    /* timing info */
    {0x18, 0},          /* resotre misc register */
    {0x1C, 0x04008032}, /* config MR2 */
    {0xC, 0},           /* restore timing register 0 */
    {0x10, 0},          /* restore timing register 1 */
    {0x818, 0},         /* restore ODT config */

    /* calibration restore */
    {0, 3},
    {0x83C, 0},
    {0x848, 0},
    {0x850, 0},
};

extern uint32_t mmdc_ddr3_switch_freq_start;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    mmdc_config_t userConfig;
    mmdc_device_config_t config;
    mmdc_auto_refresh_t autoRefresh;
    mmdc_power_config_t power;
    bool rwStatus = true;

    /* Hardware initialize. */
    BOARD_InitHardware();

    if (EXAMPLE_MMDC->MDCTL & MMDC_MDCTL_SDE_0_MASK)
    {
        PRINTF("\r\n MMDC already initialized. \r\n");
    }
    else
    {
        /* device mode register configuration */
        config.MR0 = 0x1520U;
        config.MR1 = 0x4U;
        config.MR2 = 0x200U;
        config.MR3 = 0x0U;
        /* auto refresh scheme comfiguration */
        autoRefresh.refreshCnt     = 0;
        autoRefresh.refreshRate    = 2;
        autoRefresh.refreshTrigSrc = kMMDC_RefreshTrigBy64K;
        /* power configuration */
        power.idleClockToPS         = 0x10U;
        power.idleClockToPD0        = 0x05U;
        power.idleClockToPrecharge0 = 0X00U;
        power.idleClockToPD1        = 0x00U;
        power.idleClockToPrecharge1 = 0X00U;
        /* mmdc configuration */
        MMDC_GetDefaultConfig(&userConfig);
        userConfig.devSize                  = EXAMPLE_DDR_SIZE;
        userConfig.devBank                  = EXAMPLE_DDR_BANK;
        userConfig.devType                  = kMMDC_DDR3;
        userConfig.rowWidth                 = EXAMPLE_DDR_ROW_WIDTH;
        userConfig.colWidth                 = EXAMPLE_DDR_COL_WIDTH;
        userConfig.burstLen                 = EXAMPLE_DDR_BURST_LEN;
        userConfig.zqCalibration            = &zqCal;
        userConfig.deviceConfig[0U]         = &config;
        userConfig.timing                   = &timing;
        userConfig.tuning                   = &tuning;
        userConfig.readDQSCalibration[0U]   = &readDQSCalibration;
        userConfig.wLevelingCalibration[0U] = &writeLevelingCalibration;
        userConfig.readCalibration[0U]      = &readCalibration;
        userConfig.writeCalibration[0U]     = &writeCalibration;
        userConfig.autoRefresh              = &autoRefresh;
        userConfig.powerConfig              = &power;
        userConfig.enableOnlyCS0            = true;

        /* MMDC Initialization. */
        if (MMDC_Init(EXAMPLE_MMDC, &userConfig) != kStatus_Success)
        {
            PRINTF("\r\n MMDC Init Failed. \r\n");
        }
        PRINTF("\r\n MMDC Init Success. \r\n");
    }

    if (Device_ReadWrite())
    {
        PRINTF("\r\n   DDR Write Data and Read Data Succeed.\r\n");
    }
    else
    {
        PRINTF("\r\n   DDR Write Data and Read Data Failed.\r\n");
        rwStatus = false;
    }

    if (rwStatus)
    {
        /* switch to 24MHZ first */
        PRINTF("\r\n Start switch DDR work frequency to 24000000HZ. \r\n");

        /* restore parameter */
        ddr3_switchFreqInfo[4][1]  = EXAMPLE_MMDC->MDMISC;
        ddr3_switchFreqInfo[6][1]  = EXAMPLE_MMDC->MDCFG0;
        ddr3_switchFreqInfo[7][1]  = EXAMPLE_MMDC->MDCFG1;
        ddr3_switchFreqInfo[8][1]  = EXAMPLE_MMDC->MPODTCTRL;
        ddr3_switchFreqInfo[10][1] = EXAMPLE_MMDC->MPDGCTRL0;
        ddr3_switchFreqInfo[11][1] = EXAMPLE_MMDC->MPRDDLCTL;
        ddr3_switchFreqInfo[12][1] = EXAMPLE_MMDC->MPWRDLCTL;

        /* jump to switch frequency api */
        MMDC_SwitchDeviceFrequency(EXAMPLE_MMDC, EXAMPLE_CCM, EXAMPLE_IOMUXC, (uint32_t)ddr3_switchFreqInfo,
                                   (uint32_t)&mmdc_ddr3_switch_freq_start);

        PRINTF("\r\n Switch done. \r\n");
        /* perform read/write */
        if (Device_ReadWrite())
        {
            PRINTF("\r\n   DDR Write Data and Read Data Succeed.\r\n");
        }
        else
        {
            PRINTF("\r\n   DDR Write Data and Read Data Failed.\r\n");
            rwStatus = false;
        }
    }

    if (rwStatus)
    {
        /* switch to 400000000HZ back */
        PRINTF("\r\n Start switch DDR work frequency to 396000000HZ. \r\n");
        ddr3_switchFreqInfo[2][1] = 400000000; /* target freq */
        ddr3_switchFreqInfo[3][1] = 0;         /* dll is off */

        /* jump to switch frequency api */
        MMDC_SwitchDeviceFrequency(EXAMPLE_MMDC, EXAMPLE_CCM, EXAMPLE_IOMUXC, (uint32_t)ddr3_switchFreqInfo,
                                   (uint32_t)&mmdc_ddr3_switch_freq_start);

        /* switch to 400MHZ */
        PRINTF("\r\n Switch done. \r\n");

        /* perform read/write */
        if (Device_ReadWrite())
        {
            PRINTF("\r\n   DDR Write Data and Read Data Succeed.\r\n");
        }
        else
        {
            PRINTF("\r\n   DDR Write Data and Read Data Failed.\r\n");
        }
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
    uint32_t rwSize        = EXAMPLE_RW_SIZE;                       /* Calculate DDR size in 4 bytes. */
    uint32_t *ddrStartAddr = (uint32_t *)MMDC_DEVICE_START_ADDRESS; /* DDR start address. */
    bool result            = true;

    PRINTF("\r\n DDR work freq %d HZ.\r\n", BOARD_MMDC_FREQ);

    /* Prepare data and write to DDR. */
    for (i = 0; i < rwSize / datalen; i++)
    {
        for (index = 0; index < datalen; index++)
        {
            ddr_writeBuffer[index]              = index;
            *(uint32_t *)(ddrStartAddr + index) = ddr_writeBuffer[index];
        }
        ddrStartAddr += datalen;
    }

    ddrStartAddr = (uint32_t *)MMDC_DEVICE_START_ADDRESS;

    /* Read data from the DDR. */
    for (i = 0; i < rwSize / datalen; i++)
    {
        for (index = 0; index < datalen; index++)
        {
            ddr_readBuffer[index] = *(uint32_t *)(ddrStartAddr + index);
        }
        ddrStartAddr += datalen;
    }

    ddrStartAddr = (uint32_t *)MMDC_DEVICE_START_ADDRESS;
    /* Compare the two buffers. */
    for (i = 0; i < rwSize / datalen; i++)
    {
        /* Compare the two buffers. */
        if (memcmp(ddrStartAddr, ddr_writeBuffer, datalen))
        {
            result = false;

            for (i = 0; i < datalen; i++)
            {
                if (ddrStartAddr[i] != ddr_writeBuffer[i])
                {
                    PRINTF("\r\n   DDR Read/write fail @ 0x%08X!\r\n", ddrStartAddr + i);
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
