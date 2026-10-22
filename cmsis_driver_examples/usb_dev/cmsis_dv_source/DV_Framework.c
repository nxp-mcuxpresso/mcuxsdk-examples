/*
 * Copyright (c) 2015-2020 Arm Limited. All rights reserved.
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
 * Title:       Test framework
 *
 * -----------------------------------------------------------------------------
 */


#include "cmsis_dv.h" 
#include "DV_Framework.h"

/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/
/**
\defgroup dv_framework Framework
\brief Framework configuration and functions
\details

\defgroup framework_funcs Functions
\ingroup dv_framework

@{
*/

/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/
/**
\brief Close the debug session.
\details
Debug session dead end - debug script should close session here. This function is called by \ref cmsis_dv.
*/
void closeDebug(void);

#ifndef __DOXYGEN__                     // Exclude form the documentation
void __attribute__((noinline)) closeDebug(void){
  __NOP();
  // Test completed
}
#endif


/*=======0=========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1====*/
/**
\brief This is the entry point of the test framework.
\details
Program flow:
  -# Test report is initialized
  -# For each test group following steps are executed:
    -# Test group initialization is called (custom test group initialization)
    -# Test group header is written to standard output 
    -# All tests in a group are executed as follows:
        - Test statistics are initialized
        - Test report header is written to the standard output
        - Test function is executed
        - Test results are written to the standard output
        - Test report footer is written to the standard output
    -# Test group footer is written to standard output 
    -# Test group uninitialization is called (custom test group uninitialization)
  -# Debug session ends when closeDebug function is reached
*/
void cmsis_dv (void *argument) {
  const char *fn;
  uint32_t    i, tc, no;

  (void)argument;

  if (tg_cnt != 0U) {                   /* If at least 1 test is enabled      */

    ritf.tr_Init ();                    /* Init test report                   */

    for (i = 0U; i < tg_cnt; i++) {

                                        /* Init test group report             */
      ritf.tg_Init(ts[i].ReportTitle,   /* Write test group title             */
                   ts[i].Date,          /* Write test group compilation date  */
                   ts[i].Time,          /* Write test group compilation time  */
                   ts[i].FileName);     /* Write test group module file name  */

      if (ts[i].Init != NULL) {
        ts[i].Init();                   /* Init test group (group setup)      */
      }

      ritf.tg_InfoDone();               /* Test group info done               */

      /* Execute all tests in a group */
      for (tc = 0U; tc < ts[i].NumOfTC; tc++) {
        no = tc + 1U;                   /* Test number                        */
        fn = ts[i].TC[tc].TFName;       /* Test function name string          */
        ritf.tc_Init (no, fn);          /* Init test report #(Base + TC)      */
        if (ts[i].TC[tc].TestFunc != NULL) {
          ts[i].TC[tc].TestFunc();      /* Execute test func if enabled       */
        }
        ritf.tc_Uninit ();              /* Uninit test report                 */
      }

      ritf.tg_Uninit ();                /* Uninit test group report           */

      if (ts[i].Uninit != NULL) {
        ts[i].Uninit();                 /* Uninit test group (group teardown) */
      }
    }

    ritf.tr_Uninit();                   /* Uninit test report                 */
  }

  closeDebug();                         /* Close debug session                */
}

/**
@}
*/ 
// end of group framework_funcs
