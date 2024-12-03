/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "app.h"
#include "fsl_iap.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define startSector     10
#define endSector       10
#define BUFFER_LEN_WORD FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES / sizeof(uint32_t)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void error_trap(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Buffer for program */
static uint32_t page_buf[BUFFER_LEN_WORD];
/*! @brief Buffer for readback */
static uint32_t s_buffer_rbc[BUFFER_LEN_WORD];
/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * @brief Gets called when an error occurs.
 *
 * @details show error message and trap forever.
 */

void error_trap(void)
{
    while (1)
    {
        BOARD_LedMatrixShowNumber(1, 30, 1); /*! failed show number 1 */
    }
}

/*
 * @brief Gets called when the app is complete.
 *
 * @details show finshed message and trap forever.
 */
void app_finalize(void)
{
    while (1)
    {
        BOARD_LedMatrixShowNumber(0, 30, 1); /*! end show number 0*/
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    status_t i, status;
    uint32_t destAdrss;
    /* init the board hardware */
    BOARD_InitHardware();

    /* prepare the theth sector */
    status = IAP_PrepareSectorForWrite(startSector, endSector);
    if (status == kStatus_IAP_Success)
    {
        BOARD_LedMatrixShowNumber(0, 30, 1); /*! Successfully show number 0*/
    }
    else
        error_trap();

    status = IAP_EraseSector(startSector, endSector, SystemCoreClock);
    if (status == kStatus_IAP_Success)
    {
        BOARD_LedMatrixShowNumber(0, 30, 1); /*! Successfully show number 0*/
    }
    else
        error_trap();

    /* Generate data to be written to flash */
    for (i = 0; i < FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES; i++)
    {
        *(((uint8_t *)(&page_buf[0])) + i) = i;
    }
    /* Program the tenth sector */
    IAP_PrepareSectorForWrite(startSector, endSector);
    status = IAP_CopyRamToFlash((startSector * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES), (uint32_t *)page_buf,
                                sizeof(page_buf), SystemCoreClock);
    if (status == kStatus_IAP_Success)
    {
        BOARD_LedMatrixShowNumber(0, 30, 1); /*! Successfully show number 0*/
    }
    else
        error_trap();

    /* Verify sector contents */
    status =
        IAP_Compare((startSector * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES), (uint32_t *)page_buf, sizeof(page_buf));

    if (status == kStatus_IAP_Success)
    {
        BOARD_LedMatrixShowNumber(0, 30, 1); /*! Successfully show number 0*/
    }
    else
        error_trap();

    /* Verify programming by reading back from flash directly*/
    destAdrss = startSector * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES;
    for (uint32_t count = 0; count < BUFFER_LEN_WORD; count++)
    {
        s_buffer_rbc[count] = *(volatile uint32_t *)(destAdrss + count * 4);
        if (s_buffer_rbc[count] != page_buf[count])
        {
            error_trap();
        }
    }

    /* End of FLASHIAP Example */
    app_finalize();

    return 0;
}
