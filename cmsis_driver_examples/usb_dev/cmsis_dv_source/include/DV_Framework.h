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
 * Title:       Test framework header file
 *
 * -----------------------------------------------------------------------------
 */


#ifndef __CMSIS_DV_FRAMEWORK_H__
#define __CMSIS_DV_FRAMEWORK_H__

#include "DV_Typedefs.h"
#include "DV_Report.h"

/*-----------------------------------------------------------------------------
 * Test framework global definitions
 *----------------------------------------------------------------------------*/

/* Test case definition macro                                                 */
#define TCD(x, y) { (((y) != 0) ? (x) : (NULL)), #x }

/* Test case description structure                                            */
typedef struct {
  void (*TestFunc)(void);             /* Test function                        */
  const char *TFName;                 /* Test function name string            */
} const TEST_CASE;

/* Test group description structure                                           */
typedef struct {
  const char *FileName;               /* Test module file name                */
  const char *Date;                   /* Compilation date                     */
  const char *Time;                   /* Compilation time                     */
  const char *ReportTitle;            /* Title or name of module under test   */
  void (*Init)(void);                 /* Init function callback               */
  void (*Uninit)(void);               /* Uninit function callback             */

  TEST_CASE *TC;                      /* Array of test cases                  */
  uint32_t NumOfTC;                   /* Number of test cases (sz of TC array)*/

} const TEST_GROUP;

/* Defined in user test module                                                */
extern TEST_GROUP ts[];
extern uint32_t   tg_cnt;

#endif /* __CMSIS_DV_FRAMEWORK_H__ */
