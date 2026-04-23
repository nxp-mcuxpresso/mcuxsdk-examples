/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

/*
 * This is the test configuration for mimxrt700evk
 */

#ifndef APP_CONFIG
#define APP_CONFIG 0
#endif

#if (APP_CONFIG==0) /* default app config */
#define EXPECTED_INF_TIME            (8) /* ms */
#define EXPECTED_INF_DETECTION_CNT   2
#define EXPECTED_INF_SCORE           76 /* % */
#elif (APP_CONFIG==1)
#define EXPECTED_INF_TIME            (25) /* ms */
#define EXPECTED_INF_DETECTION_CNT   2
#define EXPECTED_INF_SCORE           80 /* % */
#define USE_SCRFD_320_256_MODEL
#elif (APP_CONFIG==2)
#define EXPECTED_INF_TIME            (21) /* ms */
#define EXPECTED_INF_DETECTION_CNT   2
#define EXPECTED_INF_SCORE           75 /* % */
#define USE_SCRFD_256_256_MODEL
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */
