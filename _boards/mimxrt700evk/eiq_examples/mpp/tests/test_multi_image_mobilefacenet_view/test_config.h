/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_CONFIG_H
#define _TEST_CONFIG_H

#include "mpp_api_types.h"
#include "mobilefacenet_database_infos.h"

/* Input images */
#include "thispersondoesnotexist_4_jpg.h"
#include "thispersondoesnotexist_11_jpg.h"
#include "Serena_Williams_0009_jpg.h"
#include "Albrecht_Mentz_0002_jpg.h"
#include "Sheila_Fraser_0001_jpg.h"
#include "Strom_Thurmond_0003_jpg.h"

/*
 * This is the test configuration for mimxrt700evk
 */

/*******************************************************************************
 * TEST configuration
 ******************************************************************************/
#ifndef APP_CONFIG
#define APP_CONFIG 0
#endif

typedef struct src_image_params_s {
    mpp_img_params_t img_params;
    char img_name[MAX_NAME_SIZE+1];
    void *img_data;
} src_image_params_t;

src_image_params_t img_list[] = {
    {
        .img_params = {
            .format = SRC_IMAGE_THISPERSONDOESNOTEXIST_4_JPG_FORMAT,
            .width = SRC_IMAGE_THISPERSONDOESNOTEXIST_4_JPG_WIDTH,
            .height = SRC_IMAGE_THISPERSONDOESNOTEXIST_4_JPG_HEIGHT,
            .stripe = 0,
            .compressed_size = thispersondoesnotexist_4_jpg_data_len
        },
        .img_name = "thispersondoesnotexist_4",
        .img_data = (void *)thispersondoesnotexist_4_jpg_data
    },
    {
        .img_params = {
            .format = SRC_IMAGE_THISPERSONDOESNOTEXIST_11_JPG_FORMAT,
            .width = SRC_IMAGE_THISPERSONDOESNOTEXIST_11_JPG_WIDTH,
            .height = SRC_IMAGE_THISPERSONDOESNOTEXIST_11_JPG_HEIGHT,
            .stripe = 0,
            .compressed_size = thispersondoesnotexist_11_jpg_data_len
        },
        .img_name = "thispersondoesnotexist_11",
        .img_data = (void *)thispersondoesnotexist_11_jpg_data
    },
    {
        .img_params = {
            .format = SRC_IMAGE_SERENA_WILLIAMS_0009_JPG_FORMAT,
            .width = SRC_IMAGE_SERENA_WILLIAMS_0009_JPG_WIDTH,
            .height = SRC_IMAGE_SERENA_WILLIAMS_0009_JPG_HEIGHT,
            .stripe = 0,
            .compressed_size = serena_williams_0009_jpg_data_len
        },
        .img_name = "Serena_Williams_0009",
        .img_data = (void *)serena_williams_0009_jpg_data
    },
    {
        .img_params = {
            .format = SRC_IMAGE_ALBRECHT_MENTZ_0002_JPG_FORMAT,
            .width = SRC_IMAGE_ALBRECHT_MENTZ_0002_JPG_WIDTH,
            .height = SRC_IMAGE_ALBRECHT_MENTZ_0002_JPG_HEIGHT,
            .stripe = 0,
            .compressed_size = albrecht_mentz_0002_jpg_data_len
        },
        .img_name = "Albrecht_Mentz_0002",
        .img_data = (void *)albrecht_mentz_0002_jpg_data
    },
    {
        .img_params = {
            .format = SRC_IMAGE_SHEILA_FRASER_0001_JPG_FORMAT,
            .width = SRC_IMAGE_SHEILA_FRASER_0001_JPG_WIDTH,
            .height = SRC_IMAGE_SHEILA_FRASER_0001_JPG_HEIGHT,
            .stripe = 0,
            .compressed_size = sheila_fraser_0001_jpg_data_len
        },
        .img_name = "Sheila_Fraser_0001",
        .img_data = (void *)sheila_fraser_0001_jpg_data
    },
    {
        .img_params = {
            .format = SRC_IMAGE_STROM_THURMOND_0003_JPG_FORMAT,
            .width = SRC_IMAGE_STROM_THURMOND_0003_JPG_WIDTH,
            .height = SRC_IMAGE_STROM_THURMOND_0003_JPG_HEIGHT,
            .stripe = 0,
            .compressed_size = strom_thurmond_0003_jpg_data_len
        },
        .img_name = "Strom_Thurmond_0003",
        .img_data = (void *)strom_thurmond_0003_jpg_data
    }
};

#if (APP_CONFIG==0)
/* define persons database */
#define APP_DATABASE_NAME "embeddings_database.h"
#elif (APP_CONFIG==1)
#define APP_DATABASE_NAME "face_database.h"
#define PERSON_REGISTRATION     1
#else
#pragma message "configuration APP_CONFIG value is not supported by test"
#endif

#endif /* _TEST_CONFIG_H */
