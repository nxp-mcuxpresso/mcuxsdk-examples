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

#ifdef __cplusplus
extern "C" {
#endif

#if ((defined(FSL_FEATURE_FLEXCAN_HAS_ERRATA_5641) && FSL_FEATURE_FLEXCAN_HAS_ERRATA_5641) || \
     (defined(FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829) && FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829))
#define TX_MB_FIRST_INDEX 9
#define TX_MB_LAST_INDEX  15
#else
#define TX_MB_FIRST_INDEX 8
#define TX_MB_LAST_INDEX  15
#endif
#define RX_MESSAGE_COUNT  1

typedef struct {
    /* This is the Node ID that you ask the CANOpen stack to assign to your device, although it might not always be the final NodeID,
     * after calling canopen_app_init() you should check ActiveNodeID of CANopenNodeNXP structure for assigned Node ID.
     */
    uint8_t desiredNodeID;
    uint8_t activeNodeID;   /* Assigned Node ID */
    uint16_t baudrate;      /* This is the baudrate you've set in FlexCAN Configuration */
    CAN_Type *CANBasePtr;   /* FlexCAN peripheral base address. */
    void (*HWInitFunction)(void); /* Pass in the function that initialize the CAN peripheral. */
    void (*startTimer)(void);
    void (*stopTimer)(void);
    uint8_t outStatusLEDGreen; /* This will be updated by the stack - Use them for the LED management */
    uint8_t outStatusLEDRed;   /* This will be updated by the stack - Use them for the LED management */
    CO_t* canOpenStack;
} CANopenNodeNXP;

/* In order to use CANOpenNXP, you'll have it have a canopenNodeNXP structure somewhere in your codes, it is usually residing in CO_app_NXP.c */
extern CANopenNodeNXP* canopenNodeNXP;

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

char *getNMT_string(CO_NMT_internalState_t state);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CANOPENNODE_APP_NXP_H_ */
