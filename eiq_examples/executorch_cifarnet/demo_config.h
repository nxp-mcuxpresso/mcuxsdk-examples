/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DEMO_CONFIG_H_
#define _DEMO_CONFIG_H_

#define EXAMPLE_NAME       "CIFARNET"
#define FRAMEWORK_NAME     "ExecuTorch"
#define DETECTION_TRESHOLD 60
#define NUM_RESULTS        1
#define DEMO_VERBOSE       false
#define OUTPUT_MIN_RANGE   -128  // min value of int8_t
#define CONFIDENCE_SCALE   2.56f // Scale factor to convert uint8 [0-255] to percentage [0-100]

#endif // _DEMO_CONFIG_H_
