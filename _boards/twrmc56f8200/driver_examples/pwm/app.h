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
/* The PWM base address */
#define BOARD_PWM_BASEADDR  PWMA
#define PWM_SRC_CLK_DIV     kPWM_ClockPrescaleDivide8
#define PWM_SRC_CLK_FREQ    (CLOCK_GetIpClkSrcFreq(kCLOCK_PWMACH0) / (1U << kPWM_ClockPrescaleDivide8))
#define PWM_IP_BUS_CLK_FREQ CLOCK_GetIpClkSrcFreq(kCLOCK_PWMACH0)
#define DEMO_FAULT_CHANNEL  1U
#define DEMO_FAULT_MAP              \
    (pwm_sm_fault_input_mapping_t)  \
    {                               \
        .bFaultInput5Mapping = true \
    }
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
