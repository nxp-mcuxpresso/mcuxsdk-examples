/*
 * Copyright 2022-2023,2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef _BLEMGR_H_
#define _BLEMGR_H_

#include "fsl_os_abstraction.h"

typedef enum
{
    kBLEMGR_Event_AdvertisingStarted = 0,
    kBLEMGR_Event_Connected          = 1,
    kBLEMGR_Event_Disconnected       = 2
} blemgr_event_t;

/*******************************************************************************
 * API
 ******************************************************************************/
typedef void (*blemgr_event_cb_t)(blemgr_event_t event);

status_t BLEMGR_Init(osa_msgq_handle_t messageQueue, blemgr_event_cb_t eventCallback);
void BLEMGR_SetTemperature(int16_t temperature);
void BLEMGR_SetBatteryLevel(uint8_t level);
void BLEMGR_HostStackReady();

#endif /* _BLEMGR_H_ */
