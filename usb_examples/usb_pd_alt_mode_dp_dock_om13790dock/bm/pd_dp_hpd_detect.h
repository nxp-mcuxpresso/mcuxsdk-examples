/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PD_DP_HPD_DETECT_H__
#define __PD_DP_HPD_DETECT_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _pd_hpd_detect_config
{
    uint8_t hpdDetectPort;
    uint8_t hpdDetectPin;
} pd_hpd_detect_config_t;

typedef struct _pd_hpd_detect_instance
{
    pd_handle pdHandle;
    GPIO_HANDLE_DEFINE(gpioHpdHandle);
    volatile uint32_t debounceTime;
    pd_hpd_detect_config_t *hpdDetectConfig;
    volatile uint32_t hpdDetectChangeTick;
    volatile uint8_t hpdDetectHigh;
    volatile uint8_t hpdHigh;
    volatile uint8_t hpdDetectQueueEnable;
} pd_hpd_detect_t;

/*******************************************************************************
 * API
 ******************************************************************************/

uint8_t PD_DpHpdDetectInit(pd_hpd_detect_t *hpdDetect, void *pdHandle, void *hpdConfig);
uint8_t PD_DpHpdDetectDeinit(pd_hpd_detect_t *hpdDetect);
void PD_DpHpdModeEnterEnableDetectQueue(pd_hpd_detect_t *hpdDetect);
void PD_DpHpdDetectStart(pd_hpd_detect_t *hpdDetect);
void PD_DpHpdDetectStop(pd_hpd_detect_t *hpdDetect);
uint8_t PD_DpHpdPopOldestDetect(pd_hpd_detect_t *hpdDetect, pd_hpd_detect_type_t sent);
pd_hpd_detect_type_t PD_DpHpdPeekOldestDetect(pd_hpd_detect_t *hpdDetect);
void PD_DpHpdModeExitClearDetectQueue(pd_hpd_detect_t *hpdDetect);
void PD_DpHpdDetect1msISR(pd_hpd_detect_t *hpdDetect);
void PD_DpHpdDetectChangeCallback(void *callbackParam);
uint8_t PD_DpHpdDetectGetPinValue(pd_hpd_detect_t *hpdDetect);

#endif
