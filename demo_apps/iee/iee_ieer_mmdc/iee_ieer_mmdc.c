/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_mmdc.h"
#include "fsl_iee.h"
#include "fsl_ieer.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AES_KEY_LEN 32

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*
 *  device read and write
 */
static bool Device_ReadWrite(void);
void IEE_IEER_Setup(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const uint8_t key1[] = {0x15, 0xAE, 0x2B, 0xF0, 0x22, 0x93, 0x81, 0x98, 0xA4, 0xA4, 0x2A,
                               0x07, 0xD1, 0x86, 0x50, 0x77, 0x4A, 0xAE, 0x17, 0x75, 0x05, 0x78,
                               0x2E, 0x58, 0x78, 0xCA, 0x1C, 0xB4, 0xC4, 0x85, 0x5B, 0x07};
static const uint8_t key2[] = {0x76, 0xA2, 0xE5, 0xB2, 0x11, 0xFA, 0x3A, 0x44, 0x21, 0xF5, 0x27,
                               0xB6, 0x0A, 0xDE, 0x39, 0x1D, 0x2B, 0x18, 0x7A, 0x35, 0x75, 0x0C,
                               0x29, 0xBF, 0xE6, 0x93, 0xC6, 0x11, 0x13, 0x10, 0x82, 0xE1};

uint32_t ddr_writeBuffer[DDR_EXAMPLE_DATALEN]; /* ddr write buffer */
uint32_t ddr_readBuffer[DDR_EXAMPLE_DATALEN];  /* ddr read buffer */
uint32_t *g_ddrStartAddr;

mmdc_config_t userConfig;
mmdc_read_calibration_config_t readCal;
mmdc_write_calibration_config_t writeCal;
mmdc_device_config_t config;
mmdc_auto_refresh_t autoRefresh;

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
    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("Demo started.\r\n");

    if (EXAMPLE_MMDC->MDCTL & MMDC_MDCTL_SDE_0_MASK)
    {
        PRINTF("\r\n MMDC already initialized. \r\n");
    }
    else
    {
        readCal.mode       = kMMDC_CalWithMPR;
        readCal.readDelay0 = 0x40U;
        readCal.readDelay1 = 0x40U;

        writeCal.mode        = kMMDC_CalWithPreDefine;
        writeCal.writeDelay0 = 0x40U;
        writeCal.writeDelay1 = 0x40U;

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

    /* Allocate memory block for data in DDR (needed only for DDR target) */
    g_ddrStartAddr = malloc(DDR_EXAMPLE_DATALEN);

    if (g_ddrStartAddr == NULL)
    {
        PRINTF("\r\n Error allocating memory for data!\r\n");
        return 0;
    }

    /* Check if we are using DDR or SRAM target */
    if ((uint32_t)g_ddrStartAddr < DEMO_DDR_START_ADDRESS)
    {
        /* It seems we are using SRAM target, so we can use DEMO_DDR_START_ADDRESS directly */
        free(g_ddrStartAddr);
        g_ddrStartAddr = (uint32_t *)DEMO_DDR_START_ADDRESS;
    }

    /* Setup IEE and IEER for secure write to DDR */
    IEE_IEER_Setup();

    if (Device_ReadWrite())
    {
        PRINTF("\r\n DDR Write Data and Read Data Succeed.\r\n");
    }
    else
    {
        PRINTF("\r\n DDR Write Data and Read Data Failed.\r\n");
    }

    PRINTF("\r\n IEE & IEER & MMDC demo End.\r\n");

    while (1)
    {
    }
}

void IEE_IEER_Setup(void)
{
    iee_config_t iee_config;
    ieer_config_t ieerConfig;

    CLOCK_EnableClock(kCLOCK_Iee0);

    /* Reset IEE to factory defaults */
    IEE_Init(IEE);

    /* Configure IEE region 3 to access DDR */
    IEE_GetDefaultConfig(&iee_config);
    iee_config.bypass     = kIEE_AesUseMdField;
    iee_config.mode       = kIEE_ModeAesXTS;
    iee_config.keySize    = kIEE_AesCTR256XTS512;
    iee_config.pageOffset = IEE_GetOffset(IEE_START_ADDRESS, (uint32_t)g_ddrStartAddr);
    IEE_SetRegionConfig(IEE, kIEE_Region3, &iee_config);

    /* Set KEY1 and KEY2 for region 3 */
    IEE_SetRegionKey(IEE, kIEE_Region3, kIEE_AesKey1, key1, AES_KEY_LEN);
    IEE_SetRegionKey(IEE, kIEE_Region3, kIEE_AesKey2, key2, AES_KEY_LEN);

    /* Configure IEER */
    IEER_Init(IEER);

    /* IEER region 3 to cover all IEE address space. */
    ieerConfig.startaddr = IEE_START_ADDRESS;
    ieerConfig.endaddr   = IEE_START_ADDRESS + IEE_LEN;
    /* a hit to IEE address space to be executed according to IEE region 3 settings */
    ieerConfig.rmsg  = 3; /* select IEE region 3 to provide settings */
    ieerConfig.valid = true;
    IEER_SetRegionConfig(IEER, kIEER_Region3, &ieerConfig);
}

static bool Device_ReadWrite(void)
{
    uint32_t index;
    uint32_t datalen = DDR_EXAMPLE_DATALEN;
    uint32_t *ieeCryptStartAddr;
    bool result = true;

    /* Address for crypted read/write to DDR (IEE_START_ADDRESS + offset from beginning of the DDR) */
    ieeCryptStartAddr = (uint32_t *)(IEE_START_ADDRESS + ((uint32_t)g_ddrStartAddr - DEMO_DDR_START_ADDRESS));

    /* Prepare data and write them to DDR. */
    for (index = 0; index < datalen; index++)
    {
        ddr_writeBuffer[index]                   = index;
        *(uint32_t *)(ieeCryptStartAddr + index) = ddr_writeBuffer[index];
    }

    PRINTF("\r\n DDR Write finished!\r\n");

    PRINTF("\r\n DDR Read Start, Start Address 0x%x, Data Length %d !\r\n", g_ddrStartAddr, datalen);

    /* Read crypted data directly from the DDR. */
    for (index = 0; index < datalen; index++)
    {
        ddr_readBuffer[index] = *(uint32_t *)(g_ddrStartAddr + index);
    }

    if (memcmp(ddr_readBuffer, ddr_writeBuffer, sizeof(ddr_readBuffer)))
    {
        PRINTF("\r\n DDR data are crypted!\r\n");
        result = true;
    }

    /* Read data from the IEE. */
    for (index = 0; index < datalen; index++)
    {
        ddr_readBuffer[index] = *(uint32_t *)(ieeCryptStartAddr + index);
    }

    PRINTF("\r\n DDR Read finished!\r\n");

    /* Compare the two buffers. */
    if (memcmp(ddr_readBuffer, ddr_writeBuffer, sizeof(ddr_readBuffer)))
    {
        result = false;
        PRINTF("\r\n DDR Read/write over IEE fail!\r\n");
    }
    else
    {
        result = true;
        PRINTF("\r\n DDR Read over IEE pass!\r\n");
    }

    return result;
}
