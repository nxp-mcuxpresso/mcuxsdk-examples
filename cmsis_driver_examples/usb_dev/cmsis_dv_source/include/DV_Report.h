/*
 * Copyright (c) 2015-2021 Arm Limited. All rights reserved.
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
 * Title:       Report statistics and layout header file
 *
 * -----------------------------------------------------------------------------
 */


#ifndef __CMSIS_DV_REPORT_H__
#define __CMSIS_DV_REPORT_H__

#include "DV_Typedefs.h"

/*-----------------------------------------------------------------------------
 * Test report global definitions
 *----------------------------------------------------------------------------*/
 
/* Test case result definition */
typedef enum {
  PASSED = 0,
  FAILED,
  NOT_EXECUTED
} TC_RES;

/* Test group statistics */
typedef struct {
  uint32_t idx;                         /* Group index                        */
  uint32_t tests;                       /* Total test cases count             */
  uint32_t passed;                      /* Total test cases passed            */
  uint32_t failed;                      /* Total test cases failed            */
} TEST_GROUP_RESULTS;

/* Test report interface */
typedef struct {
  void (* tr_Init)    (void);
  void (* tr_Uninit)  (void);
  void (* tg_Init)    (const char *title, const char *date, const char *time, const char *file);
  void (* tg_Info)    (const char *info);
  void (* tg_InfoDone)(void);
  void (* tg_Uninit)  (void);
  void (* tc_Init)    (uint32_t num, const char *fn);
  void (* tc_Detail)  (const char *module, uint32_t line, const char *message);
  void (* tc_Uninit)  (void);
  void (* as_Result)  (TC_RES res);
} REPORT_ITF;

/* Global structure for interfacing test report */
extern REPORT_ITF ritf;

extern void __tg_info    (const char *info);

/* Assertions and test results */
extern void __set_result (const char *module, uint32_t line, const char *message, TC_RES res);
extern void __set_message(const char *module, uint32_t line, const char *message);

#endif /* __CMSIS_DV_REPORT_H__ */
