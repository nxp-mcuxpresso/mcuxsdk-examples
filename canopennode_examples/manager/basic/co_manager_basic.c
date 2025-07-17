/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "app.h"
#include "CO_app_NXP.h"
#include "OD.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF
#define DLC (8)
#define CANopenManagerID    0x03
#define CANopenDeviceID     0x04

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DEMO_CANInit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t g_rxBuffer[1] = {0};
CANopenNodeNXP canOpenNodeNXP;
uint8_t g_sdoTxBuffer[4];
uint8_t g_sdoRxBuffer[4];
size_t g_ReadSize;
extern CO_t* CO;
extern CO_NMT_internalState_t deviceState;
extern uint8_t changedNMT_HB;
extern volatile bool rxBufferEmpty;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_CANInit(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_rx_fifo_config_t rxFifoConfig;
    uint8_t index;

    /* Init FlexCAN module. */
    /*
     * flexcanConfig.clkSrc                 = kFLEXCAN_ClkSrc0;
     * flexcanConfig.bitRate                = 1000000U;
     * flexcanConfig.bitRateFD              = 2000000U;
     * flexcanConfig.maxMbNum               = 16;
     * flexcanConfig.enableLoopBack         = false;
     * flexcanConfig.enableSelfWakeup       = false;
     * flexcanConfig.enableIndividMask      = false;
     * flexcanConfig.disableSelfReception   = false;
     * flexcanConfig.enableListenOnlyMode   = false;
     * flexcanConfig.enableDoze             = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

    flexcanConfig.bitRate = 500000U;
#if defined(EXAMPLE_CAN_CLK_SOURCE)
    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    flexcan_timing_config_t timing_config;
    memset(&timing_config, 0, sizeof(flexcan_timing_config_t));
    if (FLEXCAN_CalculateImprovedTimingValues(EXAMPLE_CAN, flexcanConfig.bitRate, EXAMPLE_CAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        LOG_INFO("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#endif

    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

    /* Accept non-matching standard/extended ID messages */
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, 0);

    /* Setup Tx Message Buffer. */
    for (index = TX_MB_FIRST_INDEX; index <= TX_MB_LAST_INDEX; index++)
    {
        FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, index, true);
    }

    /* Setup Legacy Rx FIFO. */
    rxFifoConfig.idFilterTable = NULL;
    rxFifoConfig.idFilterNum   = 0U;
    rxFifoConfig.idFilterType  = kFLEXCAN_RxFifoFilterTypeA;
    rxFifoConfig.priority      = kFLEXCAN_RxFifoPrioHigh;
    FLEXCAN_SetRxFifoConfig(EXAMPLE_CAN, &rxFifoConfig, true);

    FLEXCAN_SetRxFifoGlobalMask(EXAMPLE_CAN, FLEXCAN_RX_FIFO_STD_MASK_TYPE_A(0, 0, 0));
}

status_t DEMO_SDOTransferWrite(CO_SDOclient_t *SDO_C,
                               uint8_t nodeId,
                               uint16_t index,
                               uint8_t subIndex,
                               uint8_t *data,
                               size_t dataSize)
{
    CO_SDO_return_t SDO_ret;
    bool_t bufferPartial = false;
    size_t nWritten;

    SDO_ret = CO_SDOclient_setup(SDO_C, CO_CAN_ID_SDO_CLI + nodeId, CO_CAN_ID_SDO_SRV + nodeId, nodeId);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd)
    {
        return kStatus_Fail;
    }

    SDO_ret = CO_SDOclientDownloadInitiate(SDO_C, index, subIndex, dataSize, 1000, false);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd)
    {
        return kStatus_Fail;
    }

    nWritten = CO_SDOclientDownloadBufWrite(SDO_C, data, dataSize);
    if (nWritten < dataSize)
    {
        bufferPartial = true;   
    }

    do
    {
        uint32_t timeDifference_us = 10000;
        SDO_ret = CO_SDOclientDownload(SDO_C, timeDifference_us, false, bufferPartial, NULL, NULL, NULL);
        if (SDO_ret < 0)
        {
            return kStatus_Fail;
        }
        SDK_DelayAtLeastUs(timeDifference_us, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    } while (SDO_ret > 0);

    return kStatus_Success;
}

status_t DEMO_SDOTransferRead(CO_SDOclient_t *SDO_C,
                              uint8_t nodeId,
                              uint16_t index,
                              uint8_t subIndex,
                              uint8_t *buf,
                              size_t bufSize,
                              size_t *readSize)
{
    CO_SDO_return_t SDO_ret;

    SDO_ret = CO_SDOclient_setup(SDO_C,
                                 CO_CAN_ID_SDO_CLI + nodeId,
                                 CO_CAN_ID_SDO_SRV + nodeId,
                                 nodeId);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd) 
    {
        return kStatus_Fail;
    }

    SDO_ret = CO_SDOclientUploadInitiate(SDO_C, index, subIndex, 1000, false);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd) 
    {
        return kStatus_Fail;
    }

    do
    {
        uint32_t timeDifference_us = 10000;
        SDO_ret = CO_SDOclientUpload(SDO_C, timeDifference_us, false, NULL, NULL, NULL, NULL);
        if (SDO_ret < 0)
        {
            return kStatus_Fail;
        }
        SDK_DelayAtLeastUs(timeDifference_us, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    } while(SDO_ret > 0);

    *readSize = CO_SDOclientUploadBufRead(SDO_C, buf, bufSize);

    return kStatus_Success;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    EXAMPLE_InitTimer();

    LOG_INFO("\r\n== CANopenNode manager basic EXAMPLE ==\r\n");
    LOG_INFO(" - Press A Enter Device Operational mode\r\n");
    LOG_INFO(" - Press B Update Device Heartbeat producer time to 500ms (0x1017)\r\n");
    LOG_INFO(" - Press C Update Device RPDO (0x1400) mapping parameter (0x2000)\r\n");
    LOG_INFO(" - Press D Read Manager RPDO (0x1400) mapping parameter (0x2001)\r\n");
    LOG_INFO(" - Press E Read Manager RPDO (0x1401) mapping parameter (0x2002)\r\n");

    LOG_INFO("\r\nPress any key to start CANopen and Manager should start first\r\n");
    GETCHAR();

    canOpenNodeNXP.CANBasePtr     = EXAMPLE_CAN;
    canOpenNodeNXP.HWInitFunction = DEMO_CANInit;
    canOpenNodeNXP.baudrate       = 500;
    canOpenNodeNXP.desiredNodeID  = CANopenManagerID;
    canOpenNodeNXP.startTimer     = EXAMPLE_StartTimer;
    canOpenNodeNXP.stopTimer      = EXAMPLE_StopTimer;
    canopen_app_init(&canOpenNodeNXP);

    while (true)
    {
        canopen_app_process();

        EXAMPLE_UARTProcess();

        if (!rxBufferEmpty)
        {
            rxBufferEmpty = true;
            switch (g_rxBuffer[0])
            {
                case 'A':
                    /* Device must enter Operational mode first then use PDO feature. */
                    LOG_INFO("Press A\r\n");
                    CO_NMT_sendCommand(CO->NMT, CO_NMT_ENTER_OPERATIONAL, CANopenDeviceID);
                    break;

                case 'B':
                    g_sdoTxBuffer[0] = 0xF4;
                    g_sdoTxBuffer[1] = 0x01;
                    DEMO_SDOTransferWrite(CO->SDOclient, CANopenDeviceID, 0x1017, 0, g_sdoTxBuffer, 2);

                    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
                    LOG_INFO("Press B\r\n");
                    LOG_INFO("Read back Heartbeat producer time (0x1017) by SDO\r\n");
                    DEMO_SDOTransferRead(CO->SDOclient, CANopenDeviceID, 0x1017, 0, g_sdoRxBuffer, 4, &g_ReadSize);
                    LOG_INFO("SDO Data 0x%x 0x%x\r\n", g_sdoRxBuffer[0], g_sdoRxBuffer[1]);
                    break;

                case 'C':
                    OD_RAM.x2000_manager_tx_var0++;
                    CO_TPDOsendRequest(&CO->TPDO[0]);

                    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
                    LOG_INFO("Press C\r\n");
                    LOG_INFO("Read back Device RPDO (0x1400) mapping parameter (0x2000) by SDO\r\n");
                    DEMO_SDOTransferRead(CO->SDOclient, CANopenDeviceID, 0x2000, 0, g_sdoRxBuffer, 4, &g_ReadSize);
                    LOG_INFO("SDO Data 0x%x 0x%x\r\n", g_sdoRxBuffer[0], g_sdoRxBuffer[1]);
                    break;

                case 'D':
                    /*
                     * Device TPDO (0x1800) SYNC start value is 20, it means device will start to send
                     * cyclic TPDO frames when it receives SYNC frame with data 0x14(20).
                     * If first SYNC frame received by device data is greater than 0x14 or device enter
                     * operational mode after this SYNC frame, device will not send cyclic TPDO frames
                     * until next SYNC frame with data 0x14 received.
                     */
                    LOG_INFO("Press D\r\n");
                    LOG_INFO("RPDO (0x1400) mapping parameter (0x2001) 0x%x\r\n", OD_RAM.x2001_manager_rx_var0);
                    break;

                case 'E':
                    /*
                     * Device TPDO (0x1801) is configured as Synchronous acyclic transmission, it means
                     * device will start to send acyclic TPDO frames when it receives SYNC frame and
                     * internal event happens.
                     * In this example, device need to press A to trigger internal event.
                     */
                    LOG_INFO("Press E\r\n");
                    LOG_INFO("RPDO (0x1401) mapping parameter (0x2002) 0x%x\r\n", OD_RAM.x2002_manager_rx_var1);
                    break;

                default:
                    LOG_INFO("Invalid Input \r\n");
                    break;
            }
        }
    }
}
