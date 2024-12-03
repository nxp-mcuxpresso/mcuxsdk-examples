/*
 * Copyright 2020, 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_ACOMP_IRQHANDLER  GAU_ACOMP_INT_FUNC11_IRQHandler
#define EXAMPLE_ACOMP_ID          kACOMP_Acomp0
#define EXAMPLE_ACOMP_BASE        GAU_ACOMP
#define EXAMPLE_ACOMP_POS_CHANNEL kACOMP_PosCh0
#define EXAMPLE_ACOMP_NEG_CHANNEL kACOMP_NegChVIO_0P50

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
