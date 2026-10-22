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
 * Title:       Tests definitions
 *
 * -----------------------------------------------------------------------------
 */

#include "cmsis_dv.h"
#ifdef  RTE_CMSIS_DV_GPIO
#include "DV_GPIO_Config.h"
#endif
#ifdef  RTE_CMSIS_DV_SPI
#include "DV_SPI_Config.h"
#endif
#ifdef  RTE_CMSIS_DV_USART
#include "DV_USART_Config.h"
#endif
#ifdef  RTE_CMSIS_DV_ETH
#include "DV_ETH_Config.h"
#endif
#ifdef  RTE_CMSIS_DV_I2C
#include "DV_I2C_Config.h"
#endif
#ifdef  RTE_CMSIS_DV_MCI
#include "DV_MCI_Config.h"
#endif
#ifdef  RTE_CMSIS_DV_USBD
#include "DV_USBD_Config.h"
#endif
#ifdef  RTE_CMSIS_DV_USBH
#include "DV_USBH_Config.h"
#endif
#ifdef  RTE_CMSIS_DV_CAN
#include "DV_CAN_Config.h"
#endif
#ifdef  RTE_CMSIS_DV_WIFI
#include "DV_WiFi_Config.h"
#endif
#include "DV_Framework.h"

/*-----------------------------------------------------------------------------
 *      Init/Uninit test suite
 *----------------------------------------------------------------------------*/
#ifdef  RTE_CMSIS_DV_SPI
static void TS_Init_SPI (void) {
  SPI_DV_Initialize ();
}
static void TS_Uninit_SPI (void) {
  SPI_DV_Uninitialize ();
}
#endif
#ifdef  RTE_CMSIS_DV_USART
static void TS_Init_USART (void) {
  USART_DV_Initialize ();
}
static void TS_Uninit_USART (void) {
  USART_DV_Uninitialize ();
}
#endif
#ifdef  RTE_CMSIS_DV_ETH
static void TS_Init_ETH (void) {
  ETH_DV_Initialize ();
}
static void TS_Uninit_ETH (void) {
  ETH_DV_Uninitialize ();
}
#endif
#ifdef  RTE_CMSIS_DV_WIFI
static void TS_Init_WiFi (void) {
  WIFI_DV_Initialize ();
}
static void TS_Uninit_WiFi (void) {
  WIFI_DV_Uninitialize ();
}
#endif

/*-----------------------------------------------------------------------------
 *      Tests list
 *----------------------------------------------------------------------------*/
#ifdef  RTE_CMSIS_DV_SPI
static TEST_CASE TC_List_SPI[] = {
  #if ( SPI_TG_DRIVER_MANAGEMENT_EN != 0 )
  TCD ( SPI_GetVersion,                 SPI_TC_GET_VERSION_EN           ),
  TCD ( SPI_GetCapabilities,            SPI_TC_GET_CAPABILITIES_EN      ),
  TCD ( SPI_Initialize_Uninitialize,    SPI_TC_INIT_UNINIT_EN           ),
  TCD ( SPI_PowerControl,               SPI_TC_POWER_CONTROL_EN         ),
  #endif
  #if ( SPI_TG_DATA_EXCHANGE_EN != 0 )
  #if ( SPI_TG_MODE_EN != 0 )
  TCD ( SPI_Mode_Master_SS_Unused,      SPI_TC_MASTER_UNUSED_EN         ),
  TCD ( SPI_Mode_Master_SS_Sw_Ctrl,     SPI_TC_MASTER_SW_EN             ),
  TCD ( SPI_Mode_Master_SS_Hw_Ctrl_Out, SPI_TC_MASTER_HW_OUT_EN         ),
  TCD ( SPI_Mode_Master_SS_Hw_Mon_In,   SPI_TC_MASTER_HW_IN_EN          ),
  TCD ( SPI_Mode_Slave_SS_Hw_Mon,       SPI_TC_SLAVE_HW_EN              ),
  TCD ( SPI_Mode_Slave_SS_Sw_Ctrl,      SPI_TC_SLAVE_SW_EN              ),
  #endif
  #if ( SPI_TG_FORMAT_EN != 0 )
  TCD ( SPI_Format_Clock_Pol0_Pha0,     SPI_TC_FORMAT_POL0_PHA0_EN      ),
  TCD ( SPI_Format_Clock_Pol0_Pha1,     SPI_TC_FORMAT_POL0_PHA1_EN      ),
  TCD ( SPI_Format_Clock_Pol1_Pha0,     SPI_TC_FORMAT_POL1_PHA0_EN      ),
  TCD ( SPI_Format_Clock_Pol1_Pha1,     SPI_TC_FORMAT_POL1_PHA1_EN      ),
  TCD ( SPI_Format_Frame_TI,            SPI_TC_FORMAT_TI_EN             ),
  TCD ( SPI_Format_Clock_Microwire,     SPI_TC_FORMAT_MICROWIRE_EN      ),
  #endif
  #if ( SPI_TG_DATA_BIT_EN != 0 )
  TCD ( SPI_Data_Bits_1,               (SPI_TC_DATA_BIT_EN_MASK      )&1),
  TCD ( SPI_Data_Bits_2,               (SPI_TC_DATA_BIT_EN_MASK >>  1)&1),
  TCD ( SPI_Data_Bits_3,               (SPI_TC_DATA_BIT_EN_MASK >>  2)&1),
  TCD ( SPI_Data_Bits_4,               (SPI_TC_DATA_BIT_EN_MASK >>  3)&1),
  TCD ( SPI_Data_Bits_5,               (SPI_TC_DATA_BIT_EN_MASK >>  4)&1),
  TCD ( SPI_Data_Bits_6,               (SPI_TC_DATA_BIT_EN_MASK >>  5)&1),
  TCD ( SPI_Data_Bits_7,               (SPI_TC_DATA_BIT_EN_MASK >>  6)&1),
  TCD ( SPI_Data_Bits_8,               (SPI_TC_DATA_BIT_EN_MASK >>  7)&1),
  TCD ( SPI_Data_Bits_9,               (SPI_TC_DATA_BIT_EN_MASK >>  8)&1),
  TCD ( SPI_Data_Bits_10,              (SPI_TC_DATA_BIT_EN_MASK >>  9)&1),
  TCD ( SPI_Data_Bits_11,              (SPI_TC_DATA_BIT_EN_MASK >> 10)&1),
  TCD ( SPI_Data_Bits_12,              (SPI_TC_DATA_BIT_EN_MASK >> 11)&1),
  TCD ( SPI_Data_Bits_13,              (SPI_TC_DATA_BIT_EN_MASK >> 12)&1),
  TCD ( SPI_Data_Bits_14,              (SPI_TC_DATA_BIT_EN_MASK >> 13)&1),
  TCD ( SPI_Data_Bits_15,              (SPI_TC_DATA_BIT_EN_MASK >> 14)&1),
  TCD ( SPI_Data_Bits_16,              (SPI_TC_DATA_BIT_EN_MASK >> 15)&1),
  TCD ( SPI_Data_Bits_17,              (SPI_TC_DATA_BIT_EN_MASK >> 16)&1),
  TCD ( SPI_Data_Bits_18,              (SPI_TC_DATA_BIT_EN_MASK >> 17)&1),
  TCD ( SPI_Data_Bits_19,              (SPI_TC_DATA_BIT_EN_MASK >> 18)&1),
  TCD ( SPI_Data_Bits_20,              (SPI_TC_DATA_BIT_EN_MASK >> 19)&1),
  TCD ( SPI_Data_Bits_21,              (SPI_TC_DATA_BIT_EN_MASK >> 20)&1),
  TCD ( SPI_Data_Bits_22,              (SPI_TC_DATA_BIT_EN_MASK >> 21)&1),
  TCD ( SPI_Data_Bits_23,              (SPI_TC_DATA_BIT_EN_MASK >> 22)&1),
  TCD ( SPI_Data_Bits_24,              (SPI_TC_DATA_BIT_EN_MASK >> 23)&1),
  TCD ( SPI_Data_Bits_25,              (SPI_TC_DATA_BIT_EN_MASK >> 24)&1),
  TCD ( SPI_Data_Bits_26,              (SPI_TC_DATA_BIT_EN_MASK >> 25)&1),
  TCD ( SPI_Data_Bits_27,              (SPI_TC_DATA_BIT_EN_MASK >> 26)&1),
  TCD ( SPI_Data_Bits_28,              (SPI_TC_DATA_BIT_EN_MASK >> 27)&1),
  TCD ( SPI_Data_Bits_29,              (SPI_TC_DATA_BIT_EN_MASK >> 28)&1),
  TCD ( SPI_Data_Bits_30,              (SPI_TC_DATA_BIT_EN_MASK >> 29)&1),
  TCD ( SPI_Data_Bits_31,              (SPI_TC_DATA_BIT_EN_MASK >> 30)&1),
  TCD ( SPI_Data_Bits_32,              (SPI_TC_DATA_BIT_EN_MASK >> 31)&1),
  #endif
  #if ( SPI_TG_BIT_ORDER_EN != 0 )
  TCD ( SPI_Bit_Order_MSB_LSB,          SPI_TC_BIT_ORDER_MSB_LSB_EN     ),
  TCD ( SPI_Bit_Order_LSB_MSB,          SPI_TC_BIT_ORDER_LSB_MSB_EN     ),
  #endif
  #if ( SPI_TG_BUS_SPEED_EN != 0 )
  TCD ( SPI_Bus_Speed_Min,              SPI_TC_BUS_SPEED_MIN_EN         ),
  TCD ( SPI_Bus_Speed_Max,              SPI_TC_BUS_SPEED_MAX_EN         ),
  #endif
  #if ( SPI_TG_OTHER_EN != 0 )
  TCD ( SPI_Number_Of_Items,            SPI_TC_NUMBER_OF_ITEMS_EN       ),
  TCD ( SPI_GetDataCount,               SPI_TC_GET_DATA_COUNT_EN        ),
  TCD ( SPI_Abort,                      SPI_TC_ABORT_EN                 ),
  #endif
  #endif
  #if ( SPI_TG_EVENT_EN != 0 )
  TCD ( SPI_DataLost,                   SPI_TC_DATA_LOST_EN             ),
  TCD ( SPI_ModeFault,                  SPI_TC_MODE_FAULT_EN            ),
  #endif
};
#endif

#ifdef  RTE_CMSIS_DV_USART
static TEST_CASE TC_List_USART[] = {
  #if ( USART_TG_DRIVER_MANAGEMENT_EN != 0 )
  TCD ( USART_GetVersion,               USART_TC_GET_VERSION_EN         ),
  TCD ( USART_GetCapabilities,          USART_TC_GET_CAPABILITIES_EN    ),
  TCD ( USART_Initialize_Uninitialize,  USART_TC_INIT_UNINIT_EN         ),
  TCD ( USART_PowerControl,             USART_TC_POWER_CONTROL_EN       ),
  #endif
  #if ( USART_TG_DATA_EXCHANGE_EN != 0 )
  #if ( USART_TG_MODE_EN != 0 )
  TCD ( USART_Mode_Asynchronous,        USART_TC_ASYNC_EN               ),
  TCD ( USART_Mode_Synchronous_Master,  USART_TC_SYNC_MASTER_EN         ),
  TCD ( USART_Mode_Synchronous_Slave,   USART_TC_SYNC_SLAVE_EN          ),
  TCD ( USART_Mode_Single_Wire,         USART_TC_SINGLE_WIRE_EN         ),
  TCD ( USART_Mode_IrDA,                USART_TC_IRDA_EN                ),
  #endif
  #if ( USART_TG_DATA_BITS_EN != 0 )
  TCD ( USART_Data_Bits_5,              USART_TC_DATA_BITS_5_EN         ),
  TCD ( USART_Data_Bits_6,              USART_TC_DATA_BITS_6_EN         ),
  TCD ( USART_Data_Bits_7,              USART_TC_DATA_BITS_7_EN         ),
  TCD ( USART_Data_Bits_8,              USART_TC_DATA_BITS_8_EN         ),
  TCD ( USART_Data_Bits_9,              USART_TC_DATA_BITS_9_EN         ),
  #endif
  #if ( USART_TG_PARITY_EN != 0 )
  TCD ( USART_Parity_None,              USART_TC_PARITY_NONE_EN         ),
  TCD ( USART_Parity_Even,              USART_TC_PARITY_EVEN_EN         ),
  TCD ( USART_Parity_Odd,               USART_TC_PARITY_ODD_EN          ),
  #endif
  #if ( USART_TG_STOP_BITS_EN != 0 )
  TCD ( USART_Stop_Bits_1,              USART_TC_STOP_BITS_1_EN         ),
  TCD ( USART_Stop_Bits_2,              USART_TC_STOP_BITS_2_EN         ),
  TCD ( USART_Stop_Bits_1_5,            USART_TC_STOP_BITS_1_5_EN       ),
  TCD ( USART_Stop_Bits_0_5,            USART_TC_STOP_BITS_0_5_EN       ),
  #endif
  #if ( USART_TG_FLOW_CTRL_EN != 0)
  TCD ( USART_Flow_Control_None,        USART_TC_FLOW_CTRL_NONE_EN      ),
  TCD ( USART_Flow_Control_RTS,         USART_TC_FLOW_CTRL_RTS_EN       ),
  TCD ( USART_Flow_Control_CTS,         USART_TC_FLOW_CTRL_CTS_EN       ),
  TCD ( USART_Flow_Control_RTS_CTS,     USART_TC_FLOW_CTRL_RTS_CTS_EN   ),
  #endif
  #if ( USART_TG_CLOCK_EN != 0)
  TCD ( USART_Clock_Pol0_Pha0,          USART_TC_CLOCK_POL0_PHA0_EN     ),
  TCD ( USART_Clock_Pol0_Pha1,          USART_TC_CLOCK_POL0_PHA1_EN     ),
  TCD ( USART_Clock_Pol1_Pha0,          USART_TC_CLOCK_POL1_PHA1_EN     ),
  TCD ( USART_Clock_Pol1_Pha1,          USART_TC_CLOCK_POL1_PHA1_EN     ),
  #endif
  #if ( USART_TG_BAUDRATE_EN != 0)
  TCD ( USART_Baudrate_Min,             USART_TC_BAUDRATE_MIN_EN        ),
  TCD ( USART_Baudrate_Max,             USART_TC_BAUDRATE_MAX_EN        ),
  #endif
  #if ( USART_TG_OTHER_EN != 0)
  TCD ( USART_Number_Of_Items,          USART_TC_NUMBER_OF_ITEMS_EN     ),
  TCD ( USART_GetTxCount,               USART_TC_GET_TX_COUNT_EN        ),
  TCD ( USART_GetRxCount,               USART_TC_GET_RX_COUNT_EN        ),
  TCD ( USART_GetTxRxCount,             USART_TC_GET_TX_RX_COUNT_EN     ),
  TCD ( USART_AbortSend,                USART_TC_ABORT_SEND_EN          ),
  TCD ( USART_AbortReceive,             USART_TC_ABORT_RECEIVE_EN       ),
  TCD ( USART_AbortTransfer,            USART_TC_ABORT_TRANSFER_EN      ),
  TCD ( USART_TxBreak,                  USART_TC_TX_BREAK_EN            ),
  #endif
  #endif
  #if ( USART_TG_MODEM_EN != 0 )
  TCD ( USART_Modem_RTS,                USART_TC_MODEM_RTS_EN           ),
  TCD ( USART_Modem_DTR,                USART_TC_MODEM_DTR_EN           ),
  TCD ( USART_Modem_CTS,                USART_TC_MODEM_CTS_EN           ),
  TCD ( USART_Modem_DSR,                USART_TC_MODEM_DSR_EN           ),
  TCD ( USART_Modem_DCD,                USART_TC_MODEM_DCD_EN           ),
  TCD ( USART_Modem_RI,                 USART_TC_MODEM_RI_EN            ),
  #endif
  #if ( USART_TG_EVENT_EN != 0 )
  TCD ( USART_Tx_Underflow,             USART_TC_TX_UNDERFLOW_EN        ),
  TCD ( USART_Rx_Overflow,              USART_TC_RX_OVERFLOW_EN         ),
  TCD ( USART_Rx_Timeout,               USART_TC_RX_TIMEOUT_EN          ),
  TCD ( USART_Rx_Break,                 USART_TC_RX_BREAK_EN            ),
  TCD ( USART_Rx_Framing_Error,         USART_TC_RX_FRAMING_ERROR_EN    ),
  TCD ( USART_Rx_Parity_Error,          USART_TC_RX_PARITY_ERROR_EN     ),
  TCD ( USART_Event_CTS,                USART_TC_EVENT_CTS_EN           ),
  TCD ( USART_Event_DSR,                USART_TC_EVENT_DSR_EN           ),
  TCD ( USART_Event_DCD,                USART_TC_EVENT_DCD_EN           ),
  TCD ( USART_Event_RI,                 USART_TC_EVENT_RI_EN            ),
  #endif
};
#endif

#ifdef  RTE_CMSIS_DV_ETH
static TEST_CASE TC_List_ETH[] = {
  TCD ( ETH_MAC_GetVersion,             ETH_MAC_GET_VERSION_EN          ),
  TCD ( ETH_MAC_GetCapabilities,        ETH_MAC_GET_CAPABILITIES_EN     ),
  TCD ( ETH_MAC_Initialization,         ETH_MAC_INITIALIZATION_EN       ),
  TCD ( ETH_MAC_PowerControl,           ETH_MAC_POWER_CONTROL_EN        ),
  TCD ( ETH_MAC_MacAddress,             ETH_MAC_MAC_ADDRESS_EN          ),
  TCD ( ETH_MAC_SetBusSpeed,            ETH_MAC_SET_BUS_SPEED_EN        ),
  TCD ( ETH_MAC_Config_Mode,            ETH_MAC_CONFIG_MODE_EN          ),
  TCD ( ETH_MAC_Config_CommonParams,    ETH_MAC_CONFIG_COMMON_PARAMS_EN ),
  TCD ( ETH_MAC_Control_Filtering,      ETH_MAC_CONTROL_FILTERING_EN    ),
  TCD ( ETH_MAC_SetAddressFilter,       ETH_MAC_SET_ADDRESS_FILTER_EN   ),
  #ifdef ETH_MAC_VLAN_FILTER_EN
  TCD ( ETH_MAC_VLAN_Filter,            ETH_MAC_VLAN_FILTER_EN          ),
  #endif
  TCD ( ETH_MAC_SignalEvent,            ETH_MAC_SIGNAL_EVENT_EN         ),
  TCD ( ETH_MAC_PTP_ControlTimer,       ETH_MAC_PTP_CONTROL_TIMER_EN    ),
  TCD ( ETH_MAC_CheckInvalidInit,       ETH_MAC_CHECK_INVALID_INIT_EN   ),
  TCD ( ETH_PHY_GetVersion,             ETH_PHY_GET_VERSION_EN          ),
  TCD ( ETH_PHY_Initialization,         ETH_PHY_INITIALIZATION_EN       ),
  TCD ( ETH_PHY_PowerControl,           ETH_PHY_POWER_CONTROL_EN        ),
  TCD ( ETH_PHY_Config,                 ETH_PHY_CONFIG_EN               ),
  TCD ( ETH_PHY_CheckInvalidInit,       ETH_PHY_CHECK_INVALID_INIT_EN   ),
  TCD ( ETH_Loopback_Transfer,          ETH_LOOPBACK_TRANSFER_EN        ),
  TCD ( ETH_Loopback_PTP,               ETH_LOOPBACK_PTP_EN             ),
  TCD ( ETH_Loopback_External,          ETH_LOOPBACK_EXTERNAL_EN        ),
};
#endif

#ifdef  RTE_CMSIS_DV_I2C
static TEST_CASE TC_List_I2C[] = {
  TCD ( I2C_GetCapabilities,            I2C_GETCAPABILITIES_EN          ),
  TCD ( I2C_Initialization,             I2C_INITIALIZATION_EN           ),
  TCD ( I2C_PowerControl,               I2C_POWERCONTROL_EN             ),
  TCD ( I2C_SetBusSpeed,                I2C_SETBUSSPEED_EN              ),
  TCD ( I2C_SetOwnAddress,              I2C_SETOWNADDRESS_EN            ),
  TCD ( I2C_BusClear,                   I2C_BUSCLEAR_EN                 ),
  TCD ( I2C_AbortTransfer,              I2C_ABORTTRANSFER_EN            ),
  TCD ( I2C_CheckInvalidInit,           I2C_CHECKINVALIDINIT_EN         ),
};
#endif

#ifdef  RTE_CMSIS_DV_MCI
static TEST_CASE TC_List_MCI[] = {
  TCD ( MCI_GetCapabilities,            MCI_GETCAPABILITIES_EN          ),
  TCD ( MCI_Initialization,             MCI_INITIALIZATION_EN           ),
  TCD ( MCI_PowerControl,               MCI_POWERCONTROL_EN             ),
  TCD ( MCI_SetBusSpeedMode,            MCI_SETBUSSPEEDMODE_EN          ),
  TCD ( MCI_Config_DataWidth,           MCI_CONFIG_DATAWIDTH_EN         ),
  TCD ( MCI_Config_CmdLineMode,         MCI_CONFIG_CMDLINEMODE_EN       ),
  TCD ( MCI_Config_DriverStrength,      MCI_CONFIG_DRIVERSTRENGTH_EN    ),
  TCD ( MCI_CheckInvalidInit,           MCI_CHECKINVALIDINIT_EN         ),
};
#endif

#ifdef  RTE_CMSIS_DV_USBD
static TEST_CASE TC_List_USBD[] = {
  TCD ( USBD_GetCapabilities,           USBD_GETCAPABILITIES_EN         ),
  TCD ( USBD_Initialization,            USBD_INITIALIZATION_EN          ),
  TCD ( USBD_PowerControl,              USBD_POWERCONTROL_EN            ),
  TCD ( USBD_CheckInvalidInit,          USBD_CHECKINVALIDINIT_EN        ),
};
#endif

#ifdef  RTE_CMSIS_DV_USBH
static TEST_CASE TC_List_USBH[] = {
  TCD ( USBH_GetCapabilities,           USBH_GETCAPABILITIES_EN         ),
  TCD ( USBH_Initialization,            USBH_INITIALIZATION_EN          ),
  TCD ( USBH_PowerControl,              USBH_POWERCONTROL_EN            ),
  TCD ( USBH_CheckInvalidInit,          USBH_CHECKINVALIDINIT_EN        ),
};
#endif

#ifdef  RTE_CMSIS_DV_CAN
static TEST_CASE TC_List_CAN[] = {
  TCD ( CAN_GetCapabilities,            CAN_GETCAPABILITIES_EN          ),
  TCD ( CAN_Initialization,             CAN_INITIALIZATION_EN           ),
  TCD ( CAN_PowerControl,               CAN_POWERCONTROL_EN             ),
  TCD ( CAN_Loopback_CheckBitrate,      CAN_LOOPBACK_CHECK_BR_EN        ),
  TCD ( CAN_Loopback_CheckBitrateFD,    CAN_LOOPBACK_CHECK_BR_FD_EN     ),
  TCD ( CAN_Loopback_Transfer,          CAN_LOOPBACK_TRANSFER_EN        ),
  TCD ( CAN_Loopback_TransferFD,        CAN_LOOPBACK_TRANSFER_FD_EN     ),
};
#endif

#ifdef  RTE_CMSIS_DV_WIFI
static TEST_CASE TC_List_WiFi[] = {
  /*    WiFi Control tests */
  #if ( WIFI_CONTROL_EN != 0)
  TCD ( WIFI_GetVersion,                WIFI_GETVERSION_EN              ),
  TCD ( WIFI_GetCapabilities,           WIFI_GETCAPABILITIES_EN         ),
  TCD ( WIFI_Initialize_Uninitialize,   WIFI_INIT_UNINIT_EN             ),
  TCD ( WIFI_PowerControl,              WIFI_POWERCONTROL_EN            ),
  TCD ( WIFI_GetModuleInfo,             WIFI_GETMODULEINFO_EN           ),
  #endif
  /*    WiFi Management tests */
  #if ( WIFI_MANAGEMENT_EN != 0)
  TCD ( WIFI_SetOption_GetOption,       WIFI_SETGETOPTION_EN            ),
  TCD ( WIFI_Scan,                      WIFI_SCAN_EN                    ),
  TCD ( WIFI_Activate_Deactivate,       WIFI_ACT_DEACT_EN               ),
  TCD ( WIFI_IsConnected,               WIFI_ISCONNECTED_EN             ),
  TCD ( WIFI_GetNetInfo,                WIFI_GETNETINFO_EN              ),
  #endif
  /*    WiFi Management tests requiring user interaction */
  #if ( WIFI_MANAGEMENT_USER_EN != 0)
  TCD ( WIFI_Activate_AP,               WIFI_ACT_AP                     ),
  #if ( WIFI_WPS_USER_EN != 0)
  TCD ( WIFI_Activate_Station_WPS_PBC,  WIFI_ACT_STA_WPS_PBC            ),
  TCD ( WIFI_Activate_Station_WPS_PIN,  WIFI_ACT_STA_WPS_PIN            ),
  TCD ( WIFI_Activate_AP_WPS_PBC,       WIFI_ACT_AP_WPS_PBC             ),
  TCD ( WIFI_Activate_AP_WPS_PIN,       WIFI_ACT_AP_WPS_PIN             ),
  #endif
  #endif
  /*    WiFi Socket API tests */
  #if ( WIFI_SOCKET_EN != 0)
  TCD ( WIFI_SocketCreate,              WIFI_SOCKETCREATE_EN            ),
  TCD ( WIFI_SocketBind,                WIFI_SOCKETBIND_EN              ),
  TCD ( WIFI_SocketListen,              WIFI_SOCKETLISTEN_EN            ),
  TCD ( WIFI_SocketAccept,              WIFI_SOCKETACCEPT_EN            ),
  TCD ( WIFI_SocketAccept_nbio,         WIFI_SOCKETACCEPT_NBIO_EN       ),
  TCD ( WIFI_SocketConnect,             WIFI_SOCKETCONNECT_EN           ),
  TCD ( WIFI_SocketConnect_nbio,        WIFI_SOCKETCONNECT_NBIO_EN      ),
  TCD ( WIFI_SocketRecv,                WIFI_SOCKETRECV_EN              ),
  TCD ( WIFI_SocketRecv_nbio,           WIFI_SOCKETRECV_NBIO_EN         ),
  TCD ( WIFI_SocketRecvFrom,            WIFI_SOCKETRECVFROM_EN          ),
  TCD ( WIFI_SocketRecvFrom_nbio,       WIFI_SOCKETRECVFROM_NBIO_EN     ),
  TCD ( WIFI_SocketSend,                WIFI_SOCKETSEND_EN              ),
  TCD ( WIFI_SocketSendTo,              WIFI_SOCKETSENDTO_EN            ),
  TCD ( WIFI_SocketGetSockName,         WIFI_SOCKETGETSOCKNAME_EN       ),
  TCD ( WIFI_SocketGetPeerName,         WIFI_SOCKETGETPEERNAME_EN       ),
  TCD ( WIFI_SocketGetOpt,              WIFI_SOCKETGETOPT_EN            ),
  TCD ( WIFI_SocketSetOpt,              WIFI_SOCKETSETOPT_EN            ),
  TCD ( WIFI_SocketClose,               WIFI_SOCKETCLOSE_EN             ),
  TCD ( WIFI_SocketGetHostByName,       WIFI_SOCKETGETHOSTBYNAME_EN     ),
  TCD ( WIFI_Ping,                      WIFI_PING_EN                    ),
  #endif
  /*    WiFi Socket Operation tests */
  #if ( WIFI_SOCKET_OP_EN != 0)
  TCD ( WIFI_Transfer_Fixed,            WIFI_TRANSFER_FIXED_EN          ),
  TCD ( WIFI_Transfer_Incremental,      WIFI_TRANSFER_INCREMENTAL_EN    ),
  TCD ( WIFI_Send_Fragmented,           WIFI_SEND_FRAGMENTED_EN         ),
  TCD ( WIFI_Recv_Fragmented,           WIFI_RECV_FRAGMENTED_EN         ),
  TCD ( WIFI_Test_Speed,                WIFI_TEST_SPEED_EN              ),
  TCD ( WIFI_Concurrent_Socket,         WIFI_CONCURRENT_SOCKET_EN       ),
  TCD ( WIFI_Downstream_Rate,           WIFI_DOWNSTREAM_RATE_EN         ),
  TCD ( WIFI_Upstream_Rate,             WIFI_UPSTREAM_RATE_EN           ),
  #endif
};
#endif

#ifdef  RTE_CMSIS_DV_GPIO
static TEST_CASE TC_List_GPIO[] = {
  TCD ( GPIO_Setup,                     GPIO_TC_SETUP_EN                ),
  TCD ( GPIO_SetDirection,              GPIO_TC_SET_DIRECTION_EN        ),
  TCD ( GPIO_SetOutputMode,             GPIO_TC_SET_OUTPUT_MODE_EN      ),
  TCD ( GPIO_SetPullResistor,           GPIO_TC_SET_PULL_RESISTOR_EN    ),
  TCD ( GPIO_SetEventTrigger,           GPIO_TC_SET_EVENT_TRIGGER_EN    ),
  TCD ( GPIO_SetOutput,                 GPIO_TC_SET_OUTPUT_EN           ),
  TCD ( GPIO_GetInput,                  GPIO_TC_GET_INPUT_EN            )
};
#endif

/*-----------------------------------------------------------------------------
 *      Test suite description
 *----------------------------------------------------------------------------*/
TEST_GROUP ts[] = {
#ifdef  RTE_CMSIS_DV_SPI                /* SPI test group                     */
{
  __FILE__, __DATE__, __TIME__,
  "CMSIS-Driver_Validation v" RTE_CMSIS_DV_PACK_VER " CMSIS-Driver SPI Test Report",
  TS_Init_SPI,
  TS_Uninit_SPI,
  TC_List_SPI,
  ARRAY_SIZE (TC_List_SPI),
},
#endif

#ifdef  RTE_CMSIS_DV_USART              /* USART test group                   */
{
  __FILE__, __DATE__, __TIME__,
  "CMSIS-Driver_Validation v" RTE_CMSIS_DV_PACK_VER " CMSIS-Driver USART Test Report",
  TS_Init_USART,
  TS_Uninit_USART,
  TC_List_USART,
  ARRAY_SIZE (TC_List_USART),
},
#endif

#ifdef  RTE_CMSIS_DV_ETH                /* ETH test group                     */
{
  __FILE__, __DATE__, __TIME__,
  "CMSIS-Driver_Validation v" RTE_CMSIS_DV_PACK_VER " CMSIS-Driver ETH Test Report",
  TS_Init_ETH,
  TS_Uninit_ETH,
  TC_List_ETH,
  ARRAY_SIZE (TC_List_ETH),
},
#endif

#ifdef  RTE_CMSIS_DV_I2C                /* I2C test group                     */
{
  __FILE__, __DATE__, __TIME__,
  "CMSIS-Driver_Validation v" RTE_CMSIS_DV_PACK_VER " CMSIS-Driver I2C (API v2.3) Test Report",
  NULL,
  NULL,
  TC_List_I2C,
  ARRAY_SIZE (TC_List_I2C),
},
#endif

#ifdef  RTE_CMSIS_DV_MCI                /* MCI test group                     */
{
  __FILE__, __DATE__, __TIME__,
  "CMSIS-Driver_Validation v" RTE_CMSIS_DV_PACK_VER " CMSIS-Driver MCI Test Report",
  NULL,
  NULL,
  TC_List_MCI,
  ARRAY_SIZE (TC_List_MCI),
},
#endif

#ifdef  RTE_CMSIS_DV_USBD               /* USBD test group                    */
{
  __FILE__, __DATE__, __TIME__,
  "CMSIS-Driver_Validation v" RTE_CMSIS_DV_PACK_VER " CMSIS-Driver USBD Test Report",
  NULL,
  NULL,
  TC_List_USBD,
  ARRAY_SIZE (TC_List_USBD),
},
#endif

#ifdef  RTE_CMSIS_DV_USBH               /* USBH test group                    */
{
  __FILE__, __DATE__, __TIME__,
  "CMSIS-Driver_Validation v" RTE_CMSIS_DV_PACK_VER " CMSIS-Driver USBH Test Report",
  NULL,
  NULL,
  TC_List_USBH,
  ARRAY_SIZE (TC_List_USBH),
},
#endif

#ifdef  RTE_CMSIS_DV_CAN                /* CAN test group                    */
{
  __FILE__, __DATE__, __TIME__,
  "CMSIS-Driver_Validation v" RTE_CMSIS_DV_PACK_VER " CMSIS-Driver CAN Test Report",
  NULL,
  NULL,
  TC_List_CAN,
  ARRAY_SIZE (TC_List_CAN),
},
#endif

#ifdef  RTE_CMSIS_DV_WIFI               /* WIFI test group                    */
{
  __FILE__, __DATE__, __TIME__,
  "CMSIS-Driver_Validation v" RTE_CMSIS_DV_PACK_VER " CMSIS-Driver WiFi Test Report",
  TS_Init_WiFi,
  TS_Uninit_WiFi,
  TC_List_WiFi,
  ARRAY_SIZE (TC_List_WiFi),
},
#endif

#ifdef  RTE_CMSIS_DV_GPIO               /* GPIO test group                    */
{
  __FILE__, __DATE__, __TIME__,
  "CMSIS-Driver_Validation v" RTE_CMSIS_DV_PACK_VER " CMSIS-Driver GPIO Test Report",
  NULL,
  NULL,
  TC_List_GPIO,
  ARRAY_SIZE (TC_List_GPIO),
},
#endif
};

/* Number of test groups in suite */
uint32_t tg_cnt = sizeof(ts)/sizeof(ts[0]);

