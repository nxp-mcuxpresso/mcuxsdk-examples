/*
 * Copyright 2021 NXP
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
#define DEMO_PDB_BASE       PDB0
#define DEMO_PDB_IRQ_ID     PDB0_IRQn
#define DEMO_PDB_IRQHandler ivINT_PDB0

#define DEMO_ADC16_BASE       SADC
#define DEMO_ADC16_GROUPID    0U
#define DEMO_ADC16_IRQ_ID     COCO_IRQn
#define DEMO_ADC16_IRQHandler ivINT_COCO

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DEMO_PDB_IRQHandler(void);
void DEMO_ADC16_IRQHandler(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
