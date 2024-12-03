/*
 * Copyright 2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef QEI_CONFIG_H_
#define QEI_CONFIG_H_
#include <stdint.h>
/*************************** BEGIN user configuration parameters for QEI engine  ****************************/
#define COUNT_MAX   100      /* QEI count maximum value in decimal */
#define SCAN_PERIOD 30 * 150 /* Scan period in 30us @150MHz */

/* Defined Address for GPIO data register*/
#define LPC55S69_GPIO_BASE 0x4008C000
#define GPIO_B0_ADDRESS    (LPC55S69_GPIO_BASE + 0)
#define LPC55S69_GPIO_BASE 0x4008C000
#define GPIO_B1_ADDRESS    (LPC55S69_GPIO_BASE + 0x20)

/* Defined GPIO Port for QEI */
#define PHASE_A                                                                                                      \
    (GPIO_B1_ADDRESS + 5)  /* Port1 pin # for PHASE_A, P1_5  in the case, the pin must be initialized as GPIO in the \
                              BOARD_InitPins firstly */
#define PHASE_B                                                                                                      \
    (GPIO_B0_ADDRESS + 16) /* Port0 pin # for PHASE_B, P0_16 in the case, the pin must be initialized as GPIO in the \
                              BOARD_InitPins firstly */
/****************************END user configuration parameters for QEI engine ******************************/

typedef struct _QEI_Para
{
    uint32_t *coprocessor_stack;
    uint32_t *p_buffer;
} QEI_Para;

#define QEI_IRQn       Reserved46_IRQn
#define QEI_IRQHandler Reserved46_IRQHandler
void QEI_Start(void);
void QEI_Init(void *pPara);

#endif /* QEI_CONFIG_H_ */
