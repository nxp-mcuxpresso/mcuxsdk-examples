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

//GPIO_AD_08    J39-16
fs_dio_test_t dio_safety_test_item_0 = {
    .gpio          = GPIO4_BASE,
    .pcr           = (uint32_t)PORT4->PCR,
    .pinNum        = 5U,
    .pinDir        = PIN_DIRECTION_IN
};

//GPIO_AD_15    J39-6
fs_dio_test_t dio_safety_test_item_1 = {
    .gpio          = GPIO4_BASE,
    .pcr           = (uint32_t)PORT4->PCR,
    .pinNum        = 4U,
    .pinDir        = PIN_DIRECTION_IN
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



//GPIO_ANA_0    J4-2
fs_aio_test_a1_t aio_safety_test_item_VL =
{
  .AdcChannel    = 0x0U,
  .commandBuffer = 1,
  .SideSelect    = 0,/* 0 = A side, 1 = B side*/
  .softwareTriggerEvent =  1, /* write to the SWTRIG register, SWTRIG[SWT0] is associated with TCTRL0 */
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(0),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(60),
  .state = FS_AIO_INIT
};

//GPIO_ANA_1    J4-4
fs_aio_test_a1_t aio_safety_test_item_VH =
{
  .AdcChannel = 0x0U,
  .commandBuffer = 1,
  .SideSelect    = 1,/* 0 = A side, 1 = B side*/
  .softwareTriggerEvent =  1, /* write to the SWTRIG register, SWTRIG[SWT0] is associated with TCTRL0 */
  .Limits.low = (uint32_t)ADC_MIN_LIMIT(ADC_MAX-60),
  .Limits.high = (uint32_t)ADC_MAX_LIMIT(ADC_MAX),
  .state = FS_AIO_INIT
};

//GPIO_P4_23    J20-28
fs_aio_test_a1_t aio_safety_test_item_BG =
{
  .AdcChannel = 0x2U,
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
 *                           STRUCTURE FOR TSI TEST                             *
 *                                                                              *
 *******************************************************************************/

/*  Self cap TSI input */
fs_tsi_t tsi_safety_test_item_0 = {
    .limits.low              = TSI_CNT_LOW_LIMIT(TSI_VAL0),
    .limits.high             = TSI_CNT_HIGH_LIMIT(TSI_VAL0),
    .limits.delta            = TSI_DELTA(TSI_VAL0, TSI_SVAL0),
    .limits.deltaDev         = TSI_MIN_DELTA_PERCENT,
    .limits.deltaPer         = TSI_DELTA(TSI_VAL0, TSI_SVAL0) * TSI_MIN_DELTA_PERCENT / 100,
    .input.rxCH              = TSI0_CH19,            /*TSI_CHx*/
    .input.txCH              = SAFETY_SELFCAP_MODE, /*TSI_CHx*/
    .gpioInput.rxGpio        = GPIO1_BASE_NS,
    .gpioInput.rxPcr         = (uint32_t)PORT1_NS->PCR,
    .gpioInput.rxPinNum      = 10,
    .gpioInput.txGpio        = SAFETY_SELFCAP_MODE,
    .gpioInput.txPinNum      = 0,
    .stimPolarity            = 0,
    .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
    .tsiCounter.counterValue = 0,
};

fs_tsi_t tsi_safety_test_item_1 = {
    .limits.low              = TSI_CNT_LOW_LIMIT(TSI_VAL1),
    .limits.high             = TSI_CNT_HIGH_LIMIT(TSI_VAL1),
    .limits.delta            = TSI_DELTA(TSI_VAL1, TSI_SVAL1),
    .limits.deltaDev         = TSI_MIN_DELTA_PERCENT,
    .limits.deltaPer         = TSI_DELTA(TSI_VAL1, TSI_SVAL1) * TSI_MIN_DELTA_PERCENT / 100,
    .input.rxCH              = TSI0_CH21,            /*TSI_CHx*/
    .input.txCH              = SAFETY_SELFCAP_MODE, /*TSI_CHx*/
    .gpioInput.rxGpio        = GPIO1_BASE_NS,
    .gpioInput.rxPcr         = (uint32_t)PORT1_NS->PCR,
    .gpioInput.rxPinNum      = 12,
    .gpioInput.txGpio        = SAFETY_SELFCAP_MODE,
    .gpioInput.txPinNum      = 0,
    .stimPolarity            = 0,
    .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
    .tsiCounter.counterValue = 0,
};


/* Mutual mode electrodes */
fs_tsi_t tsi_safety_test_item_M0 = {
    .limits.low              = TSI_CNT_LOW_LIMIT(TSI_M_VAL0),
    .limits.high             = TSI_CNT_HIGH_LIMIT(TSI_M_VAL0),
    .limits.delta            = TSI_DELTA(TSI_M_VAL0, TSI_M_SVAL0),
    .limits.deltaDev         = TSI_MIN_DELTA_PERCENT,
    .limits.deltaPer         = TSI_DELTA(TSI_M_VAL0, TSI_M_SVAL0) * TSI_MIN_DELTA_PERCENT / 100,
    .input.rxCH              = TSI0_CH14, /*TSI_CHx*/
    .input.txCH              = TSI0_CH0, /*TSI_CHx*/
    .gpioInput.rxGpio        = GPIO0_BASE_NS,
    .gpioInput.rxPcr         = (uint32_t)PORT0_NS->PCR,
    .gpioInput.rxPinNum      = 19,
    .gpioInput.txGpio        = GPIO1_BASE_NS,
    .gpioInput.txPinNum      = 0,
    .stimPolarity            = 0,
    .tsiCounter.counterLimit = TSI_COUNTER_LIMIT,
    .tsiCounter.counterValue = 0,
};


/* NULL terminated array of pointers to dio_test_t items for safety TSI test */
fs_tsi_t *tsi_safety_test_items[] = {&tsi_safety_test_item_0,
                                     &tsi_safety_test_item_1,
                                     &tsi_safety_test_item_M0,
                                     NULL};
