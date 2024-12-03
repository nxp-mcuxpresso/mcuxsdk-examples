/*
 * Copyright 2019-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_eeprom_emulation.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_PASS 0x0
#define DEMO_FAIL 0xFF

/* The maximum erease cycle value while swapping */
#define MAX_ERS_CYC_VAL 2

extern uint32_t g_freeSpaceAddress;
extern uint32_t g_currentActiveSector;

/* buffer to store data for read eeprom emulation */
uint8_t dataBuf[DATA_VALUE_SIZE];
/* source buffer to write data */
uint32_t sourceBuf[SOURCE_BUF_SIZE];
/* the seed to generate different data for different ID */
uint16_t seed[MAX_RECORD_NUMBER];

/* length of this array depends on total size of the functions need to be copied to RAM*/
#if EE_CALLBACK_ENABLE
uint16_t __ram_for_callback[CALLBACK_SIZE / 2];
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
uint32_t DEMO_InitEeprom(eeprom_emulation_handle_t *handle);
uint32_t DEMO_WriteEeprom(eeprom_emulation_handle_t *handle, uint16_t dataID, uint32_t sourceBuf);
uint32_t DEMO_ReadEeprom(eeprom_emulation_handle_t *handle, uint16_t dataID, uint8_t *readBuf);
void FillBuf(uint32_t buf, uint32_t startVal, uint16_t order);
void DEMO_InitOperation(void);
uint32_t DEMO_Setup(eeprom_emulation_handle_t *handle);

#if (EE_CALLBACK_ENABLE)
void EECallBack();
uint32_t g_EECalBackCnt;
#endif /*_EE_CALLBACK_*/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * @brief Gets called when an error occurs.
 *
 * @details Print error message and trap forever.
 */
void error_trap(void)
{
    PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLASH ERROR! ----");
    while (1)
    {
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t returnCode;
    uint32_t ersCycVal;
    uint32_t i;
    uint16_t dataID;
    uint8_t demoResult = DEMO_PASS;

    eeprom_emulation_config_t config;
    eeprom_emulation_handle_t handle;

    BOARD_InitHardware();

#if defined(SMSCM_CACHE_CLEAR_MASK) && SMSCM_CACHE_CLEAR_MASK
    /* disable flash cache/Prefetch */
    FLASH_CACHE_Disable();
#endif /* SMSCM_CACHE_CLEAR_MASK */

    PRINTF("EEPROM demo begins...\r\n");

    /* Clean up eeprom emulation config, handle driver Structure*/
    memset(&config, 0, sizeof(eeprom_emulation_config_t));
    memset(&handle, 0, sizeof(eeprom_emulation_handle_t));

    /* Set EEPROM emulation information */
    EE_GetDefaultConfig(&config);

    config.flashSectorSize         = FLASH_SECTOR_SIZE; /* 4KB in byte */
    config.flashStartAddress       = FLASH_START_ADDR;
    config.eeSectorSize            = SECTOR_SIZE; /* 8KB in byte for K70 */
    config.eeStartAddress          = START_ADDRESS;
    config.eeSectorActIndOffset    = 0x00U;
    config.eeActualReadySectors    = ACTUAL_READY_SECTORS;
    config.eeMinActualReadySectors = MIN_ACTUAL_READY_SECTORS;
    config.eeExtraReadySectors     = EXTRA_READY_SECTORS;
    config.eeExtraActiveSectors    = EXTRA_ACTIVE_SECTORS;
    config.eeDataValueSize         = DATA_VALUE_SIZE;
    config.eeMemorySize            = EEPROM_SIZE;
    config.ftfxProgramSize         = FTFx_PGM_SIZE;
    config.ftfxPGMCheckSize        = FTFx_PGMCHECK_SIZE;
    config.ftfxRD1SECSize          = FTFx_RD1SEC_SIZE;
    config.flashReadMargin         = FTFx_USER_MARGIN;

    config.eeRetryMax = RETRY_MAX;
    config.eccSize    = ECC_SIZE;

    EE_SetEepromEmulationInfo(&handle, &config);

    /* Disable cache first */
    // EE_DisableCache(DEMO_FMC);

    int sectorAddress = handle.eeStartAddress;
    for (int i = 0x00u; i < handle.eeAllotedSectors; i++)
    {
        EE_SyncFlashErase(&handle, sectorAddress);
        sectorAddress += handle.eeSectorSize;
    }

    /* Initialize Eeprom emulation */
    returnCode = DEMO_InitEeprom(&handle);
    if (returnCode != EE_OK)
    {
        demoResult |= DEMO_FAIL;
        error_trap();
    }

    /* Write some records */
    for (dataID = 0x1; dataID < MAX_RECORD_NUMBER - 5; dataID++)
    {
        seed[dataID - 1]++;
        FillBuf((uint32_t)sourceBuf, dataID, seed[dataID - 1]);
        returnCode = DEMO_WriteEeprom(&handle, dataID, (uint32_t)sourceBuf);
        if (returnCode != EE_OK)
        {
            demoResult |= DEMO_FAIL;
            error_trap();
        }
    }

    /* write until swapping for serveral times */
    dataID    = 0x1;
    ersCycVal = 0x1;
    while (ersCycVal <= MAX_ERS_CYC_VAL)
    {
        /* prepare buffer to write */
        seed[dataID - 1]++;
        FillBuf((uint32_t)sourceBuf, dataID, seed[dataID - 1]);
        /* write data to Eeprom */
        returnCode = DEMO_WriteEeprom(&handle, dataID, (uint32_t)sourceBuf);

        if (returnCode != EE_OK)
        {
            demoResult |= DEMO_FAIL;
        }
        dataID++;
        if (dataID > MAX_RECORD_NUMBER - 5)
        {
            dataID = 0x1;
        }
        /* report Eeprom status to get the current erase cycle of Eeprom */
        ersCycVal = EE_ReportStatus(&handle);
    }

    /* read Eeprom data */
    for (dataID = 0x1; dataID < MAX_RECORD_NUMBER - 5; dataID++)
    {
        /* re-prepare source buffer */
        FillBuf((uint32_t)sourceBuf, dataID, seed[dataID - 1]);
        returnCode = DEMO_ReadEeprom(&handle, dataID, dataBuf);

        /* At this time, make sure all valid record ID are written to Eeprom emulation. So, expected returnCode is
         * EE_OK*/
        /* verify the data */
        for (i = 0; i < DATA_VALUE_SIZE; i++)
        {
            if (READ8((uint32_t)sourceBuf + i) != dataBuf[i])
            {
                demoResult |= DEMO_FAIL;
                break;
            }
        }
        if (demoResult != DEMO_PASS)
        {
            dataID++;
            break;
        }
    }

    /* if don't want to use Eeprom any more, just remove it */
    returnCode = EE_Deinit(&handle);
    if (returnCode != EE_OK)
    {
        demoResult |= DEMO_FAIL;
    }

    /* Restore cache */
    // EE_RestoreCache(DEMO_FMC);

    if (demoResult == DEMO_PASS)
    {
        PRINTF("All data is correct! EEPROM Emulation example succeed!\r\n");
    }

    while (1)
    {
    }
}

uint32_t DEMO_InitEeprom(eeprom_emulation_handle_t *handle)
{
    uint32_t returnCode;
    uint32_t i;
    uint32_t cnt;
    cnt = 0x0;

    /* initialize buffer */
    for (i = 0; i < MAX_RECORD_NUMBER; i++)
    {
        seed[i] = 0x0;
    }
    for (i = 0; i < DATA_VALUE_SIZE; i++)
    {
        dataBuf[i] = 0x0;
    }
    for (i = 0; i < SOURCE_BUF_SIZE; i++)
    {
        sourceBuf[i] = 0x0;
    }

    /* Need to init the condition before using this Eeprom emulation if necessary. */
    DEMO_InitOperation();

    /* To increate the robustness of driver in case of brownout, the FSL_InitEeprom shall be called for
    several times (5 times for example) in case of returning EE_NOT_OK.
    */
    while (cnt < MAX_REPEAT_TIMES)
    {
        returnCode = EE_Init(handle);
        if (returnCode != EE_NOT_OK)
        {
            break;
        }
        cnt++;
    }
    return (returnCode);
}

/*******************************************************************************
 *
 *  Function Name    : App_WriteEeprom
 *  Description      : To write 1 record to Eeprom emulation.
 *  Arguments        :
 *                    - dataID: record ID need to be written.
 *                    - sourceBuf: source data buffer of the record.
 *  Return Value     : successful or unsuccessful operation.
 *
 *******************************************************************************/
uint32_t DEMO_WriteEeprom(eeprom_emulation_handle_t *handle, uint16_t dataID, uint32_t sourceBuf)
{
    uint32_t returnCode;
    uint32_t cnt;
    cnt = 0x0;
    while (cnt < MAX_REPEAT_TIMES)
    {
        returnCode = EE_WriteData(handle, dataID, (uint32_t)sourceBuf);

        if (EE_ERR_UPDATE == returnCode)
        {
            returnCode = DEMO_InitEeprom(handle);
            if (EE_OK != returnCode)
            {
                /* exit if init Eeprom unsuccessfully */
                break;
            }
        }
        else if (EE_OK == returnCode)
        {
            /*Deleted asynchronous model, add it here if needed */
            break;
        }
        cnt++;
        /* if FSL_WriteEeprom() returns EE_NOT_OK, continue write this record 1 more time */
    }
    return (returnCode);
}

uint32_t DEMO_ReadEeprom(eeprom_emulation_handle_t *handle, uint16_t dataID, uint8_t *readBuf)
{
    uint32_t returnCode;
    uint32_t recordAddr;
    uint32_t i;

    returnCode = EE_ReadData(handle, dataID, &recordAddr);

    if (EE_OK == returnCode)
    {
        /* read out data if returnCode is EE_OK */
        for (i = 0; i < DATA_VALUE_SIZE; i++)
        {
            *(readBuf + i) = READ8(recordAddr + i);
        }
    }
    return (returnCode);
}

void FillBuf(uint32_t buf, uint32_t startVal, uint16_t order)
{
    uint32_t i;
    uint8_t *pBuf = (uint8_t *)buf;

    for (i = 0; i < DATA_VALUE_SIZE; i++)
    {
        pBuf[i] = startVal + i + order;
    }
}

/*******************************************************************************
*
*  Function Name    : EE_InitOperation
*  Description      : To init the interrupt in Async'ed mode, the relocation of command sequence
                      and call back function if necessary.
in user's application.
*  Arguments        : n/a
*  Return Value     : n/a.
*
*******************************************************************************/
void DEMO_InitOperation(void)
{
#if (EE_USE_ASYNCHRONOUS_MODEL == true)
    InterruptSetup();
#endif

#if (EE_CALLBACK_ENABLE == true)
    g_EECallBack = (PEE_FUNCPOINTER)EECallBack;
#endif
}

/*******************************************************************************
 *
 *  Function Name    : EE_Setup
 *  Description      : To prepare the initialized condition before using this Eeprom
 *                    emulation. This function is called once at the beginning of this usage.
 *  Arguments        : n/a
 *  Return Value     : successful or unsuccessful operation.
 *
 *******************************************************************************/
uint32_t DEMO_Setup(eeprom_emulation_handle_t *handle)
{
    uint32_t returnCode;

    /* Need to init condition before using this Eeprom emulation if necessary. */
    DEMO_InitOperation();

    /* Need to clean up flash before using Eeprom emulation */
    returnCode = EE_Deinit(handle);

    return (returnCode);
}

#if (EE_CALLBACK_ENABLE == true)
void EECallBack()
{
    /* Put user code here, for example reset watchdog code */
}
#endif
