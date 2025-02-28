/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_romapi.h"
#include "fsl_common.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_LEN 512 / 4

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void error_trap();
void app_finalize(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Flash driver Structure */
static flash_config_t s_flashDriver;
/*! @brief Buffer for program */
static uint32_t s_buffer[BUFFER_LEN];
/*! @brief Buffer for readback */
static uint32_t s_buffer_rbc[BUFFER_LEN];

volatile uint32_t g_systickCounter;

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

/*
 * @brief Gets called when the app is complete.
 *
 * @details Print finshed message and trap forever.
 */
void app_finalize(void)
{
    /* Print finished message. */
    PRINTF("\r\n End of PFlash Example! \r\n");
    while (1)
    {
    }
}

/*
 * @brief Clear speculation buffer.
 *
 */
void speculation_buffer_clear(void)
{
    /* Clear Flash/Flash data speculation. */
    if(((SYSCON->NVM_CTRL & SYSCON_NVM_CTRL_DIS_MBECC_ERR_INST_MASK) == 0U) 
        && ((SYSCON->NVM_CTRL & SYSCON_NVM_CTRL_DIS_MBECC_ERR_DATA_MASK) == 0U))
    {
        if((SYSCON->NVM_CTRL & SYSCON_NVM_CTRL_DIS_FLASH_SPEC_MASK) == 0U)
        {
            /* Disable flash speculation first. */
            SYSCON->NVM_CTRL |= SYSCON_NVM_CTRL_DIS_FLASH_SPEC_MASK;          
            /* Re-enable flash speculation. */
            SYSCON->NVM_CTRL &= ~SYSCON_NVM_CTRL_DIS_FLASH_SPEC_MASK;
        }
        if((SYSCON->NVM_CTRL & SYSCON_NVM_CTRL_DIS_DATA_SPEC_MASK) == 0U)
        {
            /* Disable flash data speculation first. */
            SYSCON->NVM_CTRL |= SYSCON_NVM_CTRL_DIS_DATA_SPEC_MASK;          
            /* Re-enable flash data speculation. */
            SYSCON->NVM_CTRL &= ~SYSCON_NVM_CTRL_DIS_DATA_SPEC_MASK;
        }
    }
}

/*
 * @brief Clear L1 low power cache.
 *
 */
void lpcac_clear(void)
{
    /* Clear L1 low power cache. */
    if((SYSCON->LPCAC_CTRL & SYSCON_LPCAC_CTRL_DIS_LPCAC_MASK) == 0U)
    {
        SYSCON->LPCAC_CTRL |= SYSCON_LPCAC_CTRL_CLR_LPCAC_MASK;
    }
}

int main()
{
    status_t status;
    uint32_t destAdrss; /* Address of the target location */
    uint32_t i, failedAddress, failedData;
    uint32_t pflashBlockBase  = 0U;
    uint32_t pflashTotalSize  = 0U;
    uint32_t pflashSectorSize = 0U;
    uint32_t PflashPageSize   = 0U;

    /* Init board hardware. */
    BOARD_InitHardware();

    /* Clean up Flash, Cache driver Structure*/
    memset(&s_flashDriver, 0, sizeof(flash_config_t));

    /* Print basic information for Flash Driver API.*/
    PRINTF("\r\n Flash driver API tree demo application. \r\n");
    /* Initialize flash driver */
    PRINTF("\r\n Initializing flash driver.");
    if (FLASH_API->flash_init(&s_flashDriver) != kStatus_Success)
    {
        error_trap();
    }
    PRINTF("\r\n Flash init successfull! \r\n");

    /* Adjust the clock cycle for accessing the flash memory according to the system clock. */
    PRINTF("\r\n Config flash memory access time. \r\n");

    /* Get flash properties kFLASH_ApiEraseKey */
    FLASH_API->flash_get_property(&s_flashDriver, kFLASH_PropertyPflashBlockBaseAddr, &pflashBlockBase);
    FLASH_API->flash_get_property(&s_flashDriver, kFLASH_PropertyPflashSectorSize, &pflashSectorSize);
    FLASH_API->flash_get_property(&s_flashDriver, kFLASH_PropertyPflashTotalSize, &pflashTotalSize);
    FLASH_API->flash_get_property(&s_flashDriver, kFLASH_PropertyPflashPageSize, &PflashPageSize);

    /* print welcome message */
    PRINTF("\r\n PFlash Information:");
    /* Print flash information - PFlash. */
    PRINTF("\r\n kFLASH_PropertyPflashBlockBaseAddr = 0x%X", pflashBlockBase);
    PRINTF("\r\n kFLASH_PropertyPflashSectorSize = %d", pflashSectorSize);
    PRINTF("\r\n kFLASH_PropertyPflashTotalSize = %d", pflashTotalSize);
    PRINTF("\r\n kFLASH_PropertyPflashPageSize = 0x%X", PflashPageSize);

/*
SECTOR_INDEX_FROM_END = 1 means the last sector,
SECTOR_INDEX_FROM_END = 2 means (the last sector -1 )...
*/
#ifndef SECTOR_INDEX_FROM_END
#define SECTOR_INDEX_FROM_END 2U
#endif

    destAdrss = pflashBlockBase + (pflashTotalSize - (SECTOR_INDEX_FROM_END * pflashSectorSize));

    PRINTF("\r\n Erase a sector of flash");
    status = FLASH_API->flash_erase_sector(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);
    if (status != kStatus_Success)
    {
        error_trap();
    }

    /* Clear speculation buffer and lpcac. */
    speculation_buffer_clear();
    lpcac_clear();

    /* Verify if the given flash range is successfully erased. */
    PRINTF("\r\n Calling flash_verify_erase_sector() API.");
    status = FLASH_API->flash_verify_erase_sector(&s_flashDriver, destAdrss, pflashSectorSize);
    if (status == kStatus_Success)
    {
        PRINTF("\r\n Successfully erased sector: 0x%x -> 0x%x\r\n", destAdrss, (destAdrss + pflashSectorSize));
    }
    else
    {
        error_trap();
    }

    /* Prepare user buffer. */
    for (i = 0; i < BUFFER_LEN; i++)
    {
        s_buffer[i] = i;
    }

    /* Start programming specified flash region */
    PRINTF("\r\n Calling FLASH_Program() API.");
    status = FLASH_API->flash_program_page(&s_flashDriver, destAdrss, (uint8_t *)s_buffer, sizeof(s_buffer));
    if (status != kStatus_Success)
    {
        error_trap();
    }

    /* Clear speculation buffer and lpcac. */
    speculation_buffer_clear();
    lpcac_clear();

    /* Verify if the given flash region is successfully programmed with given data */
    PRINTF("\r\n Calling FLASH_VerifyProgram() API.");
    status = FLASH_API->flash_verify_program(&s_flashDriver, destAdrss, sizeof(s_buffer), (const uint8_t *)s_buffer,
                                             &failedAddress, &failedData);
    if (status != kStatus_Success)
    {
        error_trap();
    }

    /* Verify programming by reading back from flash directly */
    for (uint32_t i = 0; i < BUFFER_LEN; i++)
    {
        s_buffer_rbc[i] = *(volatile uint32_t *)(destAdrss + i * 4);
        if (s_buffer_rbc[i] != s_buffer[i])
        {
            error_trap();
        }
    }

    PRINTF("\r\n Successfully programmed and verified location: 0x%x -> 0x%x \r\n", destAdrss,
           (destAdrss + sizeof(s_buffer)));

    /* resume flash memory status */
    status = FLASH_API->flash_erase_sector(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);

    app_finalize();

    return 0;
}
