/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * General purpose timer setup for supporting FreeRTOS runtime
 * task statistics
 */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#if (configGENERATE_RUN_TIME_STATS == 1)
#include "fsl_gpt.h"

volatile unsigned long ulHighFrequencyTimerTicks;

void GPT4_IRQHandler(void) {
  /* Clear interrupt flag.*/
  GPT_ClearStatusFlags(GPT4, kGPT_OutputCompare1Flag);
  ulHighFrequencyTimerTicks++;
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
  __DSB();
#endif
}

void vConfigureTimerForRunTimeStats(void) {
  uint32_t gptFreq;
  gpt_config_t gptConfig;

  GPT_GetDefaultConfig(&gptConfig);

  /* Initialize GPT module */
  GPT_Init(GPT4, &gptConfig);

  /* Divide GPT clock source frequency by 3 inside GPT module */
  GPT_SetClockDivider(GPT4, 3);

  /* Get GPT clock frequency */
  gptFreq = CLOCK_GetFreq(kCLOCK_OscRc48MDiv2);

  /* GPT frequency is divided by 3 inside module */
  gptFreq /= 3;

  /* FIXME - check why this is needed when running on CM4 
   * Even though the timer is configured identically with the one running 
   * on CM7, the frequency here is actually 2 times bigger than expected */
  gptFreq *= 2;

  /* Set GPT module to 10x of the FreeRTOS tick counter */
  gptFreq = USEC_TO_COUNT((100 * 1000)/configTICK_RATE_HZ, gptFreq);
  GPT_SetOutputCompareValue(GPT4, kGPT_OutputCompare_Channel1, gptFreq);

  /* Enable GPT Output Compare1 interrupt */
  GPT_EnableInterrupts(GPT4, kGPT_OutputCompare1InterruptEnable);

  /* Enable at the Interrupt and start timer */
  EnableIRQ(GPT4_IRQn);
  GPT_StartTimer(GPT4);
}

unsigned long vGetTimerForRunTimeStats()
{
    return ulHighFrequencyTimerTicks;
}
#endif
