/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_reset.h"
#include "fsl_usart.h"
#include "fsl_clock.h"
#include "fsl_pint.h"
#include "freemaster.h"
#include "freemaster_serial_usart.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* Definition of peripheral ID */
#define FLEXCOMM0_PERIPHERAL ((USART_Type *)FLEXCOMM0)
/* Definition of the clock source frequency */
#define FLEXCOMM0_CLOCK_SOURCE 48000000UL
/* BOARD_InitPeripherals defines for PINT */
/* Definition of peripheral ID */
#define PINT_PERIPHERAL ((PINT_Type *) PINT_BASE)
/* PINT interrupt vector ID (number). */
#define PINT_PINT_0_IRQN PIN_INT0_IRQn
/* PINT interrupt vector priority. */
#define PINT_PINT_0_IRQ_PRIORITY 2
/* Definition of PINT interrupt ID for interrupt 0  */
#define PINT_INT_0 kPINT_PinInt0
/* FreeMASTER_Recorder_0's buffer size definition */
#define FREEMASTER_REC_0_SIZE 4096
/* Pipe1's Rx buffer size definition */
#define FREEMASTER_RX_PIPE_1_SIZE 32
/* Pipe1's Tx buffer size definition */
#define FREEMASTER_TX_PIPE_1_SIZE 32
/* Pipe2's Rx buffer size definition */
#define FREEMASTER_RX_PIPE_2_SIZE 32
/* Pipe2's Tx buffer size definition */
#define FREEMASTER_TX_PIPE_2_SIZE 32
/* Pipe3's Rx buffer size definition */
#define FREEMASTER_RX_PIPE_3_SIZE 32
/* Pipe3's Tx buffer size definition */
#define FREEMASTER_TX_PIPE_3_SIZE 32
/* Definition of peripheral ID */
#define FREEMASTER_SERIAL_PERIPHERAL ((USART_Type *)FLEXCOMM0)
/* Definition of the clock source frequency */
#define FREEMASTER_SERIAL_PERIPHERAL_CLK_FREQ 48000000UL

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern const usart_config_t FLEXCOMM0_config;
/* Recorder0 configuration */
extern FMSTR_U8 FreeMASTER_RecBuffer0[FREEMASTER_REC_0_SIZE];
/* Pipe1 handle */
extern FMSTR_HPIPE FreeMASTER_Pipe_handle_1;
/* Pipe1 Rx buffer */
extern FMSTR_U8 FreeMASTER_RxPipe1[FREEMASTER_RX_PIPE_1_SIZE];
/* Pipe1 Tx buffer */
extern FMSTR_U8 FreeMASTER_TxPipe1[FREEMASTER_TX_PIPE_1_SIZE];
/* Pipe2 handle */
extern FMSTR_HPIPE FreeMASTER_Pipe_handle_2;
/* Pipe2 Rx buffer */
extern FMSTR_U8 FreeMASTER_RxPipe2[FREEMASTER_RX_PIPE_2_SIZE];
/* Pipe2 Tx buffer */
extern FMSTR_U8 FreeMASTER_TxPipe2[FREEMASTER_TX_PIPE_2_SIZE];
/* Pipe3 handle */
extern FMSTR_HPIPE FreeMASTER_Pipe_handle_3;
/* Pipe3 Rx buffer */
extern FMSTR_U8 FreeMASTER_RxPipe3[FREEMASTER_RX_PIPE_3_SIZE];
/* Pipe3 Tx buffer */
extern FMSTR_U8 FreeMASTER_TxPipe3[FREEMASTER_TX_PIPE_3_SIZE];

/***********************************************************************************************************************
 * Callback functions
 **********************************************************************************************************************/
/* INT_0 callback function for the PINT component */
extern void BOARD_SW3_Callback(pint_pin_int_t pintr, uint32_t pmatch_status);

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/

void BOARD_InitPeripherals(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */
