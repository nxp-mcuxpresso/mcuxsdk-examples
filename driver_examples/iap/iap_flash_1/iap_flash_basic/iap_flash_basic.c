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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

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
    uint32_t status;
    uint32_t partID;
    uint32_t bootCodeVersion[2];
    uint32_t uniqueID[4];

    /* Board pin, clock, init */
    BOARD_InitHardware();

    /* Read part identification number */
    status = IAP_ReadPartID(&partID);
    if (status == kStatus_IAP_Success)
    {
        /* The value of partID is part identification number. */
        BOARD_LedMatrixShowNumber(0, 30, 1); /*! Successfully show number 0 */
    }
    else
        error_trap();

    /* Read boot code version */
    status = IAP_ReadBootCodeVersion(bootCodeVersion);
    if (status == kStatus_IAP_Success)
    {
        /* The boot code version is stored in the bootCodeVersion array. */
        BOARD_LedMatrixShowNumber(0, 30, 1); /*! Successfully read show number 0 */
    }
    else
        error_trap();

    /* Read unique identification number */
    status = IAP_ReadUniqueID(uniqueID);
    if (status == kStatus_IAP_Success)
    {
        /* The unique identification number is stored in the uniqueID array. */
        BOARD_LedMatrixShowNumber(0, 30, 1); /*! Successfully read show number 0 */
    }
    else
        error_trap();

    /* End of IAP Example */
    app_finalize();

    return 0;
}
