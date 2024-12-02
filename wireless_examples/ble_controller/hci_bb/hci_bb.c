/*! *********************************************************************************
* Copyright 2021-2024 NXP
* All rights reserved.
*
* This file is the source file for the hci black box application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "fsl_component_mem_manager.h"
#include "fsl_component_serial_manager.h"

#include "app.h"
#include "fwk_platform.h"
#include "fwk_platform_ble.h"

#include "controller_interface.h"
#include "controller_api.h"
#include "fwk_platform_ics.h"

#include "FunctionLib.h"

#include "ble_general.h"
#define PKT_THRESHOLD_TO_DROP_ADV_REPORT 64U
#define HCI_PKT_TYPE_EVENT 4U
#define HCI_META_EVENT_TYPE 0x3EU
#define HCI_ADV_REPORT_SUB_EVENT 0x2U
#define HCI_EXT_ADV_REPORT_SUB_EVENT 0xDU

// list of HCI commands to process in hci_bb app
#define HCI_CTRL_API_OPCODE                    0xFDAEU
#define HCI_HW_RESET_OPCODE                    0xFDAFU

#if defined(HCI_CTRL_API_OPCODE) || defined(HCI_HW_RESET_OPCODE)
#define ENABLE_HCI_CMD_HOOK
#endif

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef uint8_t hciEventCode_t;

typedef enum{
    mDetectMarker_c       = 0,
    mDetectHeader_c,
    mPacketInProgress_c
}detectState_t;

typedef PACKED_STRUCT hciCommandPacketHeader_tag
{
    uint16_t    opCode;
    uint8_t     parameterTotalLength;
}hciCommandPacketHeader_t;

typedef PACKED_STRUCT hciAclDataPacketHeader_tag
{
    uint16_t    handle      :12;
    uint16_t    pbFlag      :2;
    uint16_t    bcFlag      :2;
    uint16_t    dataTotalLength;
}hciAclDataPacketHeader_t;

typedef PACKED_STRUCT hciEventPacketHeader_tag
{
    hciEventCode_t  eventCode;
    uint8_t     dataTotalLength;
}hciEventPacketHeader_t;

typedef PACKED_STRUCT hcitPacketHdr_tag
{
    hciPacketType_t packetTypeMarker;
    PACKED_UNION
    {
        hciAclDataPacketHeader_t    aclDataPacket;
        hciEventPacketHeader_t      eventPacket;
        hciCommandPacketHeader_t    commandPacket;
    };
}hcitPacketHdr_t;

/* Hci packets header lengths */
#define gHciCommandPacketHeaderLength_c     (3U)
#define gHciAclDataPacketHeaderLength_c     (4U)
#define gHciEventPacketHeaderLength_c       (2U)

#define gHcLeAclDataPacketLengthDefault_c   (500U - gHciAclDataPacketHeaderLength_c)
#define gHcitMaxPayloadLen_c    (gHcLeAclDataPacketLengthDefault_c + gHciAclDataPacketHeaderLength_c)

typedef PACKED_STRUCT hcitPacketStructured_tag
{
    hcitPacketHdr_t header;
    uint8_t         payload[gHcitMaxPayloadLen_c];
} hcitPacketStructured_t;

typedef PACKED_UNION hcitPacket_tag
{
    /* The entire packet as unformatted data. */
    uint8_t raw[sizeof(hcitPacketStructured_t)];
}hcitPacket_t;


typedef struct hcitComm_tag
{
    hcitPacket_t        *pPacket;
    hcitPacketHdr_t     pktHeader;
    uint16_t            bytesReceived;
    uint16_t            expectedLength;
}hcitComm_t;

typedef bleResult_t (* hciTransportInterface_t)
(
    hciPacketType_t packetType,     /*!< HCI Packet Type. */
    void* pPacket,                  /*!< Pointer to packet payload. */
    uint16_t packetSize             /*!< Packet payload size. */
);

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static uint8_t platformInitialized = 0;

/*application serial manager uart handle*/
SERIAL_MANAGER_READ_HANDLE_DEFINE(g_appUartReadHandle);
SERIAL_MANAGER_WRITE_HANDLE_DEFINE(g_appUartWriteHandle);

static hcitComm_t               mHcitData;
static detectState_t            mPacketDetectStep;
static detectState_t            mPacketDetectStep;
static hcitPacket_t             mHcitPacketRaw;
static uint16_t                 nb_pkt_in_uart_tx_queue = 0;

#ifdef ENABLE_HCI_CMD_HOOK
// HCI command buffer
static uint8_t                  maPendingHciCmd[1+2+1+255];
#endif

// Defining HCIBB_ENABLE_DEBUG_FEATURES will add debug features that can be enabled via HCI proprietary commands
#ifdef HCIBB_ENABLE_DEBUG_FEATURES
#include "hci_bb_debug.h"
#endif

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void HCI_AppControllerRxCallback(uint8_t packetType, uint8_t *data, uint16_t len);

/************************************************************************************
*************************************************************************************
* Public functions prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
#if (!defined(gAppUseDtm2Wire) || (gAppUseDtm2Wire == 0))
static void Hcit_SerialFreePacket(void *pPacket,
                                  serial_manager_callback_message_t *message,
                                  serial_manager_status_t status)
{
    SerialManager_CloseWriteHandle((serial_write_handle_t)pPacket);
    (void)MEM_BufferFree(pPacket);
    uint32_t regPrimask = DisableGlobalIRQ();
    if (nb_pkt_in_uart_tx_queue > 0U)
    {
      nb_pkt_in_uart_tx_queue--;
    }
    EnableGlobalIRQ(regPrimask);
}
#endif

static void Hcit_SendMessageToController(void)
{

    uint8_t*        pSerialPacket = NULL;
    bleResult_t     result = gBleSuccess_c;
    uint32_t        packet_Size = mHcitData.bytesReceived + 1;

    pSerialPacket = MEM_BufferAlloc(packet_Size);

    if (pSerialPacket == NULL)
    {
        result = gBleOutOfMemory_c;
        assert(0);
    }
    else
    {
        pSerialPacket[0] = mHcitData.pktHeader.packetTypeMarker;
        FLib_MemCpy(&pSerialPacket[1],(uint8_t *)mHcitData.pPacket,  mHcitData.bytesReceived);

#ifdef ENABLE_HCI_CMD_HOOK
        uint16_t opcode = pSerialPacket[1]+pSerialPacket[2]*256;
        if( pSerialPacket[0] == gHciCommandPacket_c && (
#ifdef HCI_CTRL_API_OPCODE
             opcode == HCI_CTRL_API_OPCODE ||
#endif
#ifdef HCI_HW_RESET_OPCODE
             opcode == HCI_HW_RESET_OPCODE ||
#endif
             0))
        {
            if( maPendingHciCmd[0] == 0 )
            {
                // cannot handle here as it is in the IRQ handler
                memcpy(maPendingHciCmd, pSerialPacket, 4+pSerialPacket[3]);
            }
        }
        else
#endif // ENABLE_HCI_CMD_HOOK

        PLATFORM_SendHciMessage(pSerialPacket, packet_Size);

        MEM_BufferFree(pSerialPacket);
    }

    mPacketDetectStep = mDetectMarker_c;
    (void)result;
}

void HCI_AppUartRxCallback(void *callbackParam,
                           serial_manager_callback_message_t *message,
                           serial_manager_status_t status)
{
    uint8_t         recvChar;

    uint32_t        count = 0U;
    if(kStatus_SerialManager_Success != SerialManager_TryRead( (serial_read_handle_t)g_appUartReadHandle, &recvChar, 1, &count))

    {
        return;
    }

    while( count != 0U )
    {
        switch( mPacketDetectStep )
        {
            case mDetectMarker_c:
                if( (recvChar == (uint8_t)gHciDataPacket_c) ||
                    (recvChar == (uint8_t)gHciEventPacket_c) ||
                    (recvChar == (uint8_t)gHciCommandPacket_c) )
                {
                    union
                    {
                        hcitPacketHdr_t *pPacketHdr;
                        hcitPacket_t    *pPacket;
                    } packetTemp; /* MISRA rule 11.3 */

                    packetTemp.pPacketHdr = &mHcitData.pktHeader;
                    mHcitData.pPacket = packetTemp.pPacket;

                    mHcitData.pktHeader.packetTypeMarker = (hciPacketType_t)recvChar;
                    mHcitData.bytesReceived = 1;

                    mPacketDetectStep = mDetectHeader_c;
                }
                break;

            case mDetectHeader_c:
                mHcitData.pPacket->raw[mHcitData.bytesReceived++] = recvChar;

                switch( mHcitData.pktHeader.packetTypeMarker )
                {
                    case gHciDataPacket_c:
                        /* ACL Data Packet */
                        if( mHcitData.bytesReceived == (gHciAclDataPacketHeaderLength_c + 1U) )
                        {
                            /* Validate ACL Data packet length */
                            if( mHcitData.pktHeader.aclDataPacket.dataTotalLength > gHcLeAclDataPacketLengthDefault_c )
                            {
                                mHcitData.pPacket = NULL;
                                mPacketDetectStep = mDetectMarker_c;
                                break;
                            }
                            mHcitData.expectedLength = gHciAclDataPacketHeaderLength_c +
                                                       mHcitData.pktHeader.aclDataPacket.dataTotalLength;

                            mPacketDetectStep = mPacketInProgress_c;
                        }
                        break;

                    case gHciEventPacket_c:
                        /* HCI Event Packet */
                        if( mHcitData.bytesReceived == (gHciEventPacketHeaderLength_c + 1U) )
                        {
                            /* Validate HCI Event packet length
                            if( mHcitData.pktHeader.eventPacket.dataTotalLength > gHcEventPacketLengthDefault_c )
                            {
                                mHcitData.pPacket = NULL;
                                mPacketDetectStep = mDetectMarker_c;
                                break;
                            } */
                            mHcitData.expectedLength = gHciEventPacketHeaderLength_c +
                                                       (uint16_t)mHcitData.pktHeader.eventPacket.dataTotalLength;
                            mPacketDetectStep = mPacketInProgress_c;
                        }
                        break;

                    case gHciCommandPacket_c:
                        /* HCI Command Packet */
                        if( mHcitData.bytesReceived == (gHciCommandPacketHeaderLength_c + 1U) )
                        {

                            mHcitData.expectedLength = gHciCommandPacketHeaderLength_c +
                                                       (uint16_t)mHcitData.pktHeader.commandPacket.parameterTotalLength;
                            mPacketDetectStep = mPacketInProgress_c;
                        }
                        break;
                    case gHciSynchronousDataPacket_c:
                    default:
                        ; /* Not Supported */
                        break;
                }

                if( mPacketDetectStep == mPacketInProgress_c )
                {
                    mHcitData.pPacket = &mHcitPacketRaw;
                    FLib_MemCpy(mHcitData.pPacket, (uint8_t*)&mHcitData.pktHeader + 1, sizeof(hcitPacketHdr_t) - 1U);
                    mHcitData.bytesReceived -= 1U;

                    if( mHcitData.bytesReceived == mHcitData.expectedLength )
                    {
                        Hcit_SendMessageToController();
                    }
                }
                break;

            case mPacketInProgress_c:
                mHcitData.pPacket->raw[mHcitData.bytesReceived++] = recvChar;

                if( mHcitData.bytesReceived == mHcitData.expectedLength )
                {
                    Hcit_SendMessageToController();
                }
                break;

            default:
                ; /* No action required */
                break;
        }

        if(kStatus_SerialManager_Success != SerialManager_TryRead( (serial_read_handle_t)g_appUartReadHandle, &recvChar, 1, &count))
        {
            return;
        }
    }
}

#if (!defined(gAppUseDtm2Wire) || (gAppUseDtm2Wire == 0))
#if (defined(gEnableCoverage) && (gEnableCoverage == 1))
#include "fsl_debug_console.h"

#define OPEN_APPEND_CMD_PACKET_TYPE             5U
#define PUTS_CMD_PACKET_TYPE                    6U
#define CLOSE_CMD_PACKET_TYPE                   7U
#define MALLOC_FAILURE_PACKET_TYPE              8U

#define __SECTION(x) __attribute__((section(x)))
bool __SECTION(".coverage") is_buffer_freed;
static bool is_open_append = FALSE;

static bool handle_coverage_cmds_from_controller(uint8_t packetType, uint8_t *data, uint32_t len)
{
  bool is_coverage_cmd = FALSE;

  /*Check packet type received from controller*/
  if (packetType >= OPEN_APPEND_CMD_PACKET_TYPE)
  {
    is_coverage_cmd = TRUE;

    switch (packetType)
    {
      case OPEN_APPEND_CMD_PACKET_TYPE:
      {
        if (!is_open_append)
        {
          PRINTF("%s",data);
          is_open_append = TRUE;
        }
        else
        {
          PRINTF("%s\n",data);
          is_open_append = FALSE;
        }
        break;
      }
      case PUTS_CMD_PACKET_TYPE:
      {
        /*Extract non-null terminated string from packet*/
        for (int i = 0; i < len; i++)
        {
          PUTCHAR(data[i]);
        }
        break;
      }
      case CLOSE_CMD_PACKET_TYPE:
      {
        PRINTF("%s\n",data);
        break;
      }
      case MALLOC_FAILURE_PACKET_TYPE:
      {
        int size = (data[3] << 24U) | (data[2] << 16U) | (data[1] << 8U) | data[0];
        PRINTF("ERROR: Coverage scanner failed to allocate %d bytes on NBU! Increase size (x) passed in the linker option --cs-memory-pool=x on NBU side.\n",size);
        break;
      }
      default:
      {
        /*Not expected*/
        assert(FALSE);
      }
    }
    is_buffer_freed = TRUE;
  }
  return is_coverage_cmd;
}
#endif /*(defined(gEnableCoverage) && (gEnableCoverage == 1))*/

static void HCI_AppControllerRxCallback(uint8_t packetType, uint8_t *data, uint16_t len)
{
    uint8_t* pSerialPacket = NULL;
    uint8_t* pPacketBuffer;
    uint8_t* hciWriteHandle;

#if (defined(gEnableCoverage) && (gEnableCoverage == 1))
    if (!handle_coverage_cmds_from_controller(packetType, data, len))
#endif /*(defined(gEnableCoverage) && (gEnableCoverage == 1))*/
    {
        if (nb_pkt_in_uart_tx_queue > PKT_THRESHOLD_TO_DROP_ADV_REPORT)
        {
            // check if the packet is adv report
            if ((packetType == HCI_PKT_TYPE_EVENT) && (data[0] == HCI_META_EVENT_TYPE)
                && ((data[2] == HCI_EXT_ADV_REPORT_SUB_EVENT) || (data[2] == HCI_ADV_REPORT_SUB_EVENT)))
            {
                return;
            }
        }
        /* Increase by 1 the lenght to insert the packet type */
        pPacketBuffer = MEM_BufferAlloc(SERIAL_MANAGER_WRITE_HANDLE_SIZE + len + 1);
        if (pPacketBuffer == NULL)
        {
            return;
        }
        hciWriteHandle   = pPacketBuffer;
        pSerialPacket    = pPacketBuffer + SERIAL_MANAGER_WRITE_HANDLE_SIZE;
        pSerialPacket[0] = packetType;

        FLib_MemCpy(&pSerialPacket[1], (uint8_t*)data, len);
        (void)SerialManager_OpenWriteHandle((serial_handle_t)gSerMgrIf, (serial_write_handle_t)hciWriteHandle);
        (void)SerialManager_InstallTxCallback((serial_write_handle_t)hciWriteHandle, Hcit_SerialFreePacket, pPacketBuffer);
        uint32_t regPrimask = DisableGlobalIRQ();
        nb_pkt_in_uart_tx_queue++;
        EnableGlobalIRQ(regPrimask);
        if ( kStatus_SerialManager_Success != SerialManager_WriteNonBlocking((serial_write_handle_t)hciWriteHandle, pSerialPacket, len + 1) )
        {
            SerialManager_CloseWriteHandle((serial_write_handle_t)pPacketBuffer);
            (void)MEM_BufferFree(pPacketBuffer);
            uint32_t regPrimask = DisableGlobalIRQ();
            if (nb_pkt_in_uart_tx_queue > 0U)
            {
              nb_pkt_in_uart_tx_queue--;
            }
            EnableGlobalIRQ(regPrimask);
        }
    }
}
#else
#include "dtm.h"
#endif /*(!defined(gAppUseDtm2Wire) || (gAppUseDtm2Wire == 0))*/

/*! *********************************************************************************
* \brief  This is the first task created by the OS. This task will initialize
*         the system
*
* \param[in]  param
*
********************************************************************************** */

void main_task(uint32_t param)
{
    if (!platformInitialized)
    {
        platformInitialized = 1;

        /* Framework init */
        MEM_Init();

#if (!defined(gAppUseDtm2Wire) || (gAppUseDtm2Wire == 0))
        /*open application uart write/read handle, install rx callback*/
        if ( kStatus_SerialManager_Success != SerialManager_OpenWriteHandle((serial_handle_t)gSerMgrIf, (serial_write_handle_t)g_appUartWriteHandle))
        {
            return;
        }

        if ( kStatus_SerialManager_Success != SerialManager_OpenReadHandle((serial_handle_t)gSerMgrIf, (serial_read_handle_t)g_appUartReadHandle))
        {
            return;
        }
        else
        {
            if ( kStatus_SerialManager_Success !=  SerialManager_InstallRxCallback((serial_write_handle_t)g_appUartReadHandle, HCI_AppUartRxCallback, NULL))
            {
                return;
            }
        }

        PLATFORM_SetHciRxCallback(HCI_AppControllerRxCallback);

#else
        if (gBleSuccess_c != DTM_Init((serial_handle_t) gSerMgrIf))
        {
            return;
        }
#endif /*(!defined(gAppUseDtm2Wire) || (gAppUseDtm2Wire == 0))*/

#if defined(gAppConfigureFEM) && (gAppConfigureFEM == 1)
    {
        extern osa_status_t Controller_ConfigureFEM(const uint8_t *config_ptr, uint8_t config_len);
//        uint8_t FEM_config[16U] = {               /* see default_FEM_config for details */
//          1U/*dual mode*/, 0U/*Disabled*/, 1U/*Enabled*/, 0U/*Disabled*/, 0U/*Disabled*/, 0U/*TSM GPIO*/, 0U/*TSM GPIO*/,
//          0U/*TSM GPIO*/, 0U/*TSM GPIO*/, 0U/*?*/, 0U/*MUST=0*/, 0U/*?*/, 0U/*MUST=0*/, 0U/*HIGH*/, 0U/*HIGH*/, 0U/*RF_GPO[3:0]*/};
//        osa_status_t FEM_status = Controller_ConfigureFEM(FEM_config, sizeof(FEM_config));
        osa_status_t FEM_status = Controller_ConfigureFEM(NULL, 0U /*N/A*/);   /* use default configuration */
        assert(FEM_status==KOSA_StatusSuccess);
    }
#endif
#if defined(gAppConfigureCOEX) && (gAppConfigureCOEX == 1)
    {
        extern osa_status_t Controller_ConfigureCOEX(const uint8_t *config_ptr, uint8_t config_len);
//        uint8_t COEX_config[16U] = {               /* see default_COEX_config for details */
//            0x01,0x01,0x01,0x00,0x01, 0x01, 0x00,0x00,0x00,0x00,0x00, 0x01,0x01, 0x96,0x64,0x59, 0x00,0x00, 0x59, 0x59,0x59,0x00, 0x01,0x01};
//        osa_status_t COEX_status = Controller_ConfigureCOEX(COEX_config, sizeof(COEX_config));
        osa_status_t COEX_status = Controller_ConfigureCOEX(NULL, 0U /*N/A*/);   /* use default configuration */
        assert(COEX_status==KOSA_StatusSuccess);
    }
#endif
        Controller_SetTxPowerLevelDbm(mAdvertisingDefaultTxPower_c, gAdvTxChannel_c);
        Controller_SetTxPowerLevelDbm(mConnectionDefaultTxPower_c, gConnTxChannel_c);
    }

#ifdef ENABLE_HCI_CMD_HOOK
    if( maPendingHciCmd[0] == gHciCommandPacket_c )
    {
        uint16_t opcode = maPendingHciCmd[1]+maPendingHciCmd[2]*256;
#ifdef HCI_HW_RESET_OPCODE
        if(opcode == HCI_HW_RESET_OPCODE)
        {
            // the below function will reset the system and not return
            __NVIC_SystemReset();
        }
        else
#endif // HCI_HW_RESET_OPCODE
#ifdef HCI_CTRL_API_OPCODE
        if(opcode == HCI_CTRL_API_OPCODE)
        {
          uint8_t nb_param = (maPendingHciCmd[3]-3U) / 5U; // each param has 1 byte specified and 4 bytes data
            uint8_t status = 0U;
            uint32_t nb_returns = 0U;
            uint8_t event[6+NBU_API_MAX_RETURN_PARAM_LENGTH]={0xA5};
            if( maPendingHciCmd[3] != 2U + 5U*nb_param + 1U + 1U)
            {
                // error
                status = 0x12; // HCI_ERR_INVALID_PARAMETERS
            }
            else
            {
                uint16_t api = maPendingHciCmd[4]+maPendingHciCmd[5]*256;
                nb_returns = maPendingHciCmd[4U+2U+5*nb_param+1U];
                bool rpmsg_status = PLATFORM_NbuApiReq(&event[6], api, &maPendingHciCmd[6], (uint32_t*)(&maPendingHciCmd[6]+1+nb_param), nb_returns);
                if ( !rpmsg_status )
                {
                    // error
                    status = 0x1F; // HCI_ERR_UNSPECIFIED_ERROR
                }
            }
            // Send command complete
            event[0] = 0x0E; // command complete
            event[1] = 0x04+nb_returns; // payloadlength
            event[2] = 0x01; // nb commands
            event[3] = maPendingHciCmd[1]; // opcode
            event[4] = maPendingHciCmd[2]; // opcode
            event[5] = status;
            uint16_t event_len;
            if( status == 0 )
            {
              event_len = 6+nb_returns;
            }
            else
            {
              event_len = 6;
            }
            HCI_AppControllerRxCallback(0x04, event, event_len);
        }
        else
#endif // HCI_CTRL_API_OPCODE
        {
            // cmd not processed
            assert(0);
        }

        // invalidate pending command
        maPendingHciCmd[0] = 0;
    }
#endif // ENABLE_HCI_CMD_HOOK
}

/*! *********************************************************************************
* @}
********************************************************************************** */
