/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_iap.h"
#include "fsl_iap_ffr.h"
/*******************************************************************************
 * Definitions
 *******************************************************************************/
#define TEST_DEST_ADDRESS 0x80000U
#define FLASH_ERASE_SIZE  0x200

#define BUFFER_LEN 512

/*******************************************************************************
 * Prototypes
 *******************************************************************************/
static void app_finalize(void);
static void error_trap();

status_t Access_CFPA_pages(flash_config_t *flashInstance);
status_t Access_CMPA_pages(flash_config_t *flashInstance);
status_t Access_NMPA_pages(flash_config_t *flashInstance);

/*******************************************************************************
 * Variables
 *******************************************************************************/

/*! @brief Flash driver Structure */
static flash_config_t flashInstance;

/*******************************************************************************
 * Code
 *******************************************************************************/
int main()
{
    static uint32_t status;
    uint32_t FFRsectorSize    = 0;
    uint32_t FfrBlockBaseAddr = 0;
    uint32_t FfrTotalSize     = 0;
    uint32_t FfrPageSize      = 0;
    /* Init board hardware. */
    BOARD_InitHardware();
    /* Disable systick handler */
    SCB->SHCSR |= SCB_SHCSR_SYSTICKACT_Msk;
    /* Print basic information for FFR Driver API.*/
    PRINTF("\r\nFFR driver API tree Demo Application...\r\n");
    /* Initialize FFR driver */
    PRINTF("Initializing FFR driver...\r\n");
    status = FFR_Init(&flashInstance);
    if (status != kStatus_Success)
    {
        error_trap();
    }

    /* Get flash FFR properties */
    FLASH_GetProperty(&flashInstance, kFLASH_PropertyFfrBlockBaseAddr, &FfrBlockBaseAddr);
    FLASH_GetProperty(&flashInstance, kFLASH_PropertyFfrSectorSize, &FFRsectorSize);
    FLASH_GetProperty(&flashInstance, kFLASH_PropertyFfrTotalSize, &FfrTotalSize);
    FLASH_GetProperty(&flashInstance, kFLASH_PropertyFfrPageSize, &FfrPageSize);

    /* print basic message */
    PRINTF("FFR Example Start \r\n");
    /* Print flash information - PFlash. */
    PRINTF("\tkFLASH_PropertyFfrBlockBaseAddr = 0x%X\r\n", FfrBlockBaseAddr);
    PRINTF("\tkFLASH_PropertyFfrSectorSize = %d\r\n", FFRsectorSize);
    PRINTF("\tkFLASH_PropertyFfrTotalSize = %d\r\n", FfrTotalSize);
    PRINTF("\tkFLASH_PropertyFfrPageSize = %d\r\n", FfrPageSize);
    /* APIs access CFPA pages */
    PRINTF("\r\nStart access CFPA pages\r\n");
    status = Access_CFPA_pages(&flashInstance);
    if (status != kStatus_Success)
    {
        error_trap();
    }

    /* APIs to access CMPA pages */
    PRINTF("\r\nStart access CMPA pages\r\n");
    status = Access_CMPA_pages(&flashInstance);
    if (status != kStatus_Success)
    {
        error_trap();
    }

    /* APIs to access NMPA pages */
    PRINTF("\r\nStart access CMPA pages\r\n");
    status = Access_NMPA_pages(&flashInstance);
    if (status != kStatus_Success)
    {
        error_trap();
    }

    app_finalize();
}

status_t Access_CFPA_pages(flash_config_t *flashInstance)
{
    static status_t status;
    static uint32_t page_data[BUFFER_LEN / 4];
    static uint32_t pData[BUFFER_LEN / 4];
    /* access CFPA pages */
    for (uint32_t i = 0; i < 128; i++)
    {
        page_data[i] = 0;
    }
    /* Initializing FFR Customer Pages */
    status = FFR_CustomerPagesInit(flashInstance);
    status = FFR_InfieldPageWrite(flashInstance, (uint8_t *)page_data, BUFFER_LEN);
    if (status == kStatus_FLASH_OutOfDateCfpaPage)
    {
        PRINTF("\r\nCFPA page version is out of date.\r\n");
    }
    /* Read data stored in 'Customer In-field Page */
    status = FFR_GetCustomerInfieldData(flashInstance, (uint8_t *)pData, 0, BUFFER_LEN);

    return status;
}

status_t Access_CMPA_pages(flash_config_t *flashInstance)
{
    static status_t status;
    uint32_t pData[128];
    uint8_t pActivationCode[1192];
    uint32_t pKeyCode[13];
    ffr_key_store_t pKeyStore;
    for (uint32_t i = 0; i < 3; i++)
    {
        for (uint32_t k = 0; k < 512; k++)
        {
            pKeyStore.reserved[i][k] = 0;
        }
    }
    status = FFR_RecoverCmpaCfgPage(flashInstance);
    status = FFR_ProcessCmpaCfgPageUpdate(flashInstance, kFfrCmpaProgProcess_Pre);
    /* Read data stored in 'Customer Factory CFG Page */
    status = FFR_GetCustomerData(flashInstance, (uint8_t *)pData, 0, 298);
    status = FFR_KeystoreWrite(flashInstance, &pKeyStore);
    status = FFR_KeystoreGetAC(flashInstance, &pActivationCode[0]);
    status = FFR_KeystoreGetKC(flashInstance, (uint8_t *)&pKeyCode[0], kFFR_KeyTypeUser);
    return status;
}

status_t Access_NMPA_pages(flash_config_t *flashInstance)
{
    static uint32_t status;
    uint8_t pData[512];
    uint8_t uuid[16];
    status = FFR_GetRompatchData(flashInstance, &pData[0], 0, 16);
    status = FFR_GetManufactureData(flashInstance, &pData[0], 0, 128);
    status = FFR_GetUUID(flashInstance, &uuid[0]);
    return status;
}

/*
 * @brief Gets called when the app is complete.
 */
void app_finalize(void)
{
    while (1)
    {
    }
}

/*
 * @brief Gets called when an error occurs.
 */
void error_trap(void)
{
    PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLASH ERROR! ----");
    while (1)
    {
    }
}

/*******************************************************************************
 * EOF
 *******************************************************************************/
