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
#define EXPECTED_INF_TIME            (40) /* ms */
#define EXPECTED_RECO_PERS           "thispersondoesnotexist_4"
#define EXPECTED_INF_SCORE           80 /* % */
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */
