/*

Copyright 2016-2020 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#include "menus.h"
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
#include "fsl_format.h"
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
#define Serial_Print(a,b,c)  SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, (uint8_t *)b, strlen(b))
#define Serial_PrintDec(a,b) SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Dec2Str(b), strlen((char const *)FORMAT_Dec2Str(b)))
#else  /*SDK_COMPONENT_INTEGRATION > 0*/
#endif /*SDK_COMPONENT_INTEGRATION > 0*/

char * const cu8Logo[] =
{
  "\f\r\n",
  "\n\n\r\n",
  " ####         ######      ##### ##########\n\r",
  " ######      # ######    ##### #############\n\r",
  " #######     ## ######  ##### ###############\n\r",
  " ########    ### ########### ####       #####\n\r",
  " #### #####  #### ######### #####       #####\n\r",
  " ####  ##### #### ######### #################\n\r",
  " ####   ######## ########### ###############\n\r",
  " ####     ##### ######  ##### ############\n\r",
  " ####      ### ######    ##### ##\n\r",
  " ####       # ######      ##### #\n\r\n\r",
  "\r    Generic FSK Connectivity Test Demo\n\n",
  PRINT_SW_VERSION,
  "\r\n -Press enter to start",
  NULL
};

/*app erro, has to align with app_status_t */
char * const cu8AppError[] =
{
  "",
  "\r\nError: not enough memory defined in \"PoolsDetails_c\"",
  NULL
};

/*@CMA, Conn Test. New string*/
char * const cu8MainMenu[] =
{
  "\f\r  Connectivity Test Interface short cuts\n",
  "\r------------------------------------------\n",
  "\r -Press [t] for Tx operation\n",
  "\r -Press [r] for Rx operation\n",
  "\r -Press [z] to increase mode (GENFSK, BLE) and rate\n",
  "\r -Press [x] to decrease mode (GENFSK, BLE) and rate\n",
  "\r -Press [c] to toggle between fixed or per channel whitening (for BLE only)\n",
  "\r -Press [q] for channel up\n",
  "\r -Press [w] for channel down\n",
  "\r -Press [a] for Power up\n",
  "\r -Press [s] for Power down\n",
  "\r -Press [n] to increase the Payload\n",
  "\r -Press [m] to decrease the Payload\n",
  "\r -Press [d] to increase the XTAL Trim value\n",
  "\r -Press [f] to decrease the XTAL Trim value\n",
#if defined(gBoard_Antenna_Diversity_Support_d) && (gBoard_Antenna_Diversity_Support_d > 0)
  "\r -press [e] to change FRDM Shield board antenna port\n",  
#endif  
  "\r These keys can be used all over the application to change \n",
  "\r the test parameters\n",
  "\r  ________________________________\n",
  "\r |                                |\n",
  "\r |   Select the Test to perform   |\n",
  "\r |________________________________|\n",
  "\r -Press [1] Continuous tests\n",
  "\r -Press [2] Packet Error Rate test\n",
  "\r -Press [3] Range test\n",
#ifndef RTC_TRIM_DISABLED
  "\r -Press [4] Adjust RTC XTAL\n"
#else
  "\r -Press [4] Not used\n"
#endif
  "\r -Press [5] Adjust PLL numerator offset\n"
#if defined(BOARD_LOCALIZATION_REVISION_SUPPORT) && (BOARD_LOCALIZATION_REVISION_SUPPORT > 0)
  "\r -Press [6] for RF Switch Control\n",
#endif
  "\r -Press [!] Reset MCU\n\r\n",
  NULL
};

/*@CMA, Conn Test. New string*/
char * const cu8ShortCutsBar[] =
{
  "\f\r\n",
  "\r--------------------------------------------------------------\n",
  "\r  [t] Tx  [z] Mode/Rate+  [c] Fix/Ch whit.  [q] Ch+  [a] Pw+  \n",
  "\r  [r] Rx  [x] Mode/Rate-                    [w] Ch-  [s] Pw-  \n",
  "\r                                                              \n",
  "\r  [n] Pyld+  [d] XtalTrim+                                    \n",  
  "\r  [m] Pyld-  [f] XtalTrim-                                    \n",  
#if defined(gBoard_ExtPaSupport_d) && (gBoard_ExtPaSupport_d > 0)
  "\r                                                              \n",
  "\r  [g] Ext. PA gain or bypass  [i] Ext. PA antenna 1 or 2      \n",  
#endif  
#if defined(gBoard_Antenna_Diversity_Support_d) && (gBoard_Antenna_Diversity_Support_d > 0)
  "\r  [e] FRDM Shield antenna selection                           \n",  
#endif
  "\r--------------------------------------------------------------\r\n",
  NULL
};

char * const cu8ContinuousTestMenu[] =
{
  "\r __________________________ \n",
  "\r|                          |\n",
  "\r|   Continuous Test Menu   |\n",
  "\r|__________________________|\n\r\n",
  "\r-Press [1] Idle\n",
  "\r-Press [2] Burst Transmission using random-payload packets\n",
  "\r-Press [3] Continuous Modulated Transmission 1's\n",
  "\r-Press [4] Continuous Modulated Transmission 0's\n",
  "\r-Press [5] Continuous Modulated Transmission pseudo-random\n",
  "\r-Press [6] Continuous Unmodulated Transmission\n",
  "\r-Press [7] Continuous Packet Mode Reception\n",
  "\r-Press [8] Continuous Energy Detect\n",
  "\r-Press [p] Previous Menu\n\r\n",
  NULL
};

char * const cu8PerTxTestMenu[] =
{
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
  "\r  ______________________________________ \n",
  "\r |                                      |\n",
  "\r |      PER Tx Low Power Test Menu      |\n",
  "\r |______________________________________|\n\r\n",
#else
  "\r  ____________________________ \n",
  "\r |                            |\n",
  "\r |      PER Tx Test Menu      |\n",
  "\r |____________________________|\n\r\n",
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
  "\r  Choose the amount of packets to send:\n",
  "\r [0] - 1     Packet     [1] - 25    Packets\n",
  "\r [2] - 100   Packets    [3] - 500   Packets\n",
  "\r [4] - 1000  Packets    [5] - 2000  Packets\n",
  "\r [6] - 5000  Packets    [7] - 10000 Packets\n",
  "\r [8] - 65535 Packets\n\r\n",
  "\r-Press any push button to stop Transmiting Packets\n",
  "\r-Press [p] Previous Menu\n\r\n",
  NULL
};

char * const cu8PerRxTestMenu[] =
{
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
  "\r  ________________________________ \n",
  "\r |                                |\n",
  "\r |   PER Rx Low Power Test Menu   |\n",
  "\r |________________________________|\n\r\n",
#else
  "\r  ______________________ \n",
  "\r |                      |\n",
  "\r |   PER Rx Test Menu   |\n",
  "\r |______________________|\n\r\n",
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
  "\r -Press [space bar] to start/stop Receiving Packets\n",
  "\r -Press any push button to stop Receiving Packets\n",
  "\r -Press [p] Previous Menu\n\r\n",
  NULL
};

char * const cu8RangeTxTestMenu[] =
{
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
  "\r  __________________________________ \n",
  "\r |                                  |\n",
  "\r |   Range Tx Low Power Test Menu   |\n",
  "\r |__________________________________|\n\r\n",
#else
  "\r  ________________________ \n",
  "\r |                        |\n",
  "\r |   Range Tx Test Menu   |\n",
  "\r |________________________|\n\r\n",
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
  "\r -Press [space bar] to start/stop Transmiting Packets\n",
  "\r -Press any push button to stop Transmiting Packets\n",
  "\r -Press [p] Previous Menu\n\r\n",
  NULL
};

char * const cu8RangeRxTestMenu[] =
{
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
  "\r  __________________________________ \n",
  "\r |                                  |\n",
  "\r |   Range Rx Low Power Test Menu   |\n",
  "\r |__________________________________|\n\r\n",
#else
  "\r  ________________________ \n",
  "\r |                        |\n",
  "\r |   Range Rx Test Menu   |\n",
  "\r |________________________|\n\r\n",
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
  "\r -Press [space bar] to start/stop Receiving Packets\n",
  "\r -Press any push button to stop Receiving Packets\n",
  "\r -Press [p] Previous Menu\n\r\n",
  NULL
};

char* const cu8PerRxMessages[] =
{
  "\f\n\rPER Test Rx Running\r\n\r\n",
  "\n\rPER Test Rx failed. Press [ENTER] to continue\r\n\r\n",
  "\n\rPER Test Rx Stopped\r\n\r\n",
  "\n\rPER Test Finished\r\n\r\n",
  "\r\n Press [ENTER] to go back to the Per Rx test menu"
};

char* const cu8PerTxMessages[] =
{
  "\r\n\r\n Please type TX interval in miliseconds ( > 6 ms ) "
  "and press [ENTER]\r\n ",
  "\r\n\tError: TX Interval too small\r\n",
  "\r\n\r\n-TX failed. Press [ENTER] to continue\r\n\r\n",
  "\f\r\n Running PER Tx, Number of packets: ",
  "\r\n PER Tx DONE \r\n""\r\n\r\n Press [ENTER] to go "
  "back to the PER Tx test menu ",
  "\r\n\r\n-TX failed. Press [ENTER] to continue\r\n\r\n",
  "\r\n\r\n-Test interrupted by user. Press [ENTER] to continue\r\n\r\n"
};

char* const cu8RangeRxMessages[] =
{
  "\f\r\nRange Test Rx Running\r\n",
  "\f\r\nRange Test Rx failed\r\n"
  "\r\n\r\n Press [ENTER] to go back to the Range Rx test menu",
  "\n\r\n\rRange Test Rx Stopped\r\n\r\n",
  "\r\n\r\n Press [ENTER] to go back to the Range Rx test menu",
};

char* const cu8RangeTxMessages[] =
{
  "\f\r\nRange Test Tx Running\r\n",
  "\f\r\nRange Test Tx failed\r\n"
  "\r\n\r\n Press [ENTER] to go back to the Range Tx test menu",
  "\r\nPacket Dropped",
  "\n\r\n\rRange Test Tx Stopped\r\n\r\n",
  "\r\n\r\n Press [ENTER] to go back to the Range Tx test menu"
};

char * const cu8ContMessages[] =
{
    "\r\n\r\n Packet Mode PRBS started. Press [SPACE] to stop test\r\n",
    "\r\n\r\n Packet Mode PRBS failed. Press [ENTER] to continue\r\n",
    "\r\n\r\n Packet Mode PRBS stopped. Press [ENTER] to continue\r\n",
    "\r\n\r\n Packet Mode Rx started. Press [SPACE] to stop test\r\n",
    "\r\n\r\n Packet Mode Rx failed. Press [ENTER] to continue\r\n",
    "\r\n\r\n Packet Mode Rx stopped. Press [ENTER] to continue\r\n",
    "\r\n New Packet ",
    " Idle",
    " Mod 1's",
    " Mod 0's",
    " Mod PN",
    " Unmod",
    "\r\n\r\n Continuous ED stopped. Press [ENTER] to continue\r\n",
};

char * const cu8RTCTrimMenu[] =
{
  "\n\r"
  "\r __________________________\n",
  "\r|                          |\n",
  "\r|   Adjust RTC XTAL Menu   |\n",
  "\r|__________________________|\n\r\n",
  "\r-Press [1] Increment XTAL Capacitance Level\n",
  "\r-Press [2] Decrement XTAL Capacitance Level\n",
  "\r-Press [3] Increment EXTAL Capacitance Level\n",
  "\r-Press [4] Decrement EXTAL Capacitance Level\n",
  //"\r-Press [3] Move MCG from FEI to FEE\n",
  //"\r-Press [4] Move MCG from FEE to FEI\n",
  "\r-Press [p] Previous Menu\n\r\n",
  NULL
};

#if defined(BOARD_LOCALIZATION_REVISION_SUPPORT) && (BOARD_LOCALIZATION_REVISION_SUPPORT > 0)
char * const cu8RFSwitchCtrlMenu[] =
{
  "\n\r"
  "\r ____________________________\n",
  "\r|                            |\n",
  "\r|   RF Switch Control Menu   |\n",
  "\r|____________________________|\n\r\n",
  "\n\r",
  "\r---To use SMA1 in RF1 mode, populate R92 and remove R110---\n"
  "\n\r",
  "\r-Press [1] for RF1 (SMA1) mode\n",
  "\r-Press [2] for RF2 (ANT_A) mode\n",
  "\r-Press [3] for RF3 (ANT_B) mode\n",
  "\r-Press [4] for RF4 (SMA2) mode\n",
  "\r-Press [5] for ALL OFF mode\n",
  "\r-Press [p] Previous Menu\n\r\n",
  NULL
};
#endif

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/***********************************************************************************
*
* PrintMenu
*
************************************************************************************/
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
void PrintMenu(char * const pu8Menu[], serial_write_handle_t writeHandle)
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
void PrintMenu(char * const pu8Menu[], uint8_t port)
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
{
  uint8_t u8Index = 0;
  while(pu8Menu[u8Index])
  {
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
    (void)SerialManager_WriteBlocking(writeHandle, (uint8_t *)pu8Menu[u8Index], strlen(pu8Menu[u8Index]));
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
    Serial_Print(port, pu8Menu[u8Index], gAllowToBlock_d);
    (void)port;
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
    u8Index++;
  }
}

/*! *********************************************************************************
* \brief  Prints test paramteres
********************************************************************************** */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
void PrintTestParams(ct_config_params_t* params, bool_t bEraseLine, serial_handle_t port)
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
void PrintTestParams(ct_config_params_t* params, bool_t bEraseLine, uint8_t port)
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
{
    uint8_t u8lineLen = 103;
    uint8_t u8Index;

    if(bEraseLine)
    {
        Serial_Print(port, "\r", gAllowToBlock_d);
        for(u8Index = 0;u8Index<u8lineLen;u8Index++)
        {
            Serial_Print(port, " ", gAllowToBlock_d);
        }
        Serial_Print(port ,"\r", gAllowToBlock_d);
    }
    u8Index = 0;
    while(params[u8Index].paramType != gParamTypeMaxType_c)
    {
       Serial_Print(port, (char*)params[u8Index].paramName, gAllowToBlock_d);
       Serial_Print(port, " ", gAllowToBlock_d);
       if(params[u8Index].paramType == gParamTypeString_c)
       {
           Serial_Print(port,
                        (char*)params[u8Index].paramValue.stringValue,
                        gAllowToBlock_d);
       }
       else if(params[u8Index].paramType == gParamTypeNumber_c)
       {
           Serial_PrintDec(port, params[u8Index].paramValue.decValue);
       }
       else if(params[u8Index].paramType == gParamTypeBool_c)
       {
           if(params[u8Index].paramValue.boolValue)
           {
               Serial_Print(port, "TRUE", gAllowToBlock_d);
           }
           else
           {
               Serial_Print(port, "FALSE", gAllowToBlock_d);
           }
       }
       else if(params[u8Index].paramType == gParamTypeMixed_c)
       {
           Serial_Print(port,
                        (char*)params[u8Index].ptr[params[u8Index].id],
                        gAllowToBlock_d);
       }
       Serial_Print(port, ", ", gAllowToBlock_d);
       ++u8Index;
    }
    Serial_Print(port, "\b\b>", gAllowToBlock_d);
}

/*! *********************************************************************************
* \brief  Prints PER RX related information
********************************************************************************** */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
void PrintPerRxInfo(uint16_t u16PacketsIndex,
                    uint16_t u16ReceivedPackets,
                    ct_rx_indication_t* pIndicationInfo,
                    serial_write_handle_t port)
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
void PrintPerRxInfo(uint16_t u16PacketsIndex,
                    uint16_t u16ReceivedPackets,
                    ct_rx_indication_t* pIndicationInfo,
                    uint8_t port)
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
{
    int32_t i32TempRssiValue = 0;
    i32TempRssiValue = (int32_t)(pIndicationInfo->rssi);

    Serial_Print(port, "Packet ", gAllowToBlock_d);
    Serial_PrintDec(port,(uint32_t)u16ReceivedPackets);
    Serial_Print(port, ". Packet index: ",gAllowToBlock_d);
    Serial_PrintDec(port, (uint32_t)u16PacketsIndex);
    Serial_Print(port, ". Rssi: ", gAllowToBlock_d);
    if(i32TempRssiValue < 0)
    {
        i32TempRssiValue *= -1;
        Serial_Print(port, "-", gAllowToBlock_d);
    }
    Serial_PrintDec(port, (uint32_t)i32TempRssiValue);
    Serial_Print(port, ". Timestamp: ", gAllowToBlock_d);
    Serial_PrintDec(port, (uint32_t)pIndicationInfo->timestamp);
    Serial_Print(port, "\r\n", gAllowToBlock_d);
}

/*! *********************************************************************************
* \brief  Prints PER RX final statistics
********************************************************************************** */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
void PrintPerRxFinal(uint16_t u16Received,
                     uint16_t u16TotalPackets,
                     int8_t i8AvgRssi,
                     serial_write_handle_t port)
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
void PrintPerRxFinal(uint16_t u16Received,
                     uint16_t u16TotalPackets,
                     int8_t i8AvgRssi,
                     uint8_t port)
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
{
    Serial_Print(port,"\r\nAverage Rssi during PER: ",gAllowToBlock_d);

    if(i8AvgRssi< 0)
    {
        i8AvgRssi *= -1;

        Serial_Print(port, "-",gAllowToBlock_d);
    }
    Serial_PrintDec(port, (uint32_t)i8AvgRssi);
    Serial_Print(port, " dBm\r\n",gAllowToBlock_d);
    Serial_Print(port, "Received ", gAllowToBlock_d);
    Serial_PrintDec(port, (uint32_t)u16Received);
    Serial_Print(port, " of ", gAllowToBlock_d);
    Serial_PrintDec(port, (uint32_t)u16TotalPackets);
    Serial_Print(port, " packets transmitted \r\n", gAllowToBlock_d);
}

/*! *********************************************************************************
* \brief  Prints PER TX related information
********************************************************************************** */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
void PrintPerTxPackets(uint16_t u16SentPackets,
                       bool_t bIsValid,
                       serial_write_handle_t port)
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
void PrintPerTxPackets(uint16_t u16SentPackets,
                       bool_t bIsValid,
                       uint8_t port)
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
{
    if(bIsValid)
    {
        Serial_Print(port,"\r\n Packet ",gAllowToBlock_d);
        Serial_PrintDec(port,(uint32_t)u16SentPackets);
    }
    else
    {
        Serial_Print(port,"\r\n (Failed) Packet ",gAllowToBlock_d);
        Serial_PrintDec(port,(uint32_t)u16SentPackets);
    }
}

/*! *********************************************************************************
* \brief  Prints Range TX dropped packets information
********************************************************************************** */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
void PrintRangeTxDroppedPackets(uint16_t u16PacketsDropped, serial_write_handle_t port)
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
void PrintRangeTxDroppedPackets(uint16_t u16PacketsDropped, uint8_t port)
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
{
    Serial_Print(port, "\r\nPackets dropped ", gAllowToBlock_d);
    Serial_PrintDec(port, (uint32_t)u16PacketsDropped);
}

/*! *********************************************************************************
* \brief  Prints Average RSSI information for PER and Range tests
********************************************************************************** */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
void PrintAvgRssi(int8_t i8AverageRssi, serial_write_handle_t port)
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
void PrintAvgRssi(int8_t i8AverageRssi, uint8_t port)
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
{
    Serial_Print(port, "Average RSSI     ", gAllowToBlock_d);
    if(i8AverageRssi < 0)
    {
        Serial_Print(port, "-", gAllowToBlock_d);
        i8AverageRssi *= -1;
    }
    Serial_PrintDec(port, (uint32_t) i8AverageRssi);
    Serial_Print(port," dBm", gAllowToBlock_d);
}

/*! *********************************************************************************
* \brief  Prints packet RSSI for PER and Range tests
********************************************************************************** */
#if (defined(SDK_COMPONENT_INTEGRATION) && (SDK_COMPONENT_INTEGRATION > 0))
void PrintCrtRssi(int8_t i8CrtRssi, serial_write_handle_t port)
#else  /* SDK_COMPONENT_INTEGRATION > 0 */
void PrintCrtRssi(int8_t i8CrtRssi, uint8_t port)
#endif /* SDK_COMPONENT_INTEGRATION > 0 */
{
    Serial_Print(port, "\r\n RSSI = ", gAllowToBlock_d);
    if(i8CrtRssi < 0)
    {
        Serial_Print(port, "-", gAllowToBlock_d);
        i8CrtRssi *= -1;
    }
    Serial_PrintDec(port, (uint32_t)i8CrtRssi);
    Serial_Print(port," dBm", gAllowToBlock_d);
}