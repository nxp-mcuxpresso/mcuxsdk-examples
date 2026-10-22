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
 * Title:       Test framework type definitions and test macros header file
 *
 * -----------------------------------------------------------------------------
 */


#ifndef __CMSIS_DV_TYPEDEFS_H__
#define __CMSIS_DV_TYPEDEFS_H__

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

typedef unsigned int    BOOL;

#ifndef __TRUE
 #define __TRUE         1
#endif
#ifndef __FALSE
 #define __FALSE        0
#endif

#ifndef ENABLED
 #define ENABLED        1
#endif
#ifndef DISABLED
 #define DISABLED       0
#endif

#ifndef NULL
 #ifdef __cplusplus              // EC++
  #define NULL          0
 #else
  #define NULL          ((void *) 0)
 #endif
#endif

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

/* Test group info macro */
#define TEST_GROUP_INFO(info)                   __tg_info (info)

/* Test macros */
#define TEST_FAIL()                             TEST_FAIL_MESSAGE(NULL)
#define TEST_FAIL_MESSAGE(message)              __set_result (__FILE__, __LINE__, message, FAILED)
#define TEST_PASS()                             TEST_PASS_MESSAGE(NULL)
#define TEST_PASS_MESSAGE(message)              __set_result (__FILE__, __LINE__, message, PASSED)

#define TEST_ASSERT(condition)                  TEST_ASSERT_MESSAGE(condition,"[FAILED]")
#define TEST_ASSERT_MESSAGE(condition,message)  if (condition) { __set_result (__FILE__, __LINE__, NULL, PASSED); } else { __set_result (__FILE__, __LINE__, message, FAILED); }

#define TEST_MESSAGE(message)                   __set_message(__FILE__, __LINE__, message)

#endif /* __CMSIS_DV_TYPEDEFS_H__ */
