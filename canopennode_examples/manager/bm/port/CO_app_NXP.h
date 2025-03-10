/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CANOPENNODE_APP_NXP_H_
#define CANOPENNODE_APP_NXP_H_

#include "CANopen.h"
#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "fsl_gpt.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TX_MB_FIRST_INDEX 0
#define TX_MB_LAST_INDEX  7
#define TX_MB_NUMBER      (TX_MB_LAST_INDEX - TX_MB_FIRST_INDEX + 1)
#define RX_MB_FIRST_INDEX 8
#define RX_MB_LAST_INDEX  15
#define RX_MB_NUMBER      (RX_MB_LAST_INDEX - RX_MB_FIRST_INDEX + 1)

typedef struct {
    /* Pass in the timer that is going to be used for generating 1ms interrupt for tmrThread function,
     * Please note that CANOpenNXP Library will override GPT1_IRQHandler function,
     * if you also need this function in your codes, please take required steps
     */
    GPT_Type *base;
    gpt_interrupt_enable_t gptChMask;
    IRQn_Type gptIRQid;
} gpt_handle;

/* CANHandle: Pass in the CAN Handle to this function and it wil be used for all CAN Communications.
 *            It can be CAN-FD or CAN and CANOpenNXP Driver will take of care of handling that
 * HWInitFunction: Pass in the function that initialize the CAN peripheral, usually MX_CAN_Init
 * timerHandle: Pass in the timer that is going to be used for generating 1ms interrupt for tmrThread function,
 * Please note that CANOpenNXP Library will override GPT1_IRQHandler function, if you also need this function
 * in your codes, please take required steps
 */
typedef struct {
    /* This is the Node ID that you ask the CANOpen stack to assign to your device, although it might not always be the final NodeID,
     * after calling canopen_app_init() you should check ActiveNodeID of CANopenNodeNXP structure for assigned Node ID.
     */
    uint8_t desiredNodeID;
    uint8_t activeNodeID; /* Assigned Node ID */
    uint16_t baudrate;    /* This is the baudrate you've set in FlexCAN Configuration */
    gpt_handle timerHandle;
    /* Pass in the CAN Handle to this function and it wil be used for all CAN Communications.
     * It can be CAN-FD or CAN and CANOpenNXP Driver will take of care of handling that
     */
    CAN_Type *CANHandle;
    void (*HWInitFunction)(void); /* Pass in the function that initialize the CAN peripheral, usually MX_CAN_Init */
    uint8_t outStatusLEDGreen; // This will be updated by the stack - Use them for the LED management
    uint8_t outStatusLEDRed;   // This will be updated by the stack - Use them for the LED management
    CO_t* canOpenStack;
} CANopenNodeNXP;

// In order to use CANOpenNXP, you'll have it have a canopenNodeNXP structure somewhere in your codes, it is usually residing in CO_app_NXP.c
extern CANopenNodeNXP* canopenNodeNXP;

void gpt_timerInit(GPT_Type *base, gpt_output_compare_channel_t ch,uint32_t gptFreq);
/* This function will initialize the required CANOpen Stack objects, allocate the memory and prepare stack for communication reset */
int canopen_app_init(CANopenNodeNXP* canopenNXP);
/* This function will reset the CAN communication periperhal and also the CANOpen stack variables */
int canopen_app_resetCommunication(void);
/* This function will check the input buffers and any outstanding tasks that are not time critical,
 * this function should be called regurarly from your code (i.e from your while(1))
 */
void canopen_app_process(void);
/* Thread function executes in constant intervals, this function can be called from FreeRTOS tasks or Timers */
void canopen_app_interrupt(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CANOPENNODE_APP_NXP_H_ */
