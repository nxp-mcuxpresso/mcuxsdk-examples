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
#define DEMO_ADC16_BASEADDR      ADC
#define DEMO_ADC16_CHANNEL_GROUP 0U

#define DEMO_ADC16_IRQ_ID           ADC_IRQn
#define DEMO_ADC16_IRQ_HANDLER_FUNC ADC_IRQHandler

#define DEMO_LPTMR_BASE LPTMR

#define kPmcBandgapBufferDriveLow kPMC_BandgapBufferDriveLow

/*
 * These values are used to get the temperature. DO NOT MODIFY
 * The method used in this demo to calculate temperature of chip is mapped to
 * Temperature Sensor for the HCS08 Microcontroller Family document (Document Number: AN3031)
 */
#define ADCR_VDD      (65535U) /* Maximum value when use 16b resolution */
#define V_BG          (1000U)  /* BANDGAP voltage in mV (trim to 1.0V) */
#define V_TEMP25      (716U)   /* Typical VTEMP25 in mV */
#define M             (1620U)  /* Typical slope: (mV x 1000)/oC */
#define STANDARD_TEMP (25U)

#define LED1_INIT() LED_RED_INIT(LOGIC_LED_OFF)
#define LED1_ON()   LED_RED_ON()
#define LED1_OFF()  LED_RED_OFF()

#define LED2_INIT() LED_GREEN_INIT(LOGIC_LED_OFF)
#define LED2_ON()   LED_GREEN_ON()
#define LED2_OFF()  LED_GREEN_OFF()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ConfigTriggerSource(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
