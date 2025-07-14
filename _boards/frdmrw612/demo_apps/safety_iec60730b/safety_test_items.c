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

//P0_21 (ARD_D4)    J1-10
//fs_dio_test_t dio_safety_test_item_0 = {
//    .gpio          = GPIO0_BASE_NS,
//    .pinNum        = 21U,
//    .pinDir        = PIN_DIRECTION_IN,
//    .pcr           = (uint32_t)PORT0_NS->PCR,
//};
//
////P0_22 (ARD_D7)    J1-16
//fs_dio_test_t dio_safety_test_item_1 = {
//    .gpio          = GPIO0_BASE_NS,
//    .pinNum        = 22U,
//    .pinDir        = PIN_DIRECTION_IN,
//    .pcr           = (uint32_t)PORT0_NS->PCR,
//};


/* NULL terminated array of pointers to dio_test_t items for safety DIO test */
//fs_dio_test_t *g_dio_safety_test_items[] = {&dio_safety_test_item_0, &dio_safety_test_item_1, NULL};


/*******************************************************************************
 *                                                                              *
 *                           STRUCTURE FOR AIO TEST                             *
 *                                                                              *
 *******************************************************************************/
#define ADC_DEVIATION_PERCENT 1
#define ADC_RESOLUTION 1
#define ADC_BANDGAP_LEVEL 1
#define ADC_REFERENCE 1
#define ADC_MAX                 ((1 << (ADC_RESOLUTION)) - 1)
#define ADC_BANDGAP_LEVEL_RAW   (((ADC_BANDGAP_LEVEL) * (ADC_MAX)) / (ADC_REFERENCE))
#define ADC_MIN_LIMIT(val)      (uint16_t)(((val) * (100 - ADC_DEVIATION_PERCENT)) / 100)
#define ADC_MAX_LIMIT(val)      (uint16_t)(((val) * (100 + ADC_DEVIATION_PERCENT)) / 100)



//Internal VREFL  
fs_aio_test_a1_t aio_safety_test_item_VL =
{
  .AdcChannel    = 0x7U,
  .commandBuffer = 1,
  .SideSelect    = 1,/* 0 = A side, 1 = B side*/
  .softwareTriggerEvent =  1, /* write to the SWTRIG register, SWTRIG[SWT0] is associated with TCTRL0 */
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(0),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(60),
  .state = FS_AIO_INIT
};

//GPIO_P4_15    J4-4
fs_aio_test_a1_t aio_safety_test_item_VH =
{
  .AdcChannel    = 0x1U,
  .commandBuffer = 1,
  .SideSelect    = 0,/* 0 = A side, 1 = B side*/
  .softwareTriggerEvent =  1, /* write to the SWTRIG register, SWTRIG[SWT0] is associated with TCTRL0 */
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(ADC_MAX-60),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(ADC_MAX),
  .state = FS_AIO_INIT
};

//GPIO_P4_6    J4-2
fs_aio_test_a1_t aio_safety_test_item_BG =
{
  .AdcChannel    = 0x3U,
  .commandBuffer = 1,
  .SideSelect    = 0,/* 0 = A side, 1 = B side*/
  .softwareTriggerEvent =  1, /* write to the SWTRIG register, SWTRIG[SWT0] is associated with TCTRL0 */
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(ADC_BANDGAP_LEVEL_RAW),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(ADC_BANDGAP_LEVEL_RAW),
  .state = FS_AIO_INIT
};


/* NULL terminated array of pointers to fs_aio_test_a1_t items for safety AIO test */
fs_aio_test_a1_t *g_aio_safety_test_items[] = {&aio_safety_test_item_VL,
                                                &aio_safety_test_item_VH,
                                                &aio_safety_test_item_BG,
                                                NULL};
