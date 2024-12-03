/*
 * Copyright 2019 NXP
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
#define SMARTCARD_Control(base, handle, control, param)   SMARTCARD_EMVSIM_Control(base, handle, control, 0)
#define SMARTCARD_TransferNonBlocking(base, handle, xfer) SMARTCARD_EMVSIM_TransferNonBlocking(base, handle, xfer)
#define SMARTCARD_Init(base, handle, sourceClockHz)       SMARTCARD_EMVSIM_Init(base, handle, sourceClockHz)
#define SMARTCARD_Deinit(base)                            SMARTCARD_EMVSIM_Deinit(base)
#define SMARTCARD_GetTransferRemainingBytes(base, handle) SMARTCARD_EMVSIM_GetTransferRemainingBytes(base, handle)
#define SMARTCARD_AbortTransfer(base, handle)             SMARTCARD_EMVSIM_AbortTransfer(base, handle)

#if defined(USING_PHY_TDA8035)
#define SMARTCARD_PHY_Activate(base, handle, resetType)     SMARTCARD_PHY_TDA8035_Activate(base, handle, resetType)
#define SMARTCARD_PHY_Deactivate(base, handle)              SMARTCARD_PHY_TDA8035_Deactivate(base, handle)
#define SMARTCARD_PHY_Control(base, handle, control, param) SMARTCARD_PHY_TDA8035_Control(base, handle, control, param)
#define SMARTCARD_PHY_Init(base, config, sourceClockHz)     SMARTCARD_PHY_TDA8035_Init(base, config, sourceClockHz)
#define SMARTCARD_PHY_Deinit(base, config)                  SMARTCARD_PHY_TDA8035_Deinit(base, config)
#define SMARTCARD_PHY_GetDefaultConfig(config)              SMARTCARD_PHY_TDA8035_GetDefaultConfig(config)
#endif

#if defined(USING_PHY_EMVSIM)
#define SMARTCARD_PHY_Activate(base, handle, resetType)     SMARTCARD_PHY_EMVSIM_Activate(base, handle, resetType)
#define SMARTCARD_PHY_Deactivate(base, handle)              SMARTCARD_PHY_EMVSIM_Deactivate(base, handle)
#define SMARTCARD_PHY_Control(base, handle, control, param) SMARTCARD_PHY_EMVSIM_Control(base, handle, control, param)
#define SMARTCARD_PHY_Init(base, config, sourceClockHz)     SMARTCARD_PHY_EMVSIM_Init(base, config, sourceClockHz)
#define SMARTCARD_PHY_Deinit(base, config)                  SMARTCARD_PHY_EMVSIM_Deinit(base, config)
#define SMARTCARD_PHY_GetDefaultConfig(config)              SMARTCARD_PHY_EMVSIM_GetDefaultConfig(config)
#endif
#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
