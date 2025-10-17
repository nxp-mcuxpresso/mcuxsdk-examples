/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_npx.h"
#include "board.h"
#include "app.h"

#include "fsl_mem_interface.h"
#include "fsl_cache_lpcac.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* @brief Written data size into memory */
#define DATA_SIZE (0x1000U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief config API context structure */

SDK_ALIGN(static api_core_context_t apiCoreCtx, 8u);

/*! @brief NPX context structure */
SDK_ALIGN(static npx_region_config_t config, 8u);

/*! @brief config API initialization data structure */
static const kp_api_init_param_t apiInitParam = {
    .allocStart = 0x30010000U, /* Allocate an area from ram for storing configuration information. */
    .allocSize  = 0x6000U      /* Configuration information size. */
};

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Compares buffer with specified 32bit word pattern */
static inline bool memcmp_word_pattern(uint32_t *buffer, uint32_t pattern, size_t size)
{
    __ISB();
    __DSB();

    register size_t i      = 0;
    register uint32_t *ptr = NULL;
    while (i < ((size / sizeof(uint32_t)) - 1))
    {
        ptr = buffer + i;
        if (memcmp((void *)ptr, &pattern, sizeof(uint32_t)) != 0)
        {
            return false;
        }
        i++;
    }

    return true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    status_t status;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\nNPX Example.\r\n");
    
    
    /* Clean up API context structure*/
    memset(&apiCoreCtx, 0, sizeof(apiCoreCtx));
    
    config.startAddress = DATA_START_ADDR;
    config.endAddress   = DATA_END_ADDR;
    config.subRegionEnable = 0;
    
    uint32_t ctxIvReg[4] = {0xDEADBEEF,0xDEADBEEF,0xDEADBEEF,0xDEADBEEF};
    
    
    status = API_Init(&apiCoreCtx, &apiInitParam);
    if (status == kStatus_Success)
    {
        PRINTF("API_Init Successfully!\r\n ");
    }
    else
    {
        PRINTF("API_Init failure!\r\n");
    }
        
    /* Set NPX region 0 */
    status = NPX_SetRegion(NPX0, kNPX_Region0, config );
    if (status != kStatus_Success)
    {
        PRINTF("Error setting region0 config!\r\n");
    }
    
    /* Load IV into NPX registers. Only 8 bytes of ctxIvReg will be used! */
    status = NPX_SetRegionIV(NPX0, kNPX_Region0, (uint8_t *)ctxIvReg);
    if (status != kStatus_Success)
    {
        PRINTF("Error setting region0 nonce!\r\n");
    }
    
    
    /* Enable NPX encryption/decryption */
    NPX_EncryptEnable(NPX0);
    
    PRINTF("NPX was successfully configured for memory encryption/decryption from 0x%x to 0x%x.\r\n", config.startAddress,
           config.endAddress);    
    
    /* Fill memory with data to be encrypted */
    status = MEM_Fill(&apiCoreCtx, config.startAddress, DATA_SIZE, 0xaabbccdd, kMemoryInternal);
    if (status != kStatus_Success)
    {
        PRINTF("Error writing memory!\r\n");
    }
          
    L1CACHE_InvalidateCodeCache();
   
    /* Data shall match pattern as NPX do the On-the-Fly Encryption and Decryption*/
    /* So during write into memory in MEM_fill() is encrypted and during read in memcmp_word_pattern is decrypted */
    if (memcmp_word_pattern((void *)config.startAddress, 0xaabbccdd, DATA_SIZE) != true)
    {
        PRINTF("*Fail* Didnt match expected pattern on the fly encryption doesnt work!!! \r\n");
    }
    else
    {
        PRINTF("*Success* read programmed&encrypted data from 0x%x to 0x%x match expected pattern.\r\n", config.startAddress, config.endAddress);
    }
    
    /* Disable NPX On-the-Fly Encryption and Decryption */
    /* Data previsouly written when NPX is enabled shall be gibberish*/
    NPX_EncryptDisable(NPX0);
 
    /* Invalidate cache because after disabling NPX cache contain invalid cache data */
    L1CACHE_InvalidateCodeCache();
    
    /* Data should not match pattern as NPX has been disabled, so pattern shall be some gibberish ciphertext*/
    if (memcmp_word_pattern((void *)config.startAddress, 0xaabbccdd, DATA_SIZE) != false)
    {
        PRINTF("*Fail* Readed plaintext data, NPX didnt encrypt data during writing into memory!!! \r\n");
    }
    else
    {
        PRINTF("*Success* read at 0x%x to 0x%x doesnt match expected pattern.\r\n", config.startAddress, config.endAddress);
    }
    
    PRINTF("Example end.\r\n");

    while (1)
    {
    }
}
