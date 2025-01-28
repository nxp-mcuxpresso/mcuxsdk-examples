/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

/*
 * This is the test configuration for evkbmimxrt1070
 */

/*******************************************************************************
 * TEST configuration
 ******************************************************************************/
#define IMG_stopwatch 0

/* Set to the image type used for testing. */
#ifndef IMAGE_TYPE
#define IMAGE_TYPE IMG_stopwatch
#endif

#pragma message "No test configuration is supported for this application and for this board""

#endif /* _TEST_CONFIG_H */
