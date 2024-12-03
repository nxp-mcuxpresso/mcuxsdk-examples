/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "fsl_swm.h"
#include "pin_mux.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) { /* Function assigned for the Cortex-M0P */
  /* Enable clock of SWM. */
    CLOCK_EnableClock(kCLOCK_Swm);
    /* Configure pins for USART0. */
    /* Configure pins for USART0. */
    SWM_SetMovablePinSelect(SWM0, kSWM_USART0_RXD, kSWM_PortPin_P1_16);
    SWM_SetMovablePinSelect(SWM0, kSWM_USART0_TXD, kSWM_PortPin_P1_17);
    /* Configure pins for CAPT. */
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_X0, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_X1, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_X2, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_X3, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_X4, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_X5, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_X6, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_X7, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_X8, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_YH, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_CAPT_YL, true);
    
    /* Disable clock of SWM. The SWM clock can be disabled once the pins have been configred. */
    CLOCK_DisableClock(kCLOCK_Swm);
    
    CLOCK_EnableClock(kCLOCK_Iocon);
    /* Configure pins for CAPT. */
    IOCON->PIO[IOCON_INDEX_PIO0_31] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO1_0] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO1_1] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO1_2] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO1_3] &= ~IOCON_PIO_MODE_MASK;   
    IOCON->PIO[IOCON_INDEX_PIO1_4] &= ~IOCON_PIO_MODE_MASK; 
    IOCON->PIO[IOCON_INDEX_PIO1_5] &= ~IOCON_PIO_MODE_MASK; 
    IOCON->PIO[IOCON_INDEX_PIO1_6] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO1_7] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO1_8] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO1_9] &= ~IOCON_PIO_MODE_MASK;
    /* Configure pins for LED. */
    IOCON->PIO[IOCON_INDEX_PIO0_15] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO0_16] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO0_17] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO0_18] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO0_19] &= ~IOCON_PIO_MODE_MASK;  
    IOCON->PIO[IOCON_INDEX_PIO0_20] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO0_21] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO0_22] &= ~IOCON_PIO_MODE_MASK;
    IOCON->PIO[IOCON_INDEX_PIO0_23] &= ~IOCON_PIO_MODE_MASK; 
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
