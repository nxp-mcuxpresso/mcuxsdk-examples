/*! *********************************************************************************
 * \defgroup Heart Rate Sensor
 * @{
 ********************************************************************************** */
/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 , NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define APP_BLE_BRIDGE_RPMSG_NS_ANNOUNCE_STRING ("rpmsg-virtual-tty-channel")

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
#ifdef __cplusplus
extern "C" {
#endif
void BOARD_InitHardware(void);
void BleApp_Init(void);
void BleApp_Start(void);
void BleApp_GenericCallback(gapGenericEvent_t *pGenericEvent);

#ifdef __cplusplus
}
#endif
/*${prototype:end}*/

#endif /* _APP_H_ */
