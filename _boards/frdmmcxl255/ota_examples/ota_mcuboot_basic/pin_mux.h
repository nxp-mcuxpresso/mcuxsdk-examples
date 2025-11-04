/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Configures pins.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);
void BOARD_InitACCELPins(void);
void BOARD_InitMAGSWPins(void);
void BOARD_InitI2CPins(void);
void BOARD_InitSPIPins(void);
void BOARD_InitQTMRPins(void);
void BOARD_InitLEDsPins(void);
void BOARD_InitACMPPins(void);
void BOARD_InitLCDPins(void);
void BOARD_InitKPPPins(void);
void BOARD_InitAonLpadcPins(void);
void BOARD_InitBUTTONsPins(void);
void BOARD_InitCtimerMatch0Pins(void);
void BOARD_InitCtimerMatch1Pins(void);
void BOARD_InitCtimerPwmPins(void);
void BOARD_InitCtimerCapPins(void);
void BOARD_InitAOIPins(void);
void BOARD_InitLPACMPPins(void);
void BOARD_InitFREQMEPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
