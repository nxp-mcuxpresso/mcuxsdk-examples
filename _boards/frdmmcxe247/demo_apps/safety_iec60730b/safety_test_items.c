/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"

#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = ".safety_ram"
#endif

/*******************************************************************************
 *                                                                              *
 *                           STRUCTURE FOR DIO Initialization and TEST          *
 *                                                                              *
 *******************************************************************************/

fs_dio_test_t dio_safety_test_item_0 = {
    .gpio   = GPIOD_BASE,
    .pcr    = PORTD_BASE, /* Base address of PCR register */
    .pinNum = 6,
    .pinDir = PIN_DIRECTION_IN,
    .pinMux = PIN_MUX_GPIO,
};

fs_dio_test_t dio_safety_test_item_1 = {
    .gpio   = GPIOD_BASE,
    .pcr    = PORTD_BASE, /* Base address of PCR register */
    .pinNum = 15,
    .pinDir = PIN_DIRECTION_IN,
    .pinMux = PIN_MUX_GPIO,
};

/* NULL terminated array of pointers to dio_test_t items for safety DIO test */
fs_dio_test_t *g_dio_safety_test_items[] = {&dio_safety_test_item_0, &dio_safety_test_item_1, NULL};

/*******************************************************************************
 *                                                                              *
 *                           STRUCTURE FOR AIO TEST                             *
 *                                                                              *
 *******************************************************************************/
#define ADC_MAX                 ((1 << (ADC_RESOLUTION)) - 1)
#define ADC_BANDGAP_LEVEL_RAW   (((ADC_BANDGAP_LEVEL) * (ADC_MAX)) / (ADC_REFERENCE))
#define ADC_MIN_LIMIT(val)      (uint16_t)(((val) * (100 - ADC_DEVIATION_PERCENT)) / 100)
#define ADC_MAX_LIMIT(val)      (uint16_t)(((val) * (100 + ADC_DEVIATION_PERCENT)) / 100)

// ADC0_SE0-Arduino_A4
fs_aio_test_a23468_t aio_safety_test_item_VL =
{
  .AdcChannel = 0,       
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(0),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(60),
  .state = FS_AIO_INIT
};

// ADC0_SE1-Arduino_A5
fs_aio_test_a23468_t aio_safety_test_item_VH =
{
  .AdcChannel = 1,
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(ADC_MAX),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(ADC_MAX),
  .state = FS_AIO_INIT
};

// ADC0_SE2-Arduino_A1
fs_aio_test_a23468_t aio_safety_test_item_BG =
{
  .AdcChannel = 2,
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(ADC_BANDGAP_LEVEL_RAW),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(ADC_BANDGAP_LEVEL_RAW),
  .state = FS_AIO_INIT
};

/* NULL terminated array of pointers to fs_aio_test_a2346_t items for safety AIO test */
fs_aio_test_a23468_t *g_aio_safety_test_items[] = {&aio_safety_test_item_VL,
                                                &aio_safety_test_item_VH,
                                                &aio_safety_test_item_BG,
                                                NULL};
