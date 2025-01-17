/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_mem_interface.h"
#include "fsl_common.h"
#include "app.h"
#include "pin_mux.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_LEN PFLASH_PAGE_SIZE_BYTES

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void error_trap();
static void app_finalize(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief config API context structure */
static api_core_context_t apiCoreCtx;

/*! @brief config API initialization data structure */
static const kp_api_init_param_t apiInitParam = {
    .allocStart = 0x30010000U, /* Allocate an area from ram for storing configuration information. */
    .allocSize  = 0x6000U      /* Configuration information size. */
};

/*! @brief Buffer for program */
static uint8_t s_buffer[BUFFER_LEN];
/*! @brief Buffer for readback */
static uint8_t s_buffer_rbc[BUFFER_LEN];

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
    PRINTF("\r\n\r\n\r\n\t---- Halted due to flash error! ----");
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
    PRINTF("\r\n End of internal flash example! \r\n");
    while (1)
    {
    }
}

int main()
{
    status_t status;
    uint32_t destAddress; /* Address of the target location */
    uint32_t pflashBlockBase  = 0U;
    uint32_t pflashTotalSize  = 0U;
    uint32_t pflashSectorSize = 0U;

    /* Init hardware */
    BOARD_InitHardware();

    /* Clean up API context structure*/
    memset(&apiCoreCtx, 0, sizeof(apiCoreCtx));

    /* print welcome message */
    PRINTF("\r\n Internal flash example start: \r\n");

    PRINTF("\r\n Calling API_Version!");
    standard_version_t iapVersion = API_Version();
    PRINTF("IAP API version = %d.%d.%d", iapVersion.major, iapVersion.minor, iapVersion.bugfix);

    PRINTF("\r\n Calling API_Init!");
    status = API_Init(&apiCoreCtx, &apiInitParam);
    if (status == kStatus_Success)
    {
        PRINTF("\r\n API_Init Successfully! \r\n ");
    }
    else
    {
        PRINTF("\r\n API_Init failure!\r\n");
        error_trap();
    }

    /* Get flash properties*/
    pflashBlockBase  = PFLASH_BASE_ADDRESS;
    pflashTotalSize  = PFLASH_TOTAL_SIZE_BYTES;
    pflashSectorSize = PFLASH_SECTOR_SIZE_BYTES;
    /* Print flash information - PFlash. */
    PRINTF("\r\n PFlash Information: ");
    PRINTF("\r\n Program Flash Base Address:\t%d KB, Hex: (0x%x)", (pflashBlockBase / 1024), pflashBlockBase);
    PRINTF("\r\n Total Program Flash Size:\t%d KB, Hex: (0x%x)", (pflashTotalSize / 1024), pflashTotalSize);
    PRINTF("\r\n Program Flash Sector Size:\t%d KB, Hex: (0x%x) ", (pflashSectorSize / 1024), pflashSectorSize);

/*
 * SECTOR_INDEX_FROM_END = 1 means the last sector,
 * SECTOR_INDEX_FROM_END = 2 means (the last sector - 1) ...
 */
#ifndef SECTOR_INDEX_FROM_END
#define SECTOR_INDEX_FROM_END 2U
#endif

    destAddress = pflashBlockBase + (pflashTotalSize - (SECTOR_INDEX_FROM_END * pflashSectorSize));

    /* Erase a sector from destAddress. */
    PRINTF("\r\n Erasing internal flash ");
    status = MEM_Erase(&apiCoreCtx, destAddress, pflashSectorSize, kMemoryInternal);
    if (status == kStatus_Success)
    {
        /* Print message for user. */
        PRINTF("\r\n Successfully erased internal flash 0x%x -> 0x%x\r\n", destAddress,
               (destAddress + pflashSectorSize));
    }
    else
    {
        error_trap();
    }

    /* Prepare user buffer. */
    for (uint32_t i = 0; i < BUFFER_LEN; i++)
    {
        s_buffer[i] = i;
    }

    /* Program user buffer into flash*/
    PRINTF("\r\n Calling MEM_Write to program internal flash! \r\n");
    status = MEM_Write(&apiCoreCtx, destAddress, sizeof(s_buffer), (uint8_t *)s_buffer, kMemoryInternal);
    if (status != kStatus_Success)
    {
        error_trap();
    }

    /* Verify programming by Calling MEM_Read */
    memcpy(s_buffer_rbc, (uint32_t *)destAddress, sizeof(s_buffer_rbc));

    /* Compare the read data. */
    status = memcmp(s_buffer, s_buffer_rbc, sizeof(s_buffer));
    if (status == kStatus_Success)
    {
        PRINTF("\r\n Successfully Programmed and Verified Location 0x%x -> 0x%x \r\n", destAddress,
               (destAddress + sizeof(s_buffer)));
    }
    else
    {
        error_trap();
    }

    app_finalize();

    return 0;
}
