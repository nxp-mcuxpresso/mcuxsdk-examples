/*
 * Copyright (c) 2015-2025 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -----------------------------------------------------------------------------
 *
 * Project:     CMSIS-Driver Validation
 * Title:       Tests definitions header file
 *
 * -----------------------------------------------------------------------------
 */

#ifndef __CMSIS_DV_H
#define __CMSIS_DV_H

#include <stdint.h>

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

#include "cmsis_os2.h"

#define GET_SYSTICK()   	osKernelGetTickCount()
#define SYSTICK_MS(ms)  	(((uint32_t)ms * osKernelGetTickFreq()) / 1000U)

#define GET_SYSTIMER()      osKernelGetSysTimerCount()
#define SYSTIMER_US(us)     (((uint64_t)us *  osKernelGetSysTimerFreq()) / 1000000)

#include "cmsis_compiler.h"

/* Expansion macro used to create CMSIS Driver references */
#define EXPAND_SYMBOL(name, port) name##port
#define CREATE_SYMBOL(name, port) EXPAND_SYMBOL(name, port)

// Test main function
extern void cmsis_dv (void *argument);

// Init/Uninit and testing functions
extern void SPI_DV_Initialize (void);
extern void SPI_DV_Uninitialize (void);
extern void SPI_GetVersion (void);
extern void SPI_GetCapabilities (void);
extern void SPI_Initialize_Uninitialize (void);
extern void SPI_PowerControl (void);
extern void SPI_Mode_Master_SS_Unused (void);
extern void SPI_Mode_Master_SS_Sw_Ctrl (void);
extern void SPI_Mode_Master_SS_Hw_Ctrl_Out (void);
extern void SPI_Mode_Master_SS_Hw_Mon_In (void);
extern void SPI_Mode_Slave_SS_Hw_Mon (void);
extern void SPI_Mode_Slave_SS_Sw_Ctrl (void);
extern void SPI_Format_Clock_Pol0_Pha0 (void);
extern void SPI_Format_Clock_Pol0_Pha1 (void);
extern void SPI_Format_Clock_Pol1_Pha0 (void);
extern void SPI_Format_Clock_Pol1_Pha1 (void);
extern void SPI_Format_Frame_TI (void);
extern void SPI_Format_Clock_Microwire (void);
extern void SPI_Data_Bits_1 (void);
extern void SPI_Data_Bits_2 (void);
extern void SPI_Data_Bits_3 (void);
extern void SPI_Data_Bits_4 (void);
extern void SPI_Data_Bits_5 (void);
extern void SPI_Data_Bits_6 (void);
extern void SPI_Data_Bits_7 (void);
extern void SPI_Data_Bits_8 (void);
extern void SPI_Data_Bits_9 (void);
extern void SPI_Data_Bits_10 (void);
extern void SPI_Data_Bits_11 (void);
extern void SPI_Data_Bits_12 (void);
extern void SPI_Data_Bits_13 (void);
extern void SPI_Data_Bits_14 (void);
extern void SPI_Data_Bits_15 (void);
extern void SPI_Data_Bits_16 (void);
extern void SPI_Data_Bits_17 (void);
extern void SPI_Data_Bits_18 (void);
extern void SPI_Data_Bits_19 (void);
extern void SPI_Data_Bits_20 (void);
extern void SPI_Data_Bits_21 (void);
extern void SPI_Data_Bits_22 (void);
extern void SPI_Data_Bits_23 (void);
extern void SPI_Data_Bits_24 (void);
extern void SPI_Data_Bits_25 (void);
extern void SPI_Data_Bits_26 (void);
extern void SPI_Data_Bits_27 (void);
extern void SPI_Data_Bits_28 (void);
extern void SPI_Data_Bits_29 (void);
extern void SPI_Data_Bits_30 (void);
extern void SPI_Data_Bits_31 (void);
extern void SPI_Data_Bits_32 (void);
extern void SPI_Bit_Order_MSB_LSB (void);
extern void SPI_Bit_Order_LSB_MSB (void);
extern void SPI_Bus_Speed_Min (void);
extern void SPI_Bus_Speed_Max (void);
extern void SPI_Number_Of_Items (void);
extern void SPI_GetDataCount (void);
extern void SPI_Abort (void);
extern void SPI_DataLost (void);
extern void SPI_ModeFault (void);

extern void USART_DV_Initialize (void);
extern void USART_DV_Uninitialize (void);
extern void USART_GetVersion (void);
extern void USART_GetCapabilities (void);
extern void USART_Initialize_Uninitialize (void);
extern void USART_PowerControl (void);
extern void USART_Mode_Asynchronous (void);
extern void USART_Mode_Synchronous_Master (void);
extern void USART_Mode_Synchronous_Slave (void);
extern void USART_Mode_Single_Wire (void);
extern void USART_Mode_IrDA (void);
extern void USART_Data_Bits_5 (void);
extern void USART_Data_Bits_6 (void);
extern void USART_Data_Bits_7 (void);
extern void USART_Data_Bits_8 (void);
extern void USART_Data_Bits_9 (void);
extern void USART_Parity_None (void);
extern void USART_Parity_Even (void);
extern void USART_Parity_Odd (void);
extern void USART_Stop_Bits_1 (void);
extern void USART_Stop_Bits_2 (void);
extern void USART_Stop_Bits_1_5 (void);
extern void USART_Stop_Bits_0_5 (void);
extern void USART_Flow_Control_None (void);
extern void USART_Flow_Control_RTS (void);
extern void USART_Flow_Control_CTS (void);
extern void USART_Flow_Control_RTS_CTS (void);
extern void USART_Clock_Pol0_Pha0 (void);
extern void USART_Clock_Pol0_Pha1 (void);
extern void USART_Clock_Pol1_Pha0 (void);
extern void USART_Clock_Pol1_Pha1 (void);
extern void USART_Baudrate_Min (void);
extern void USART_Baudrate_Max (void);
extern void USART_Number_Of_Items (void);
extern void USART_GetTxCount (void);
extern void USART_GetRxCount (void);
extern void USART_GetTxRxCount (void);
extern void USART_AbortSend (void);
extern void USART_AbortReceive (void);
extern void USART_AbortTransfer (void);
extern void USART_TxBreak (void);
extern void USART_Modem_RTS (void);
extern void USART_Modem_DTR (void);
extern void USART_Modem_CTS (void);
extern void USART_Modem_DSR (void);
extern void USART_Modem_DCD (void);
extern void USART_Modem_RI (void);
extern void USART_Tx_Underflow (void);
extern void USART_Rx_Overflow (void);
extern void USART_Rx_Timeout (void);
extern void USART_Rx_Break (void);
extern void USART_Rx_Framing_Error (void);
extern void USART_Rx_Parity_Error (void);
extern void USART_Event_CTS (void);
extern void USART_Event_DSR (void);
extern void USART_Event_DCD (void);
extern void USART_Event_RI (void);

extern void ETH_DV_Initialize (void);
extern void ETH_DV_Uninitialize (void);
extern void ETH_MAC_GetVersion (void);
extern void ETH_MAC_GetCapabilities (void);
extern void ETH_MAC_Initialization (void);
extern void ETH_MAC_PowerControl (void);
extern void ETH_MAC_MacAddress (void);
extern void ETH_MAC_SetBusSpeed (void);
extern void ETH_MAC_Config_Mode (void);
extern void ETH_MAC_Config_CommonParams (void);
extern void ETH_MAC_Control_Filtering (void);
extern void ETH_MAC_SetAddressFilter (void);
extern void ETH_MAC_VLAN_Filter (void);
extern void ETH_MAC_SignalEvent (void);
extern void ETH_MAC_PTP_ControlTimer (void);
extern void ETH_MAC_CheckInvalidInit (void);
extern void ETH_PHY_GetVersion (void);
extern void ETH_PHY_Initialization (void);
extern void ETH_PHY_PowerControl (void);
extern void ETH_PHY_Config (void);
extern void ETH_PHY_CheckInvalidInit (void);
extern void ETH_Loopback_Transfer (void);
extern void ETH_Loopback_PTP (void);
extern void ETH_Loopback_External (void);

extern void I2C_GetCapabilities (void);
extern void I2C_Initialization (void);
extern void I2C_PowerControl (void);
extern void I2C_SetBusSpeed (void);
extern void I2C_SetOwnAddress (void);
extern void I2C_BusClear (void);
extern void I2C_AbortTransfer (void);
extern void I2C_CheckInvalidInit (void);

extern void MCI_GetCapabilities (void);
extern void MCI_Initialization (void);
extern void MCI_PowerControl (void);
extern void MCI_SetBusSpeedMode (void);
extern void MCI_Config_DataWidth (void);
extern void MCI_Config_CmdLineMode (void);
extern void MCI_Config_DriverStrength (void);
extern void MCI_CheckInvalidInit (void);

extern void USBD_GetCapabilities (void);
extern void USBD_Initialization (void);
extern void USBD_PowerControl (void);
extern void USBD_CheckInvalidInit (void);

extern void USBH_GetCapabilities (void);
extern void USBH_Initialization (void);
extern void USBH_PowerControl (void);
extern void USBH_CheckInvalidInit (void);

extern void CAN_GetCapabilities (void);
extern void CAN_Initialization (void);
extern void CAN_PowerControl (void);
extern void CAN_CheckInvalidInit (void);
extern void CAN_Loopback_CheckBitrate (void);
extern void CAN_Loopback_CheckBitrateFD (void);
extern void CAN_Loopback_Transfer (void);
extern void CAN_Loopback_TransferFD (void);

extern void WIFI_DV_Initialize (void);
extern void WIFI_DV_Uninitialize (void);
extern void WIFI_GetVersion (void);
extern void WIFI_GetCapabilities (void);
extern void WIFI_Initialize_Uninitialize (void);
extern void WIFI_PowerControl (void);
extern void WIFI_GetModuleInfo (void);
extern void WIFI_SetOption_GetOption (void);
extern void WIFI_Scan (void);
extern void WIFI_Configure (void);
extern void WIFI_Activate_Deactivate (void);
extern void WIFI_IsConnected (void);
extern void WIFI_GetNetInfo (void);
extern void WIFI_Activate_AP (void);
extern void WIFI_Activate_Station_WPS_PBC (void);
extern void WIFI_Activate_Station_WPS_PIN (void);
extern void WIFI_Activate_AP_WPS_PBC (void);
extern void WIFI_Activate_AP_WPS_PIN (void);
extern void WIFI_SocketCreate (void);
extern void WIFI_SocketBind (void);
extern void WIFI_SocketListen (void);
extern void WIFI_SocketAccept (void);
extern void WIFI_SocketAccept_nbio (void);
extern void WIFI_SocketConnect (void);
extern void WIFI_SocketConnect_nbio (void);
extern void WIFI_SocketRecv (void);
extern void WIFI_SocketRecv_nbio (void);
extern void WIFI_SocketRecvFrom (void);
extern void WIFI_SocketRecvFrom_nbio (void);
extern void WIFI_SocketSend (void);
extern void WIFI_SocketSendTo (void);
extern void WIFI_SocketGetSockName (void);
extern void WIFI_SocketGetPeerName (void);
extern void WIFI_SocketGetOpt (void);
extern void WIFI_SocketSetOpt (void);
extern void WIFI_SocketClose (void);
extern void WIFI_SocketGetHostByName (void);
extern void WIFI_Ping (void);
extern void WIFI_Transfer_Fixed (void);
extern void WIFI_Transfer_Incremental (void);
extern void WIFI_Send_Fragmented (void);
extern void WIFI_Recv_Fragmented (void);
extern void WIFI_Test_Speed (void);
extern void WIFI_Concurrent_Socket (void);
extern void WIFI_Downstream_Rate (void);
extern void WIFI_Upstream_Rate (void);

extern void GPIO_Setup (void);
extern void GPIO_SetDirection (void);
extern void GPIO_SetOutputMode (void);
extern void GPIO_SetPullResistor (void);
extern void GPIO_SetEventTrigger (void);
extern void GPIO_SetOutput (void);
extern void GPIO_GetInput (void);

#endif /* __CMSIS_DV_H */
