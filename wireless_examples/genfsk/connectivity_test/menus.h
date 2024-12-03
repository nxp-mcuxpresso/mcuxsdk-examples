/*

Copyright 2016-2020 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#include "EmbeddedTypes.h"          /*Include special data types*/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
#include "fsl_component_serial_manager.h"
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
#include "SerialManager.h"
#endif /*SDK_COMPONENT_INTEGRATION > 0*/
#include "gen_fsk_tests.h"
#include "genfsk_interface.h"
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern char * const cu8Logo[];
extern char * const cu8AppError[];
extern char * const cu8MainMenu[];
extern char * const cu8ShortCutsBar[];
extern char * const cu8ContinuousTestMenu[];
extern char * const cu8PerTxTestMenu[];
extern char * const cu8PerRxTestMenu[];
extern char * const cu8RangeTxTestMenu[];
extern char * const cu8RangeRxTestMenu[];
extern char * const cu8ContinuousTestTags[];
extern char * const cu8SelectTags[];
extern char * const cu8TxModTestTags[];
extern char * const cu8PerRxMessages[];
extern char * const cu8PerTxMessages[];
extern char * const cu8RangeRxMessages[];
extern char * const cu8RangeTxMessages[];
extern char * const cu8ContMessages[];
extern char * const cu8RTCTrimMenu [];
#if defined(BOARD_LOCALIZATION_REVISION_SUPPORT) && (BOARD_LOCALIZATION_REVISION_SUPPORT > 0)
extern char * const cu8RFSwitchCtrlMenu [];
#endif
/************************************************************************************
*************************************************************************************
* Module macros
*************************************************************************************
************************************************************************************/

#define isAsciiHex(Data) (((Data >= '0') && (Data <= '9')) \
                       || ((Data >= 'A') && (Data <= 'F')) \
                       || ((Data >= 'a') && (Data <= 'f')))

#if defined(XCVR_TAG) && defined(XCVR_SHA1) && defined(GENFSK_TAG) && defined(GENFSK_SHA1)
/* This macro shall be inserted in cu8Logo[] array, if a specific application
needs to report the SW versions used.*/
#define PRINT_SW_VERSION            \
  "\r    SW versions:\n",           \
  "\r    XCVR:\n",                  \
  "\r         ",XCVR_TAG,"\n",      \
  "\r         ",XCVR_SHA1,"\n",     \
  "\r    GENFSK:\n",                \
  "\r         ",GENFSK_TAG,"\n",    \
  "\r         ",GENFSK_SHA1,"\n"
#else
#define PRINT_SW_VERSION            \
  ""
#endif /*defined(XCVR_TAG) && defined(XCVR_SHA1) && defined(GENFSK_TAG) && defined(GENFSK_SHA1)*/
/************************************************************************************
*************************************************************************************
* Interface functions prototypes
*************************************************************************************
************************************************************************************/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
extern void PrintMenu(char * const pu8Menu[], serial_write_handle_t writeHandle);
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
extern void PrintMenu(char * const pu8Menu[], uint8_t port);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
extern void PrintTestParams(ct_config_params_t* params,
                            bool_t bEraseLine,
                            serial_write_handle_t writeHandle);
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
extern void PrintTestParams(ct_config_params_t* params,
                            bool_t bEraseLine,
                            uint8_t port);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
extern void PrintPerTxPackets(uint16_t u16SentPackets,
                              bool_t bIsValidPacket,
                              serial_write_handle_t writeHandle);
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
extern void PrintPerTxPackets(uint16_t u16SentPackets,
                              bool_t bIsValidPacket,
                              uint8_t port);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
extern void PrintPerRxInfo(uint16_t u16PacketsIndex,
                           uint16_t u16ReceivedPackets,
                           ct_rx_indication_t* pIndicationInfo,
                           serial_write_handle_t writeHandle);
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
extern void PrintPerRxInfo(uint16_t u16PacketsIndex,
                           uint16_t u16ReceivedPackets,
                           ct_rx_indication_t* pIndicationInfo,
                           uint8_t port);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
extern void PrintPerRxFinal(uint16_t u16Received,
                            uint16_t u16TotalPackets,
                            int8_t i8AvgRssi,
                            serial_write_handle_t writeHandle);
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
extern void PrintPerRxFinal(uint16_t u16Received,
                            uint16_t u16TotalPackets,
                            int8_t i8AvgRssi,
                            uint8_t port);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
extern void PrintRangeTxDroppedPackets(uint16_t u16PacketsDropped,
                                       serial_write_handle_t writeHandle);
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
extern void PrintRangeTxDroppedPackets(uint16_t u16PacketsDropped,
                                       uint8_t port);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
extern void PrintAvgRssi(int8_t i8AverageRssi,
                         serial_write_handle_t writeHandle);
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
extern void PrintAvgRssi(int8_t i8AverageRssi,
                         uint8_t port);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */

#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
extern void PrintCrtRssi(int8_t i8CrtRssi,
                         serial_write_handle_t writeHandle);
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
extern void PrintCrtRssi(int8_t i8CrtRssi,
                         uint8_t port);
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
