/*! *********************************************************************************
 * \addtogroup GenFSK Application Module
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2024 NXP
*
*
* \file
*
* This file is the source file for the GenFSK Application Module
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef BLE_GENFSK_APPLICATION_H
#define BLE_GENFSK_APPLICATION_H

#if defined(gAppEnableHybridGenfsk_d) && (gAppEnableHybridGenfsk_d == 1)
/************************************************************************************
 *************************************************************************************
 * Include
 *************************************************************************************
 ************************************************************************************/

/* BLE Host Stack */
#include "hci_types.h"

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/
#define gGenFSK_NetworkAddress_c                        0xD6, 0xBE, 0x89, 0x8E
#define gGenFSK_H0Value_c                               0x42
#define gGenFSK_Identifier_c                            0x09, 0x09, 0x09, 0x09, 0x09, 0x09

#define gGenFSK_TxHeaderSize_c                          (6U) /* Network Addres(4 bytes); H0Value(1 byte); Size value itself (1 byte)*/
#define gGenFSK_RxHeaderSize_c                          (2U) /* H0Value(1 byte); Size value itself (1 byte)*/

#define gGenFskApp_TxInterval_c                         (1000U) /* [ms] */

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef enum gfskAppEvent_tag{
    gGfskEvt_MetaEventTransmitComplete_c,
    gGfskEvt_MetaEventReceiveComplete_c,
    gGfskEvt_CommandCompleteTransmit_c,
    gGfskEvt_CommandCompleteReceive_c,
    gGfskEvt_CommandCompleteCancel_c,
    gGfskEvt_Unknown_c,
} gfskAppEvent_t;

typedef struct appGfskTransmitCompleteEvent_tag
{
    uint8_t status;                     /*!< Genfsk Status, bit 0: 0/1 for CRC valid/error, bit 1: 0/1 for length valid/error */
    uint8_t RSSI;                       /*!< Genfsk RSSI */
    uint8_t payloadLength;              /*!< Genfsk payload length, max 252 */
    uint8_t payload[1];                 /*!< Genfsk payload, max payload length 252 bytes*/
} appGfskTransmitCompleteEvent_t;

typedef struct gfskAppEventData_tag
{
    gfskAppEvent_t      appEvent;                                       /*!< Event type. */
    union {
        bleResult_t                     cmdStatus;
        appGfskTransmitCompleteEvent_t  transmitCompleteData;
    } eventData;                        /*!< Event data, selected according to event type. */
} gfskAppEventData_t;

typedef void (*gfskAppEventHandler_t) (const void *pGfskAppEvent);

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern bleResult_t Ble_HciSend(hciPacketType_t packetType, void* pPacket, uint16_t packetSize);

/*! *********************************************************************************
* \fn           void GfskApp_Init(gfskAppEventHandler_t pfGfskAppEventCallback)
*
* \brief        Initialize GFSK module
*
* \param[in]    pfGfskAppEventCallback  pointer to function to be called by Genfsk module
*                                       to handle an application event.
*
* \return       void
*
********************************************************************************** */
void GfskApp_Init(gfskAppEventHandler_t pfGfskAppEventCallback);

/*! *********************************************************************************
* \fn             void GfskApp_StartTx(void)
*
* \brief          Start GFSK App timer for periodic TX.
*
* \return         void
*
********************************************************************************** */
void GfskApp_StartTx(void);

/*! *********************************************************************************
* \fn             void GfskApp_StopTx(void)
*
* \brief          Cancel GFSK Tx Procedure.
*
* \return         void
*
********************************************************************************** */
void GfskApp_StopTx(void);

/*! *********************************************************************************
* \fn             void GfskApp_StartRx(void)
*
* \brief          Start GFSK RX procedure
*
* \return         void
*
********************************************************************************** */
void GfskApp_StartRx(void);

/*! *********************************************************************************
* \fn             void GfskApp_StopRx(void)
*
* \brief          Stop GFSK RX procedure
*
* \return         void
*
********************************************************************************** */
void GfskApp_StopRx(void);

#ifdef __cplusplus
}
#endif 

#endif /* defined(gAppEnableHybridGenfsk_d) && (gAppEnableHybridGenfsk_d == 1) */

#endif /* BLE_GENFSK_APPLICATION_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
