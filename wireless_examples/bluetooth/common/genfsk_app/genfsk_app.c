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

#if defined(gAppEnableHybridGenfsk_d) && (gAppEnableHybridGenfsk_d == 1)
/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/* Framework / Drivers */
#include "EmbeddedTypes.h"
#include "fsl_component_timer_manager.h"
#include "fsl_component_mem_manager.h"

/* Application */
#include "genfsk_app.h"
#include "app_conn.h"

/* Controller */
#include "hybrid_gfsk.h"

/************************************************************************************
 *************************************************************************************
 * Private macros
 *************************************************************************************
 ************************************************************************************/
#define Hci_CommandPacket(pHciCmdPacket, parameterTotalLength)\
            Ble_HciSend(\
                gHciCommandPacket_c,\
                (void*)(pHciCmdPacket),\
                gHciCommandPacketHeaderLength_c + (parameterTotalLength))

#define HciCommand(opCodeGroup, opCodeCommand)\
    (((uint16_t)(opCodeGroup) & (uint16_t)0x3FU)<<(uint16_t)SHIFT10)|(uint16_t)((opCodeCommand) & 0x3FFU)

/* Opcodes for the HCI vendor genfsk commands */
#define mHciVendorLeGenfskTransmitCommand_c             0xFCB4U
#define mHciVendorLeGenfskReceiveCommand_c              0xFCB5U
#define mHciVendorLeGenfskCancelCommand_c               0xFCB6U

/* Commands length */
#define mHciVendorLeGenfskTransmitCommandLength_c       (0U) /* without payload */
#define mHciVendorLeGenfskReceiveCommandLength_c        (0U)
#define mHciVendorLeGenfskCancelCommandLength_c         (0U)

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
    mHciVendorLeGenfskTransmitCompleteEvent_c           = 0xA8,
    mHciVendorLeGenfskReceiveCompleteEvent_c            = 0xA9,
} hciVendorLeGenfskEventCode_tag;

/***************************************************************************************************
****************************************************************************************************
* Public memory declarations
****************************************************************************************************
***************************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
/* Timer used to restart the Tx procedure at a fixed interval */
static TIMER_MANAGER_HANDLE_DEFINE(mAppGenfskTmr);

/* Variables used to keep state of the Tx/Rx Genfsk Procedures */
static bool_t mbAppGenfskRxOn = FALSE;
static bool_t mbAppGenfskTxOn = FALSE;

/* Counter used in packet to keep track of packet loss */
static uint8_t localTxCounter = 0;

/* Pointer function used to communicate events to the application */
static gfskAppEventHandler_t mpfGfskAppEventCallback = NULL;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \fn             static bool_t hciGenfskEventHook(hciPacketType_t packetType, 
*                                                  void *pHciPacket,
*                                                  uint16_t packetSize)
*
* \brief          Host to Application interface function. The function builds the HCI Event
*                 from the HCI packet received for Genfsk.
*
* \param[in]      packetType    HCI Packet Type.
* \param[in]      pHciPacket    Pointer to packet payload.
* \param[in]      packetSize    Packet payload size.
*
* \return         TRUE if opCode was handled, FALSE otherwise
*
********************************************************************************** */
static bool_t hciGenfskEventHook
(
    const hciPacketType_t packetType,
    const void            *pHciPacket,
    const uint16_t        packetSize
);

/*! *********************************************************************************
* \fn           void GfskApp_TimerCallback(void *pParam)
*
* \brief        Timer callback function used tot switch to application task
*
* \param[in]    pParam                 callback function parameter.
*
* \return       void
*
********************************************************************************** */
static void GfskApp_TimerCallback(void *pParam);

/*! *********************************************************************************
* \fn           void GfskApp_Tx(void *payload)
*
* \brief        Controller to Host interface function. The function builds the HCI Command Complete Event
*               from the HCI packet received for Genfsk Commands.
*
* \param[in]    payload                 the address of data payload to be used in the genfsk packet.
*
* \return       void
*
********************************************************************************** */
static void GfskApp_Tx
(
    void *payload
);

/*! *********************************************************************************
* \fn           void GfskApp_EventHandler(void *pGfskAppData)
*
* \brief        Handle the Genfsk application events recieved from Host
*
* \param[in]    pGfskAppData            pointer to stucture of type pGfskAppData
*                                       containing event and event data.
*
* \return       void
*
********************************************************************************** */
static void GfskApp_EventHandler
(
    void *pGfskAppData
);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

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
void GfskApp_Init(gfskAppEventHandler_t pfGfskAppEventCallback)
{
    /* init pointer function for handling the genfsk events */
    (void)Hcit_RegisterGfskEventCallback(hciGenfskEventHook);

    /* Enable or disable hybrid GENFSK module */
    hybrid_gfsk_init();

    /* initialise the callback function for the application */
    if(mpfGfskAppEventCallback == NULL)
    {
        mpfGfskAppEventCallback = pfGfskAppEventCallback;
    }

    /* initialise the Tx timer */
    (void)TM_Open(mAppGenfskTmr);
}

/*! *********************************************************************************
* \fn             void GfskApp_StartTx(void)
*
* \brief          Start GFSK App timer for periodic TX.
*
* \return         void
*
********************************************************************************** */
void GfskApp_StartTx(void)
{
    (void)TM_InstallCallback((timer_handle_t)mAppGenfskTmr, (timer_callback_t)GfskApp_TimerCallback, NULL);
    (void)TM_Start((timer_handle_t)mAppGenfskTmr, (uint8_t)kTimerModeIntervalTimer, gGenFskApp_TxInterval_c);

    mbAppGenfskTxOn = TRUE;
}

/*! *********************************************************************************
* \fn             void GfskApp_StopTx(void)
*
* \brief          Cancel GFSK Tx Procedure.
*
* \return         void
*
********************************************************************************** */
void GfskApp_StopTx(void)
{
    if (TRUE == mbAppGenfskTxOn)
    {
        (void)TM_Stop(mAppGenfskTmr);

        mbAppGenfskTxOn = FALSE;

        /* send command via HCI */
        hciCommandPacket_t  hciPacket =
        {
            HciCommand(gHciVendorSpecificDebugCommands_c, mHciVendorLeGenfskCancelCommand_c),
            mHciVendorLeGenfskCancelCommandLength_c
        };

        (void)Hci_CommandPacket(&hciPacket, mHciVendorLeGenfskCancelCommandLength_c);
    }
}

/*! *********************************************************************************
* \fn             void GfskApp_StartRx(void)
*
* \brief          Start GFSK RX procedure
*
* \return         void
*
********************************************************************************** */
void GfskApp_StartRx(void)
{
    /* send command via HCI */
    hciCommandPacket_t  hciPacket =
    {
        HciCommand(gHciVendorSpecificDebugCommands_c, mHciVendorLeGenfskReceiveCommand_c),
        mHciVendorLeGenfskReceiveCommandLength_c
    };

    (void)Hci_CommandPacket(&hciPacket, mHciVendorLeGenfskReceiveCommandLength_c);
}

/*! *********************************************************************************
* \fn             void GfskApp_StopRx(void)
*
* \brief          Stop GFSK RX procedure
*
* \return         void
*
********************************************************************************** */
void GfskApp_StopRx(void)
{
    mbAppGenfskRxOn = FALSE;

    /* send command via HCI */
    hciCommandPacket_t  hciPacket =
    {
        HciCommand(gHciVendorSpecificDebugCommands_c, mHciVendorLeGenfskCancelCommand_c),
        mHciVendorLeGenfskCancelCommandLength_c
    };

    (void)Hci_CommandPacket(&hciPacket, mHciVendorLeGenfskCancelCommandLength_c);
}


/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \fn             static bool_t hciGenfskEventHook(hciPacketType_t packetType, 
*                                                  void *pHciPacket,
*                                                  uint16_t packetSize)
*
* \brief          Host to Application interface function. The function builds the HCI Event
*                 from the HCI packet received for Genfsk.
*
* \param[in]      packetType    HCI Packet Type.
* \param[in]      pHciPacket    Pointer to packet payload.
* \param[in]      packetSize    Packet payload size.
*
* \return         TRUE if opCode was handled, FALSE otherwise
*
********************************************************************************** */
static bool_t hciGenfskEventHook
(
    const hciPacketType_t packetType,
    const void            *pHciPacket,
    const uint16_t        packetSize
)
{
    bool_t found = TRUE;

    if (packetType == gHciEventPacket_c)
    {
        hciEventCode_t  eventCode;
        uint8_t parLength;
        const uint8_t *pPacket = pHciPacket;

        /* MISRA Rule 13.1 - Initializer lists shall not contain persistent side effects */
        eventCode = *pPacket;
        pPacket++;
        parLength = *pPacket;
        pPacket++;

        switch(eventCode)
        {
            case (uint8_t)gHciCommandCompleteEvent_c:
            {
                union
                {
                    bleResult_t result;
                    uint8_t     hciStatus;
                } temp = {gBleStatusBase_c};

                hciErrorCode_t hciStatus;
                uint8_t numHciCommandPackets;
                hciCommandOpCode_t commandOpCode;

                numHciCommandPackets = *pPacket;
                pPacket++;
                (void)numHciCommandPackets;

                commandOpCode.opCode = Utils_ExtractTwoByteValue((const uint8_t *)pPacket);
                pPacket = &pPacket[2];

                if (*pPacket != 0U)
                {
                    /* Valid ONLY for Command Complete Events */
                    temp.result = gHciStatusBase_c;
                    temp.hciStatus = (uint8_t)*pPacket;
                    hciStatus = temp.result;
                }
                else
                {
                    hciStatus = gHciSuccess_c;
                }
                pPacket++;

                switch (commandOpCode.opCode)
                {
                    case HciCmdOpcode(gHciVendorSpecificDebugCommands_c, mHciVendorLeGenfskTransmitCommand_c):
                    {
                        gfskAppEventData_t *pAppData = MEM_BufferAlloc(sizeof(gfskAppEventData_t));

                        if (pAppData != NULL)
                        {
                            pAppData->appEvent = gGfskEvt_CommandCompleteTransmit_c;
                            pAppData->eventData.cmdStatus = hciStatus;
                        }

                        (void)App_PostCallbackMessage(GfskApp_EventHandler, (void*)pAppData);
                    }
                    break;

                    case HciCmdOpcode(gHciVendorSpecificDebugCommands_c, mHciVendorLeGenfskReceiveCommand_c):
                    {
                        gfskAppEventData_t *pAppData = MEM_BufferAlloc(sizeof(gfskAppEventData_t));

                        if (pAppData != NULL)
                        {
                            pAppData->appEvent = gGfskEvt_CommandCompleteReceive_c;
                            pAppData->eventData.cmdStatus = hciStatus;
                        }

                        (void)App_PostCallbackMessage(GfskApp_EventHandler, (void*)pAppData);
                    }
                    break;

                    case HciCmdOpcode(gHciVendorSpecificDebugCommands_c, mHciVendorLeGenfskCancelCommand_c):
                    {
                        gfskAppEventData_t *pAppData = MEM_BufferAlloc(sizeof(gfskAppEventData_t));

                        if (pAppData != NULL)
                        {
                            pAppData->appEvent = gGfskEvt_CommandCompleteCancel_c;
                            pAppData->eventData.cmdStatus = hciStatus;
                        }

                        (void)App_PostCallbackMessage(GfskApp_EventHandler, (void*)pAppData);
                    }
                    break;

                    default:
                    {
                        found = FALSE;
                    }
                    break;
                }
            }
            break;

            case (uint8_t)gHciVendorHandoverMetaEvent_c:
            {
                uint8_t eventType  = *pPacket;
                pPacket++;

                switch(eventType)
                {
                    case (uint8_t)mHciVendorLeGenfskTransmitCompleteEvent_c:
                    {
                        gfskAppEventData_t *pAppData = MEM_BufferAlloc(sizeof(gfskAppEventData_t));

                        if (pAppData != NULL)
                        {
                            pAppData->appEvent = gGfskEvt_MetaEventTransmitComplete_c;
                        }

                        (void)App_PostCallbackMessage(GfskApp_EventHandler, (void*)pAppData);
                    }
                    break;

                    case (uint8_t)mHciVendorLeGenfskReceiveCompleteEvent_c:
                    {
                        gfskAppEventData_t *pAppData = MEM_BufferAlloc(sizeof(gfskAppEventData_t) + ((uint32_t)parLength - 3U));

                        if (pAppData != NULL)
                        {
                            pAppData->appEvent = gGfskEvt_MetaEventReceiveComplete_c;

                            pAppData->eventData.transmitCompleteData.status = *pPacket;
                            pPacket++;
                            pAppData->eventData.transmitCompleteData.RSSI = *pPacket;
                            pPacket++;
                            pAppData->eventData.transmitCompleteData.payloadLength = 
                                parLength - 3U; /* Substract Event Code; Param total Length; Subevent Code */

                            FLib_MemCpy(pAppData->eventData.transmitCompleteData.payload, pPacket, (uint32_t)parLength - 3U);
                        }

                        (void)App_PostCallbackMessage(GfskApp_EventHandler, (void*)pAppData);
                    }
                    break;

                    default:
                    {
                        found = FALSE;
                    }
                    break;
                }
            }
            break;

            default:
            {
                found = FALSE;
            }
            break;
        }
    }
    else
    {
        found = FALSE;
    }

    return found;
}

/*! *********************************************************************************
* \fn           void GfskApp_TimerCallback(void *pParam)
*
* \brief        Timer callback function used tot switch to application task
*
* \param[in]    pParam                 callback function parameter.
*
* \return       void
*
********************************************************************************** */
static void GfskApp_TimerCallback(void *pParam)
{
    /* Switch to application task */
    (void)App_PostCallbackMessage(GfskApp_Tx, pParam);
}

/*! *********************************************************************************
* \fn           void GfskApp_Tx(void *payload)
*
* \brief        Controller to Host interface function. The function builds the HCI Command Complete Event
*               from the HCI packet received for Genfsk Commands.
*
* \param[in]    payload                 the address of data payload to be used in the genfsk packet.
* \param[in]    payloadLength           the length of the payload.
*
* \return       void
*
********************************************************************************** */
static void GfskApp_Tx(void *payload)
{
    /* Not used. The demo will send the packet from bellow */
    (void)payload;

    /* Dummy packet */
    uint8_t aMessage[] = {
        gGenFSK_NetworkAddress_c, /* Network address used. */
        gGenFSK_H0Value_c,        /* H0 Value.*/
        0x00,                     /* Payload Length */
        gGenFSK_Identifier_c,     /* Identifier Address */
        0x02, 0x01, 0x06, 0x09, 0x08, 0x47, 0x46, 0x53, 0x4B, 0x5F, 0x41, 0x44,
                                  /* Raw data: flags, device name (GFSK_AD) */
        0x00                      /* One dynamic octet counter */
    };

    /* Subtract the header to obtain the payload length */
    aMessage[5] = sizeof(aMessage) - gGenFSK_TxHeaderSize_c;

    /* Include a counter at the end to monitor the packet number at rx */
    aMessage[sizeof(aMessage) - 1U] = localTxCounter++;

    /* Send packet via HCI */
    uint8_t pHciPacket[mHciVendorLeGenfskTransmitCommandLength_c + sizeof(aMessage) + gHciCommandPacketHeaderLength_c];
    uint8_t *pPacketData = (uint8_t *)&pHciPacket[3];

    uint16_t temp = HciCommand(gHciVendorSpecificDebugCommands_c, mHciVendorLeGenfskTransmitCommand_c);
    FLib_MemCpy((void *)pHciPacket, (const void *)&temp, 2U);
    pHciPacket[2] = (uint8_t)mHciVendorLeGenfskTransmitCommandLength_c + sizeof(aMessage);

    FLib_MemCpy(pPacketData, aMessage, sizeof(aMessage));

    (void)Hci_CommandPacket(pHciPacket, (uint16_t)pHciPacket[2]);
}

/*! *********************************************************************************
* \fn           void GfskApp_EventHandler(void *pGfskAppData)
*
* \brief        Handle the Genfsk application events recieved from Host
*
* \param[in]    pGfskAppData            pointer to stucture of type pGfskAppData
*                                       containing event and event data.
*
* \return       void
*
********************************************************************************** */
static void GfskApp_EventHandler(void *pGfskAppData)
{
    if (pGfskAppData != NULL)
    {
        gfskAppEventData_t *pAppData = (gfskAppEventData_t*)pGfskAppData;

        switch (pAppData->appEvent)
        {
            case gGfskEvt_MetaEventTransmitComplete_c:
            {
                /* No internal action needed */
            }
            break;

            case gGfskEvt_MetaEventReceiveComplete_c:
            {
                if (TRUE == mbAppGenfskRxOn)
                {
                    /* Restart RX asap */
                    GfskApp_StartRx();
                }
            }
            break;

            case gGfskEvt_CommandCompleteTransmit_c:
            {
                /* No internal action needed */
            }
            break;

            case gGfskEvt_CommandCompleteReceive_c:
            {
                if (gHciSuccess_c == pAppData->eventData.cmdStatus)
                {
                    mbAppGenfskRxOn = TRUE;
                }
                else
                {
                    mbAppGenfskRxOn = FALSE;
                }
            }
            break;

            case gGfskEvt_CommandCompleteCancel_c:
            {
                /* No internal action needed */
            }
            break;

            default:
            {
                /* MISRA */
            }
            break;
        }

        if (mpfGfskAppEventCallback != NULL)
        {
            mpfGfskAppEventCallback(pAppData);
        }

        /* pGfskAppData checked against NULL at start of function */
        (void)MEM_BufferFree(pGfskAppData);
    }
}
#endif /* defined(gAppEnableHybridGenfsk_d) && (gAppEnableHybridGenfsk_d == 1) */

/*! *********************************************************************************
* @}
********************************************************************************** */
