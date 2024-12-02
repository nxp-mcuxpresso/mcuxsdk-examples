/*
 * Copyright 2021,2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_hardfaultFlag = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void Fault_handler()
{
#if defined(TRDC_DOMAIN_ERROR_OFFSET) && TRDC_DOMAIN_ERROR_OFFSET
    trdc_domain_error_t error;
    while (kStatus_Success == TRDC_GetAndClearFirstDomainError(EXAMPLE_TRDC_INSTANCE, &error))
    {
        APP_CheckAndResolveMrcAccessError(&error);
        APP_CheckAndResolveMbcAccessError(&error);
        g_hardfaultFlag = true;
    }
#else
    g_hardfaultFlag = true;
#if defined(FSL_FEATURE_TRDC_HAS_MRC) && FSL_FEATURE_TRDC_HAS_MRC
    APP_ResolveMrcAccessError();
#endif
#if defined(FSL_FEATURE_TRDC_HAS_MBC) && FSL_FEATURE_TRDC_HAS_MBC
    APP_ResolveMbcAccessError();
#endif
#endif
}
/*!
 * @brief BusFault_Handler
 */
void BusFault_Handler(void)
{
    Fault_handler();
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief HardFault_Handler
 */
void HardFault_Handler(void)
{
    Fault_handler();
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware.*/
    BOARD_InitHardware();

    /* Print the initial banner */
    PRINTF("TRDC example start\r\n");

    APP_SetTrdcGlobalConfig();
#if defined(FSL_FEATURE_TRDC_HAS_MRC) && FSL_FEATURE_TRDC_HAS_MRC
#if defined(KW45B41Z83_SERIES) || defined(KW45B41Z82_SERIES) || defined(KW45B41Z53_SERIES) || defined(KW45B41Z52_SERIES) || \
    defined(K32W1480_SERIES) || defined(MCXW716A_SERIES) || defined(MCXW716C_SERIES)
    /* For KW45B41Z/K32W1480/MCXW716 soc, The memory 0x48800000-0x48A00000 controlled by MRC0 belongs to the NBU flash memory,
       and CM33 core can only be able to access it if the silicon is NXP Fab or NXP Provisioned.
       Check the CLC bitfield of the LIFECYCLE register in MSCM peripheral to get the silicon revision. */
    if (((SMSCM->LIFECYCLE & SMSCM_LIFECYCLE_CLC_MASK) >> SMSCM_LIFECYCLE_CLC_SHIFT) < 7U)
#endif
    {
        /* Set the MRC unaccessible. */
        PRINTF("Set the MRC selected memory region not accessiable\r\n");
        APP_SetMrcUnaccessible();

        /* Touch the MRC, there will be hardfault. */
        g_hardfaultFlag = false;

        APP_TouchMrcMemory();

        /* Wait for the hardfault occurs. */
        while (!g_hardfaultFlag)
        {
        }
        PRINTF("The MRC selected region is accessiable now\r\n");
    }
#endif

#if defined(FSL_FEATURE_TRDC_HAS_MBC) && FSL_FEATURE_TRDC_HAS_MBC
    /* Set the MBC unaccessible. */
    PRINTF("Set the MBC selected memory block not accessiable\r\n");
    APP_SetMbcUnaccessible();

    /* Touch the MBC, there will be hardfault. */
    g_hardfaultFlag = false;

    APP_TouchMbcMemory();

    /* Wait for the hardfault occurs. */
    while (!g_hardfaultFlag)
    {
    }

    PRINTF("The MBC selected block is accessiable now\r\n");
#endif

    PRINTF("TRDC example Success\r\n");

    while (1)
    {
    }
}