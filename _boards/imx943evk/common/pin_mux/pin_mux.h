/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "fsl_iomuxc.h"

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/* Boot pin initialization (weak, can be overridden per demo) */

/* I2C */
void BOARD_InitI2C3Pins(void);
void BOARD_InitI2C4Pins(void);
void BOARD_InitI3c2Pins(void);

/* SPI */
void BOARD_InitLPSPI3Pins(void);
void BOARD_InitLPSPI8Pins(void);
void BOARD_InitSincLpspi3Pins(void);

/* SAI/Audio */
void BOARD_InitSAI1Pins(void);
void BOARD_InitPDMPins(void);

/* CAN */
void BOARD_InitCAN1Pins(void);

/* GPIO */
void BOARD_InitGPIOPins(void);

/* EtherCAT */
void BOARD_InitEcatResetPins(void);
void BOARD_InitEcatLinkPins(void);
void BOARD_InitEcatMDIOPins(void);
void BOARD_InitEcatCLKPins(void);
void BOARD_InitEcatI2CPins(void);
void BOARD_InitEcatPortRmiiPins(void);
void BOARD_InitEcatPortMiiPins(void);

/* XSPI/Flash */
void BOARD_InitXspiPins(void);

/* Encoder */
void BOARD_InitEncoder1Pins(void);
void BOARD_InitEncoder2Pins(void);
void BOARD_InitENCODER1Pins(void);
void BOARD_InitENCODER2Pins(void);

/* Motor Control */
void BOARD_Init_M1_PWM(void);
void BOARD_Init_M1_FAULTS(void);
void BOARD_Init_M1_Encoder(void);
void BOARD_Init_M1_SINC(void);
void BOARD_Init_M1_ENDAT2P2(void);
void BOARD_Init_M2_PWM(void);
void BOARD_Init_M2_FAULTS(void);
void BOARD_Init_M2_Encoder(void);
void BOARD_Init_M2_SINC(void);
void BOARD_Init_M2_ENDAT2P2(void);

/* PWM */
void BOARD_InitPWM1Pins(void);

/* SINC */
void BOARD_InitSinc2Pins(void);

/* NETC/Ethernet */
void BOARD_InitNETC_ETH0_Pins(void);
void BOARD_InitNETC_ETH1_Pins(void);
void BOARD_InitNETC_ETH2_Pins(void);
void BOARD_InitNETC_ETH3_Pins(void);
void BOARD_InitNETC_ETH4_Pins(void);

/* FlexIO */
void BOARD_InitFlexio1Pins(void);
void BOARD_InitFlexio1PwmPins(void);
void BOARD_InitFlexio1UartPins(void);
void BOARD_InitFlexio1SpiPins(void);

/* TPM */
void BOARD_InitTpm6Pins(void);
void BOARD_InitTpm2Pins(void);

/* EQDC */
void BOARD_InitEqdcPins(void);

/* LPIT */
void BOARD_InitLpitPwmPins(void);

/* XBAR */
void BOARD_InitXbarPins(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PIN_MUX_H_ */
