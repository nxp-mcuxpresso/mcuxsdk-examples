/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

/*
 * This is the test configuration for evkbmimxrt1170
 */

/*******************************************************************************
 * TEST configuration
 ******************************************************************************/

#ifndef APP_CONFIG
#define APP_CONFIG 0
#endif

#if (APP_CONFIG==0) /* default app config */
#define EXPECTED_PERSON_CONFIDENCE_MIN  50
#define EXPECTED_FACE_CONFIDENCE_MIN    98
#define EXPECTED_NUM_DETECTED_PERSONS   1
#define EXPECTED_NUM_DETECTED_FACES     2
#define EXPECTED_PERSON_MAX_INF_TIME    295
#define EXPECTED_ULTRAFACE_MAX_INF_TIME 70
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */