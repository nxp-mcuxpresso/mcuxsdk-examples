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
 *                           STRUCTURE FOR AIO TEST                             *
 *                                                                              *
 *******************************************************************************/
#define ADC_MAX                 ((1 << (ADC_RESOLUTION)) - 1)
#define ADC_BANDGAP_LEVEL_RAW   (((ADC_BANDGAP_LEVEL) * (ADC_MAX)) / (ADC_REFERENCE))
#define ADC_MIN_LIMIT(val)      (uint16_t)(((val) * (100 - ADC_DEVIATION_PERCENT)) / 100)
#define ADC_MAX_LIMIT(val)      (uint16_t)(((val) * (100 + ADC_DEVIATION_PERCENT)) / 100)



//P0_15    J132-2
fs_aio_test_a1_t aio_safety_test_item_VL =
{
  .AdcChannel    = 0x3U,
  .commandBuffer = 1,
  .SideSelect    = 0,/* 0 = A side, 1 = B side*/
  .softwareTriggerEvent =  1, /* write to the SWTRIG register, SWTRIG[SWT0] is associated with TCTRL0 */
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(0),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(60),
  .state = FS_AIO_INIT
};

//P0_16    J132-4
fs_aio_test_a1_t aio_safety_test_item_VH =
{
  .AdcChannel = 0x3U,
  .commandBuffer = 1,
  .SideSelect    = 1,/* 0 = A side, 1 = B side*/
  .softwareTriggerEvent =  1, /* write to the SWTRIG register, SWTRIG[SWT0] is associated with TCTRL0 */
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(ADC_MAX-60),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(ADC_MAX),
  .state = FS_AIO_INIT
};

//P0_23    J9-3
fs_aio_test_a1_t aio_safety_test_item_BG =
{
  .AdcChannel = 0x8U,
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


/*******************************************************************************
 *                                                                              *
 *                           STRUCTURE FOR DIO Initialization and TEST          *
 *                                                                              *
 *******************************************************************************/

fs_dio_test_lpc_t dio_safety_test_item_0 =                /* P1_4 */
    {.iocon_mode_shift = IOCON_PIO_MODE_SHIFT,            /*Device depend*/
     .pPort_byte       = (uint8_t *)&(GPIO->B[1][4]),     /*adress of byte register in GPIO*/
     .pPort_dir        = (uint32_t *)&(GPIO->DIR[1]),     /* asress of dir1 register*/
     .pPort_Iocon      = (uint32_t *)&(IOCON->PIO[1][4]), /* Adress of concrete IOCON register*/
     .pinNum           = 4,                               /*Position in DIR registor*/
     .gpio_clkc_shift  = SYSCON_AHBCLKCTRL0_GPIO1_SHIFT};

fs_dio_test_lpc_t dio_safety_test_item_1 =                /* P1_17 */
    {.iocon_mode_shift = IOCON_PIO_MODE_SHIFT,            /* Device depend */
     .pPort_byte       = (uint8_t *)&(GPIO->B[1][17]),     /*adress of byte register in GPIO*/
     .pPort_dir        = (uint32_t *)&(GPIO->DIR[1]),     /* asress of dir1 register*/
     .pPort_Iocon      = (uint32_t *)&(IOCON->PIO[1][17]), /* Adress of concrete IOCON register*/

     .pinNum          = 17, /*Position in DIR registor*/
     .gpio_clkc_shift = SYSCON_AHBCLKCTRL0_GPIO1_SHIFT};

/* NULL terminated array of pointers to dio_test_t items for safety DIO test */
fs_dio_test_lpc_t *g_dio_safety_test_items[] = {&dio_safety_test_item_0, &dio_safety_test_item_1, NULL};
