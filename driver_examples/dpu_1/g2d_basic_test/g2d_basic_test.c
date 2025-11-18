/*
 * Copyright (c) 2024, Freescale Semiconductor, Inc.
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "fsl_debug_console.h"
#include "fsl_tstmr.h"
#include "g2d.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/
#define TEST_WIDTH         1920
#define TEST_HEIGHT        1080
#define TICKS_PER_USEC     24U
#define TSTMR_BASE         TSTMR2
#define EXPECTED_COLOR_AA   0xAAAAAAAAU
#define EXPECTED_COLOR_55   0x55555555U
#define MAX_THRESHOLD       0xFFFFFFFFU
#define TEST_LOOPS          16
#define STRESS_LOOPS_OPEN   2048
#define STRESS_LOOPS_ALLOC  128
#define BLEND_THRESHOLD     2
#define PRNG_MULTIPLIER     1103515245U
#define PRNG_INCREMENT      12345U
#define PRNG_SHIFT          16U
#define PRNG_MASK           0x7FFFU

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_InitHardware(void);
extern void SCB_EnableDCache(void);


static int32_t run_stress_tests(void *handle);
static int32_t run_yuv_feature_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf,
                                     int32_t test_width, int32_t test_height);
static int32_t run_blending_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf, 
                                  int32_t test_width, int32_t test_height);
static int32_t run_rotation_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf,
                                  int32_t test_width, int32_t test_height);
static int32_t run_copy_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf,
                              int32_t test_width, int32_t test_height);
static int32_t run_resize_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf,
                                int32_t test_width, int32_t test_height);
static int32_t run_clear_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf,
                               int32_t test_width, int32_t test_height);

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void) 
{
    BOARD_InitHardware();
    SCB_EnableDCache();

    void *handle = NULL;
    struct g2d_buf *s_buf = NULL;
    struct g2d_buf *d_buf = NULL;
    int32_t result = 0;
    
    const int32_t test_width  = (int32_t)((TEST_WIDTH + 15U) & (~15U));
    const int32_t test_height = (int32_t)((TEST_HEIGHT + 15U) & (~15U));

    (void)PRINTF("Width: %d, Height: %d\r\n", test_width, test_height);

    if (g2d_open(&handle) != 0) {
        (void)PRINTF("g2d_open failed.\r\n");
        return 0;
    }

    result = run_stress_tests(handle);
    if (result == 0) {
        s_buf = g2d_alloc((uint32_t)(test_height * test_width * 4), 0U);
        d_buf = g2d_alloc((uint32_t)(test_height * test_width * 4), 0U);
        
        if ((s_buf != NULL) && (d_buf != NULL)) {
            result = run_yuv_feature_tests(handle, s_buf, d_buf, test_width, test_height);
            if (result == 0) {
                result = run_blending_tests(handle, s_buf, d_buf, test_width, test_height);
                if (result == 0) {
                    result = run_rotation_tests(handle, s_buf, d_buf, test_width, test_height);
                    if (result == 0) {
                        result = run_clear_tests(handle, s_buf, d_buf, test_width, test_height);
                        if (result == 0) {
                            result = run_resize_tests(handle, s_buf, d_buf, test_width, test_height);
                            if (result == 0) {
                                result = run_copy_tests(handle, s_buf, d_buf, test_width, test_height);
                            }
                        }
                    }
                }
            }
        } else {
            (void)PRINTF("Buffer allocation failed.\r\n");
            result = 1;
        }
        
        if (s_buf != NULL) { 
            (void)g2d_free(s_buf); 
        }
        if (d_buf != NULL) { 
            (void)g2d_free(d_buf); 
        }
    }

    (void)g2d_close(handle);
    (void)PRINTF("G2D basic test done\r\n");
    return result;
}

static void fill_source_buffer(struct g2d_buf* buf, int32_t rows, int32_t cols) 
{
    for (int32_t i = 0; i < rows; i++) {
        for (int32_t j = 0; j < cols; j++) {
            uintptr_t base_addr = (uintptr_t)buf->buf_vaddr;
            uintptr_t offset = (uintptr_t)((i * cols + j) * 4);
            char* p = (char *)(void *)(base_addr + offset);

            p[0] = (char)((i * cols + j) % 255);
            p[1] = p[0];
            p[2] = p[0];
            p[3] = p[0];
        }
    } 
}

static void fill_destination_buffer(struct g2d_buf* buf, int32_t rows, int32_t cols) 
{
    for (int32_t i = 0; i < rows; i++) {
        for (int32_t j = 0; j < cols; j++) {
            uintptr_t base_addr = (uintptr_t)buf->buf_vaddr;
            uintptr_t offset = (uintptr_t)((i * cols + j) * 4);
            char* p = (char *)(void *)(base_addr + offset);

            p[0] = (char)((i * cols + j + 128) % 255);
            p[1] = p[0];
            p[2] = p[0];
            p[3] = p[0];
        }
    } 
}

static void init_surface_common(struct g2d_surface *surface, struct g2d_buf *buf, 
                                int32_t width, int32_t height)
{
    surface->planes[0] = buf->buf_paddr;
    surface->planes[1] = buf->buf_paddr + (uint32_t)(height * width);
    surface->planes[2] = buf->buf_paddr + (uint32_t)(height * width * 2);
    surface->left = 0;
    surface->top = 0;
    surface->right = width;
    surface->bottom = height;
    surface->stride = width;
    surface->width = width;
    surface->height = height;
    surface->rot = G2D_ROTATION_0;
    surface->format = G2D_RGBA8888;
}

/**
 * @brief Get current timestamp from timer
 * @return Current timestamp value
 */
static uint64_t get_timestamp(void)
{
    return TSTMR_ReadTimeStamp(TSTMR_BASE);
}

/**
 * @brief Calculate average test runtime in microseconds
 * @param start Start timestamp
 * @param loops Number of test loops
 * @return Average runtime per loop in microseconds
 */
static uint32_t get_test_runtime(uint64_t start, int32_t loops) 
{
    uint64_t stop = get_timestamp();
    uint64_t runtime = (stop - start) / TICKS_PER_USEC / (uint64_t)loops;
    
    if (runtime > MAX_THRESHOLD) {
        (void)PRINTF("%s: runtime 32-bit print threshold reached\r\n", __FUNCTION__);
    }

    return (uint32_t)runtime;
}

/**
 * @brief Simple PRNG replacement for rand()
 * @return Pseudo-random number 0-32767
 */
static uint32_t simple_prng_state = 12345U;
static uint32_t simple_rand(void) {
    simple_prng_state = (simple_prng_state * PRNG_MULTIPLIER) + PRNG_INCREMENT;
    return (simple_prng_state >> PRNG_SHIFT) & PRNG_MASK;
}

static int32_t run_stress_tests(void *handle)
{
    (void)PRINTF("---------------- g2d_open/close stress test ----------\r\n");
    
    for (int32_t i = 0; i < STRESS_LOOPS_OPEN; i++) {
        if (g2d_open(&handle) != 0) {  
            (void)PRINTF("g2d_open/close stress test fail.\r\n");
            return 1;
        }
        (void)g2d_close(handle);  
    }

    if (g2d_open(&handle) != 0) {
        (void)PRINTF("g2d_open failed.\r\n");
        return 1;
    }

    (void)PRINTF("---------------- g2d_alloc stress test ---------------\r\n");
    
    for (int32_t i = 0; i < STRESS_LOOPS_ALLOC; i++) {
        struct g2d_buf *test_s_buf = g2d_alloc(1024U * 1024U * (uint32_t)((i % 4) + 1), 1U);
        if (test_s_buf != NULL) {
            (void)g2d_free(test_s_buf);
        } else {
            (void)PRINTF("g2d_alloc stress test fail\r\n");
            return 1;
        }

        struct g2d_buf *test_d_buf = g2d_alloc(1024U * 1024U * (uint32_t)((i % 16) + 1), 0U);
        if (test_d_buf != NULL) {
            (void)g2d_free(test_d_buf);
        } else {
            (void)PRINTF("g2d_alloc stress test fail\r\n");
            return 1;
        }
    }

    return 0;
}

static int32_t run_yuv_feature_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf,
                                     int32_t test_width, int32_t test_height)
{
    struct g2d_surface src, dst;
    uint64_t start;
    uint32_t us;
    int g2d_feature_available = 0;

    init_surface_common(&src, s_buf, test_width, test_height);
    init_surface_common(&dst, d_buf, test_width, test_height);

    (void)g2d_query_feature(handle, G2D_DST_YUV, &g2d_feature_available);
    if (g2d_feature_available == 1) {
        (void)PRINTF("---------------- test dst YUV feature ----------------\r\n");

        src.format = G2D_RGBA8888;
        dst.format = G2D_YUYV;

        (void)memset(s_buf->buf_vaddr, 0xcc, (size_t)(test_height * test_width * 4));
        (void)memset(d_buf->buf_vaddr, 0x0, (size_t)(test_height * test_width * 4));

        start = get_timestamp();

        for (int32_t i = 0; i < TEST_LOOPS; i++) {
            (void)g2d_blit(handle, &src, &dst);
        }
        (void)g2d_finish(handle);

        us = get_test_runtime(start, TEST_LOOPS);
        (void)PRINTF("RGBA to YUY2 time %dus, %dfps, %dMpixel/s ........\r\n", us,
                1000000U / us, (uint32_t)(test_height * test_width) / us);
    } else {
        (void)PRINTF("%d: feature not available\r\n", g2d_feature_available);
    }

    return 0;
}

static int32_t validate_blend_clear_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            char *p = (char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if ((p[0] != 0) || (p[0] != p[1]) || (p[0] != p[2]) || (p[0] != p[3])) {
                (void)PRINTF("2d blended r/g/b/a (%d/%d/%d/%d) are not zero in clear mode!\r\n",
                        (int)p[0], (int)p[1], (int)p[2], (int)p[3]);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_src_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            unsigned char Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in SRC mode!\r\n",
                    (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            Co = (unsigned char)((i * test_width + j) % 255);
            Ao = Co;

            if ((Co != p[0]) || (Ao != p[3])) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in SRC mode, Co %d, Ao %d\r\n",
                        (int)p[0], (int)p[3], (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_dst_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            unsigned char Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in DST mode!\r\n",
                            (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            Co = (unsigned char)(((i * test_width + j + 128) % 255));
            Ao = Co;

            if ((Co != p[0]) || (Ao != p[3])) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in DST mode, Co "
                            "%d, Ao %d\r\n",
                            (int)p[0], (int)p[3], (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_src_over_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            unsigned int iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
                PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in SRC OVER "
                        "mode!\r\n", p[0], p[1], p[2]);
            }

            Cs = As = (i * test_width + j) % 255;
            Cd = Ad = ((i * test_width + j + 128) % 255);

            iCo = ((unsigned int)Cs * 255 + (unsigned int)Cd * (255 - As)) / 255;
            iAo = ((unsigned int)As * 255 + (unsigned int)Ad * (255 - As)) / 255;

            if (iCo > 255) {
                Co = 255;
            }
            else {
                Co = (unsigned char)iCo;
            }
            
            if (iAo > 255) {
                Ao = 255;
            }
            else {
                Ao = (unsigned char)iAo;
            }

            // compare the result with +/-1 threshold
            if (abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2) {
                PRINTF("2d blended color(%d) or alpha(%d) is incorrect in SRC OVER "
                        "mode, Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d\r\n",
                        p[0], p[3], Cs, As, Cd, Ad, Co, Ao);
            }
        }
    }
    return 0;
}

static int32_t validate_blend_dst_over_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            uint32_t iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in DST OVER mode!\r\n",
                        (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            Cs = (unsigned char)((i * test_width + j) % 255);
            As = Cs;
            Cd = (unsigned char)(((i * test_width + j + 128) % 255));
            Ad = Cd;

            iCo = (((uint32_t)Cs * (255U - (uint32_t)Ad)) + ((uint32_t)Cd * 255U)) / 255U;
            iAo = (((uint32_t)As * (255U - (uint32_t)Ad)) + ((uint32_t)Ad * 255U)) / 255U;

            Co = (iCo > 255U) ? 255U : (unsigned char)iCo;
            Ao = (iAo > 255U) ? 255U : (unsigned char)iAo;

            if ((abs((int32_t)Co - (int32_t)p[0]) > BLEND_THRESHOLD) || 
                (abs((int32_t)Ao - (int32_t)p[3]) > BLEND_THRESHOLD)) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in DST OVER mode, "
                        "Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d\r\n",
                        (int)p[0], (int)p[3], (int)Cs, (int)As, (int)Cd, (int)Ad, (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_src_in_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            unsigned char Cs, As, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in SRC IN mode!\r\n",
                        (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            Cs = (unsigned char)((i * test_width + j) % 255);
            As = Cs;
            Ad = (unsigned char)(((i * test_width + j + 128) % 255));

            Co = (unsigned char)(((uint32_t)Cs * (uint32_t)Ad) / 255U);
            Ao = (unsigned char)(((uint32_t)As * (uint32_t)Ad) / 255U);

            if ((abs((int32_t)Co - (int32_t)p[0]) > BLEND_THRESHOLD) || 
                (abs((int32_t)Ao - (int32_t)p[3]) > BLEND_THRESHOLD)) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in SRC IN mode, "
                        "Cs %d, As %d, Ad %d, Co %d, Ao %d\r\n",
                        (int)p[0], (int)p[3], (int)Cs, (int)As, (int)Ad, (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_dst_in_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            unsigned char As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in DST IN mode!\r\n",
                        (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            As = (unsigned char)((i * test_width + j) % 255);
            Cd = (unsigned char)(((i * test_width + j + 128) % 255));
            Ad = Cd;

            Co = (unsigned char)(((uint32_t)Cd * (uint32_t)As) / 255U);
            Ao = (unsigned char)(((uint32_t)Ad * (uint32_t)As) / 255U);

            if ((abs((int32_t)Co - (int32_t)p[0]) > BLEND_THRESHOLD) || 
                (abs((int32_t)Ao - (int32_t)p[3]) > BLEND_THRESHOLD)) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in DST IN mode, "
                        "As %d, Cd %d, Ad %d, Co %d, Ao %d\r\n",
                        (int)p[0], (int)p[3], (int)As, (int)Cd, (int)Ad, (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_src_out_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            unsigned char Cs, As, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in SRC OUT mode!\r\n",
                        (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            Cs = (unsigned char)((i * test_width + j) % 255);
            As = Cs;
            Ad = (unsigned char)(((i * test_width + j + 128) % 255));

            Co = (unsigned char)(((uint32_t)Cs * (255U - (uint32_t)Ad)) / 255U);
            Ao = (unsigned char)(((uint32_t)As * (255U - (uint32_t)Ad)) / 255U);

            if ((abs((int32_t)Co - (int32_t)p[0]) > BLEND_THRESHOLD) || 
                (abs((int32_t)Ao - (int32_t)p[3]) > BLEND_THRESHOLD)) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in SRC OUT mode, "
                        "Cs %d, As %d, Ad %d, Co %d, Ao %d\r\n",
                        (int)p[0], (int)p[3], (int)Cs, (int)As, (int)Ad, (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_dst_out_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            unsigned char As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in DST OUT mode!\r\n",
                        (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            As = (unsigned char)((i * test_width + j) % 255);
            Cd = (unsigned char)(((i * test_width + j + 128) % 255));
            Ad = Cd;

            Co = (unsigned char)(((uint32_t)Cd * (255U - (uint32_t)As)) / 255U);
            Ao = (unsigned char)(((uint32_t)Ad * (255U - (uint32_t)As)) / 255U);

            if ((abs((int32_t)Co - (int32_t)p[0]) > BLEND_THRESHOLD) || 
                (abs((int32_t)Ao - (int32_t)p[3]) > BLEND_THRESHOLD)) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in DST OUT mode, "
                        "As %d, Cd %d, Ad %d, Co %d, Ao %d\r\n",
                        (int)p[0], (int)p[3], (int)As, (int)Cd, (int)Ad, (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_src_atop_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            uint32_t iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in SRC ATOP mode!\r\n",
                        (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            Cs = (unsigned char)((i * test_width + j) % 255);
            As = Cs;
            Cd = (unsigned char)(((i * test_width + j + 128) % 255));
            Ad = Cd;

            iCo = (((uint32_t)Cs * (uint32_t)Ad) + ((uint32_t)Cd * (255U - (uint32_t)As))) / 255U;
            iAo = (((uint32_t)As * (uint32_t)Ad) + ((uint32_t)Ad * (255U - (uint32_t)As))) / 255U;

            Co = (iCo > 255U) ? 255U : (unsigned char)iCo;
            Ao = (iAo > 255U) ? 255U : (unsigned char)iAo;

            if ((abs((int32_t)Co - (int32_t)p[0]) > BLEND_THRESHOLD) || 
                (abs((int32_t)Ao - (int32_t)p[3]) > BLEND_THRESHOLD)) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in SRC ATOP mode, "
                        "Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d\r\n",
                        (int)p[0], (int)p[3], (int)Cs, (int)As, (int)Cd, (int)Ad, (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_dst_atop_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            uint32_t iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in DST ATOP mode!\r\n",
                        (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            Cs = (unsigned char)((i * test_width + j) % 255);
            As = Cs;
            Cd = (unsigned char)(((i * test_width + j + 128) % 255));
            Ad = Cd;

            iCo = (((uint32_t)Cs * (255U - (uint32_t)Ad)) + ((uint32_t)Cd * (uint32_t)As)) / 255U;
            iAo = (((uint32_t)As * (255U - (uint32_t)Ad)) + ((uint32_t)Ad * (uint32_t)As)) / 255U;

            Co = (iCo > 255U) ? 255U : (unsigned char)iCo;
            Ao = (iAo > 255U) ? 255U : (unsigned char)iAo;

            if ((abs((int32_t)Co - (int32_t)p[0]) > BLEND_THRESHOLD) || 
                (abs((int32_t)Ao - (int32_t)p[3]) > BLEND_THRESHOLD)) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in DST ATOP mode, "
                        "Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d\r\n",
                        (int)p[0], (int)p[3], (int)Cs, (int)As, (int)Cd, (int)Ad, (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_xor_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            uint32_t iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in XOR mode!\r\n",
                        (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            Cs = (unsigned char)((i * test_width + j) % 255);
            As = Cs;
            Cd = (unsigned char)(((i * test_width + j + 128) % 255));
            Ad = Cd;

            iCo = (((uint32_t)Cs * (255U - (uint32_t)Ad)) + ((uint32_t)Cd * (255U - (uint32_t)As))) / 255U;
            iAo = (((uint32_t)As * (255U - (uint32_t)Ad)) + ((uint32_t)Ad * (255U - (uint32_t)As))) / 255U;

            Co = (iCo > 255U) ? 255U : (unsigned char)iCo;
            Ao = (iAo > 255U) ? 255U : (unsigned char)iAo;

            if ((abs((int32_t)Co - (int32_t)p[0]) > BLEND_THRESHOLD) || 
                (abs((int32_t)Ao - (int32_t)p[3]) > BLEND_THRESHOLD)) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in XOR mode, "
                        "Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d\r\n",
                        (int)p[0], (int)p[3], (int)Cs, (int)As, (int)Cd, (int)Ad, (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_global_alpha_dst_in(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height, 
                                                  unsigned char global_alpha)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in Global Alpha DST IN mode!\r\n",
                        (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            Cs = (unsigned char)((i * test_width + j) % 255);
            As = Cs;
            Cd = (unsigned char)(((i * test_width + j + 128) % 255));
            Ad = Cd;

            Co = (unsigned char)(((uint32_t)Cd * (uint32_t)As * (uint32_t)global_alpha) / (255U * 255U));
            Ao = (unsigned char)(((uint32_t)Ad * (uint32_t)As * (uint32_t)global_alpha) / (255U * 255U));

            if ((abs((int32_t)Co - (int32_t)p[0]) > BLEND_THRESHOLD) || 
                (abs((int32_t)Ao - (int32_t)p[3]) > BLEND_THRESHOLD)) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in Global Alpha DST IN mode, "
                        "Cs %d, As %d, Ad %d, Co %d, Ao %d\r\n",
                        (int)p[0], (int)p[3], (int)Cs, (int)As, (int)Ad, (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_global_alpha_src_over(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height,
                                                    unsigned char global_alpha)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            uint32_t iCo, iAo, iCo_on_pxp, iAo_on_pxp;
            unsigned char Cs, As, Cd, Ad, Co, Ao, Co_on_pxp, Ao_on_pxp;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("2d blended r/g/b values(%d/%d/%d) are not same in Global Alpha SRC OVER mode!\r\n",
                        (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            Cs = (unsigned char)((i * test_width + j) % 255);
            As = Cs;
            Cd = (unsigned char)(((i * test_width + j + 128) % 255));
            Ad = Cd;

            iCo = (((uint32_t)Cs * (uint32_t)global_alpha) + 
                   ((uint32_t)Cd * (255U - ((uint32_t)As * (uint32_t)global_alpha / 255U)))) / 255U;
            iAo = (((uint32_t)As * (uint32_t)global_alpha / 255U) * 255U + 
                   ((uint32_t)Ad * (255U - ((uint32_t)As * (uint32_t)global_alpha / 255U)))) / 255U;

            iCo_on_pxp = ((uint32_t)Cs + 
                          ((uint32_t)Cd * (255U - ((uint32_t)As * (uint32_t)global_alpha / 255U)) / 255U));
            iAo_on_pxp = (((uint32_t)As * (uint32_t)global_alpha / 255U) + 
                          ((uint32_t)Ad * (255U - ((uint32_t)As * (uint32_t)global_alpha / 255U)) / 255U));

            Co = (iCo > 255U) ? 255U : (unsigned char)iCo;
            Ao = (iAo > 255U) ? 255U : (unsigned char)iAo;

            Co_on_pxp = (iCo_on_pxp > 255U) ? 255U : (unsigned char)iCo_on_pxp;
            Ao_on_pxp = (iAo_on_pxp > 255U) ? 255U : (unsigned char)iAo_on_pxp;

            if (((abs((int32_t)Co - (int32_t)p[0]) > BLEND_THRESHOLD) || 
                 (abs((int32_t)Ao - (int32_t)p[3]) > BLEND_THRESHOLD)) && 
                ((abs((int32_t)Co_on_pxp - (int32_t)p[0]) > BLEND_THRESHOLD) || 
                 (abs((int32_t)Ao_on_pxp - (int32_t)p[3]) > BLEND_THRESHOLD))) {
                (void)PRINTF("2d blended color(%d) or alpha(%d) is incorrect in Global Alpha SRC OVER mode "
                        "Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d, global alpha=%d\r\n",
                        (int)p[0], (int)p[3], (int)Cs, (int)As, (int)Cd, (int)Ad, (int)Co, (int)Ao,
                        (int)global_alpha);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_blend_premultiplied_mode(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            uint32_t iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if ((p[0] != p[1]) || (p[0] != p[2])) {
                (void)PRINTF("(%d/%d/%d) not same in premultiplied mode!\r\n", 
                        (int)p[0], (int)p[1], (int)p[2]);
                return 1;
            }

            Cs = (unsigned char)((i * test_width + j) % 255);
            As = Cs;
            Cd = (unsigned char)(((i * test_width + j + 128) % 255));
            Ad = Cd;

            iCo = (((uint32_t)Cs * (uint32_t)As * 255U) + 
                   ((uint32_t)Cd * (uint32_t)Ad * (255U - (uint32_t)As))) / (255U * 255U);
            iAo = (((uint32_t)As * 255U) + ((uint32_t)Ad * (255U - (uint32_t)As))) / 255U;

            Co = (iCo > 255U) ? 255U : (unsigned char)iCo;
            Ao = (iAo > 255U) ? 255U : (unsigned char)iAo;

            if ((abs((int32_t)Co - (int32_t)p[0]) > 4) || (abs((int32_t)Ao - (int32_t)p[3]) > 1)) {
                (void)PRINTF("premult fail: color(%d) alpha(%d), Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d\r\n",
                        (int)p[0], (int)p[3], (int)Cs, (int)As, (int)Cd, (int)Ad, (int)Co, (int)Ao);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t run_blending_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf, 
                                  int32_t test_width, int32_t test_height)
{
    struct g2d_surface src, dst;
    uint64_t start;
    uint32_t us;
    int32_t result = 0;

    init_surface_common(&src, s_buf, test_width, test_height);
    init_surface_common(&dst, d_buf, test_width, test_height);

    fill_source_buffer(s_buf, test_height, test_width);
    fill_destination_buffer(d_buf, test_height, test_width);

    (void)PRINTF("---------------- g2d blit performance ----------------\r\n");
    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("RGBA->RGBA time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    /******** test alpha blending with Porter-Duff modes *************/
    
    /* Clear: alpha blending mode G2D_ZERO, G2D_ZERO */
    fill_destination_buffer(d_buf, test_height, test_width);
    src.blendfunc = G2D_ZERO;
    dst.blendfunc = G2D_ZERO;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_clear_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* SRC: alpha blending mode G2D_ONE, G2D_ZERO */
    fill_destination_buffer(d_buf, test_height, test_width);
    src.blendfunc = G2D_ONE;
    dst.blendfunc = G2D_ZERO;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_src_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* Random rectangle resize with blending mode G2D_ONE, G2D_ZERO */
    (void)memset(s_buf->buf_vaddr, 0x55, (size_t)(test_height * test_width * 4));

    for (int32_t tests = 0; tests < TEST_LOOPS; tests++) {
        (void)memset(d_buf->buf_vaddr, 0xAA, (size_t)(test_height * test_width * 4));

        uint32_t rand_right = (simple_rand() % (uint32_t)test_width);
        if (rand_right == 0U) {
            rand_right = 1U;
        }
        src.right = (int32_t)rand_right;
        dst.right = src.right;
        
        src.left = (int32_t)(simple_rand() % (uint32_t)dst.right);
        dst.left = src.left;
        
        uint32_t rand_bottom = (simple_rand() % (uint32_t)test_height);
        if (rand_bottom == 0U) {
            rand_bottom = 1U;
        }
        src.bottom = (int32_t)rand_bottom;
        dst.bottom = src.bottom;
        
        src.top = (int32_t)(simple_rand() % (uint32_t)dst.bottom);
        dst.top = src.top;

        (void)g2d_enable(handle, G2D_BLEND);
        (void)g2d_blit(handle, &src, &dst);
        (void)g2d_disable(handle, G2D_BLEND);
        (void)g2d_finish(handle);

        /* Validate rectangle blending */
        for (int32_t i = 0; i < test_height; i++) {
            for (int32_t j = 0; j < test_width; j++) {
                int32_t color = *(int32_t *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);
                
                if ((j >= dst.left) && (j < dst.right) && (i >= dst.top) && (i < dst.bottom)) {
                    if (color != (int32_t)EXPECTED_COLOR_55) {
                        (void)PRINTF("[%d, %d] Expected value 0x%x, Real value 0x%x\r\n", j, i,
                                    (int32_t)EXPECTED_COLOR_55, color);
                        return 1;
                    }
                } else {
                    if (color != (int32_t)EXPECTED_COLOR_AA) {
                        (void)PRINTF("[%d, %d] Expected value 0x%x, Real value 0x%x\r\n", j, i,
                                    (int32_t)EXPECTED_COLOR_AA, color);
                        return 1;
                    }
                }
            }
        }
    }
    
    /* Reset surface dimensions */
    init_surface_common(&src, s_buf, test_width, test_height);
    init_surface_common(&dst, d_buf, test_width, test_height);
    (void)PRINTF(". ");

    /* DST: alpha blending mode G2D_ZERO, G2D_ONE */
    fill_source_buffer(s_buf, test_height, test_width);
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ZERO;
    dst.blendfunc = G2D_ONE;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_dst_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* SRC OVER: alpha blending mode G2D_ONE, G2D_ONE_MINUS_SRC_ALPHA */
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ONE;
    dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_src_over_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* DST OVER: alpha blending mode G2D_ONE_MINUS_DST_ALPHA, G2D_ONE */
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ONE_MINUS_DST_ALPHA;
    dst.blendfunc = G2D_ONE;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_dst_over_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* SRC IN: alpha blending mode G2D_DST_ALPHA, G2D_ZERO */
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_DST_ALPHA;
    dst.blendfunc = G2D_ZERO;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_src_in_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* DST IN: alpha blending mode G2D_ZERO, G2D_SRC_ALPHA */
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ZERO;
    dst.blendfunc = G2D_SRC_ALPHA;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_dst_in_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* SRC OUT: alpha blending mode G2D_ONE_MINUS_DST_ALPHA, G2D_ZERO */
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ONE_MINUS_DST_ALPHA;
    dst.blendfunc = G2D_ZERO;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_src_out_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* DST OUT: alpha blending mode G2D_ZERO, G2D_ONE_MINUS_SRC_ALPHA */
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ZERO;
    dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_dst_out_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* SRC ATOP: alpha blending mode G2D_DST_ALPHA, G2D_ONE_MINUS_SRC_ALPHA */
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_DST_ALPHA;
    dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_src_atop_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* DST ATOP: alpha blending mode G2D_ONE_MINUS_DST_ALPHA, G2D_SRC_ALPHA */
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ONE_MINUS_DST_ALPHA;
    dst.blendfunc = G2D_SRC_ALPHA;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_dst_atop_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* XOR: test alpha blending mode G2D_ONE_MINUS_DST_ALPHA, G2D_ONE_MINUS_SRC_ALPHA */
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ONE_MINUS_DST_ALPHA;
    dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_xor_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* Global Alpha: alpha blending mode G2D_ZERO, G2D_SRC_ALPHA */
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ZERO;
    dst.blendfunc = G2D_SRC_ALPHA;

    src.global_alpha = 0xabU;
    dst.global_alpha = 0xffU;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_enable(handle, G2D_GLOBAL_ALPHA);

    (void)g2d_blit(handle, &src, &dst);

    (void)g2d_disable(handle, G2D_GLOBAL_ALPHA);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);
    
    result = validate_blend_global_alpha_dst_in(d_buf, test_width, test_height, 0xabU);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* Global Alpha: alpha blending mode G2D_ONE, G2D_ONE_MINUS_SRC_ALPHA */
    fill_destination_buffer(d_buf, test_height, test_width);  

    src.blendfunc = G2D_ONE;
    dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA;

    src.global_alpha = 0x69U;
    dst.global_alpha = 0xffU;

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_enable(handle, G2D_GLOBAL_ALPHA);

    (void)g2d_blit(handle, &src, &dst);

    (void)g2d_disable(handle, G2D_GLOBAL_ALPHA);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_global_alpha_src_over(d_buf, test_width, test_height, 0x69U);
    if (result != 0) {
        return result;
    }
    (void)PRINTF(". ");

    /* Pre-multiplied & de-multiply test: alpha blending mode G2D_ONE, G2D_ONE_MINUS_SRC_ALPHA */
    fill_destination_buffer(d_buf, test_height, test_width); 

    src.blendfunc = (enum g2d_blend_func)(G2D_ONE | G2D_PRE_MULTIPLIED_ALPHA);
    dst.blendfunc = (enum g2d_blend_func)(G2D_ONE_MINUS_SRC_ALPHA | G2D_PRE_MULTIPLIED_ALPHA);

    (void)g2d_enable(handle, G2D_BLEND);
    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_disable(handle, G2D_BLEND);
    (void)g2d_finish(handle);

    result = validate_blend_premultiplied_mode(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }

    (void)PRINTF(".\r\n");

    /* Test blending performance */
    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_enable(handle, G2D_BLEND);
        (void)g2d_blit(handle, &src, &dst);
        (void)g2d_disable(handle, G2D_BLEND);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("g2d blending time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    return 0;
}

static int32_t validate_rotation_90(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_width; i++) {
        for (int32_t j = 0; j < test_height; j++) {
            int32_t correct_val = (test_height - 1 - j) * test_width + i;
            int32_t rotated_val = *(int32_t *)(((char *)d_buf->buf_vaddr) + (i * test_height + j) * 4);
            if (rotated_val != correct_val) {
                (void)PRINTF("[%d][%d]: 90 rotation value should be %d instead of %d(0x%x)\r\n",
                            i, j, correct_val, rotated_val, (uint32_t)rotated_val);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_rotation_180(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            int32_t correct_val = (test_height - 1 - i) * test_width + (test_width - 1 - j);
            int32_t rotated_val = *(int32_t *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);
            if (rotated_val != correct_val) {
                (void)PRINTF("[%d][%d]: 180 rotation value should be %d instead of %d(0x%x)\r\n",
                    i, j, correct_val, rotated_val, (uint32_t)rotated_val);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_rotation_270(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_width; i++) {
        for (int32_t j = 0; j < test_height; j++) {
            int32_t correct_val = test_width * j + (test_width - 1 - i);
            int32_t rotated_val = *(int32_t *)(((char *)d_buf->buf_vaddr) + (i * test_height + j) * 4);
            if (rotated_val != correct_val) {
                (void)PRINTF("[%d][%d]: 270 rotation value should be %d instead of %d(0x%x)\r\n",
                            i, j, correct_val, rotated_val, (uint32_t)rotated_val);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_flip_h(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            int32_t correct_val = test_width * i + (test_width - 1 - j);
            int32_t rotated_val = *(int32_t *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);
            if (rotated_val != correct_val) {
                (void)PRINTF("[%d][%d]: flip-h value should be %d instead of %d(0x%x)\r\n", i,
                        j, correct_val, rotated_val, (uint32_t)rotated_val);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t validate_flip_v(struct g2d_buf *d_buf, int32_t test_width, int32_t test_height)
{
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            int32_t correct_val = test_width * (test_height - 1 - i) + j;
            int32_t rotated_val = *(int32_t *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);
            if (rotated_val != correct_val) {
                (void)PRINTF("[%d][%d]: flip-v value should be %d instead of %d(0x%x)\r\n", i,
                            j, correct_val, rotated_val, (uint32_t)rotated_val);
                return 1;
            }
        }
    }
    return 0;
}

static int32_t run_rotation_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf,
                                  int32_t test_width, int32_t test_height)
{
    struct g2d_surface src, dst;
    uint64_t start;
    uint32_t us;
    int32_t result = 0;

    (void)PRINTF("---------------- g2d rotation performance ----------------\r\n");
    
    /* Initialize source buffer with test pattern */
    for (int32_t i = 0; i < test_height; i++) {
        for (int32_t j = 0; j < test_width; j++) {
            *(int32_t*)(((char*)s_buf->buf_vaddr) + (i * test_width + j) * 4) =
                i * test_width + j;
        }
    }
    (void)memset(d_buf->buf_vaddr, 0xcd, (size_t)(test_height * test_width * 4));

    init_surface_common(&src, s_buf, test_width, test_height);

    /* 1. 90 degree rotation test */
    dst.planes[0] = d_buf->buf_paddr;
    dst.planes[1] = d_buf->buf_paddr + (uint32_t)(test_height * test_width);
    dst.planes[2] = d_buf->buf_paddr + (uint32_t)(test_height * test_width * 2);
    dst.left = 0;
    dst.top = 0;
    dst.right  = test_height; 
    dst.width  = test_height;
    dst.stride = test_height;
    dst.bottom = test_width;
    dst.height = test_width;
    dst.format = G2D_RGBA8888;
    dst.rot = G2D_ROTATION_90;

    src.rot = G2D_ROTATION_0;

    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_finish(handle);

    result = validate_rotation_90(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }

    start = get_timestamp();
    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("90 rotation time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    /* 2. 270 degree rotation test */
    dst.rot = G2D_ROTATION_270;

    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_finish(handle);

    result = validate_rotation_270(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }

    start = get_timestamp();
    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("270 rotation time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    /* Reset dimensions for 180 degree rotation */
    init_surface_common(&dst, d_buf, test_width, test_height);

    /* 3. 180 degree rotation test */
    dst.rot = G2D_ROTATION_180;

    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_finish(handle);

    result = validate_rotation_180(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }

    start = get_timestamp();
    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);
        
    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("180 rotation time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    /* 4. flip h test */
    dst.rot = G2D_FLIP_H;

    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_finish(handle);

    result = validate_flip_h(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }

    start = get_timestamp();
    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("g2d flip-h time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    /* 5. flip v test */
    dst.rot = G2D_FLIP_V;

    (void)g2d_blit(handle, &src, &dst);
    (void)g2d_finish(handle);

    result = validate_flip_v(d_buf, test_width, test_height);
    if (result != 0) {
        return result;
    }

    start = get_timestamp();
    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("g2d flip-v time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    /* YUV rotation performance tests */
    (void)PRINTF("---------------- g2d YUV rotation performance ----------------\r\n");
    (void)memset(d_buf->buf_vaddr, 0xcd, (size_t)(test_height * test_width * 4));
    
    /* YUV 90 degree rotation */
    dst.right  = test_height; 
    dst.width  = test_height;
    dst.stride = test_height;
    dst.bottom = test_width;
    dst.height = test_width;

    src.format = G2D_YUYV;
    src.rot = G2D_ROTATION_0;

    dst.format = G2D_RGBA8888;
    dst.rot = G2D_ROTATION_90;

    start = get_timestamp();
    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("YUYV 90 rotation time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    /* YUV 270 degree rotation */
    dst.rot = G2D_ROTATION_270;
    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("YUYV 270 rotation time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    return 0;
}

static int32_t run_clear_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf,
                               int32_t test_width, int32_t test_height)
{
    struct g2d_surface dst;
    uint64_t start;
    uint32_t us;

    (void)PRINTF("---------------- g2d clear performance ----------------\r\n");

    init_surface_common(&dst, d_buf, test_width, test_height);

    /* Test clear */
    (void)memset(d_buf->buf_vaddr, 0xcd, (size_t)(test_height * test_width * 4));

    dst.clrcolor = 0xffeeddccU;

    (void)g2d_clear(handle, &dst);
    (void)g2d_finish(handle);

    /* Check if the generated color is correct */
    for (int32_t i = 0; i < (test_height * test_width); i++) {
        int32_t clrcolor = *(int32_t *)(((char *)d_buf->buf_vaddr) + (i * 4));
        if (clrcolor != (int32_t)dst.clrcolor) {
            (void)PRINTF("[%d] Clear color 0x%x, Error color 0x%x\r\n", i, (uint32_t)dst.clrcolor,
                    (uint32_t)clrcolor);
            break;
        }
    }
    (void)PRINTF(". ");

    /* Test random rectangle clear */
    (void)memset(s_buf->buf_vaddr, 0xcd, (size_t)(test_height * test_width * 4));

    dst.format = G2D_RGBA8888;
    dst.clrcolor = 0xffeeddccU;

    for (int32_t tests = 0; tests < TEST_LOOPS; tests++) {
        (void)memset(d_buf->buf_vaddr, 0xcd, (size_t)(test_height * test_width * 4));

        uint32_t rand_right;
        do {   
            rand_right = simple_rand() % (uint32_t)test_width;
        } while (rand_right <= 0U);
        dst.right = (int32_t)rand_right;
        
        uint32_t rand_bottom;
        do {   
            rand_bottom = simple_rand() % (uint32_t)test_height;
        } while (rand_bottom <= 0U);
        dst.bottom = (int32_t)rand_bottom;

        dst.left = (int32_t)(simple_rand() % (uint32_t)dst.right);
        dst.top = (int32_t)(simple_rand() % (uint32_t)dst.bottom);

        (void)g2d_clear(handle, &dst);
        (void)g2d_finish(handle);

        /* Check if the generated color is correct */
        for (int32_t i = 0; i < test_height; i++) {
            for (int32_t j = 0; j < test_width; j++) {
                int32_t clrcolor = *(int32_t *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

                if ((j >= dst.left) && (j < dst.right) && (i >= dst.top) && (i < dst.bottom)) {
                    if (clrcolor != (int32_t)dst.clrcolor) {
                        (void)PRINTF("[%d, %d] Expected value 0x%x, Real color value 0x%x\r\n", j,
                                    i, (uint32_t)dst.clrcolor, (uint32_t)clrcolor);
                        return 1;
                    }
                } else {
                    if (clrcolor != 0xcdcdcdcd) {
                        (void)PRINTF("[%d, %d] Expected value 0x%x, Real color value 0x%x\r\n", j,
                                    i, 0xcdcdcdcdU, (uint32_t)clrcolor);
                        return 1;
                    }
                }
            }
        }
        (void)PRINTF(". ");
    }
    (void)PRINTF("\r\n");

    /* Reset surface dimensions */
    init_surface_common(&dst, d_buf, test_width, test_height);

    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_clear(handle, &dst);
    }
    (void)g2d_finish(handle);
    
    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("g2d clear time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    return 0;
}

static int32_t run_resize_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf,
                                int32_t test_width, int32_t test_height)
{
    struct g2d_surface src, dst;
    uint64_t start;
    uint32_t us;

    (void)PRINTF("---------------- g2d resize test performance ----------------\r\n");
    
    /* Setup source surface for resize */
    src.planes[0] = s_buf->buf_paddr;
    src.planes[1] = s_buf->buf_paddr + (uint32_t)(test_height * test_width);
    src.planes[2] = s_buf->buf_paddr + (uint32_t)(test_height * test_width * 2);
    src.left = 0;
    src.top = 0;
    src.right = (test_width > 1280) ? 1280 : (test_width >> 1);
    src.bottom = (test_height > 720) ? 720 : (test_height >> 1);
    src.stride = (test_width > 1280) ? 1280 : (test_width >> 1);
    src.width = (test_width > 1280) ? 1280 : (test_width >> 1);
    src.height = (test_height > 720) ? 720 : (test_height >> 1);
    src.format = G2D_BGRA8888;
    src.rot = G2D_ROTATION_0;

    init_surface_common(&dst, d_buf, test_width, test_height);

    (void)PRINTF("g2d resize test from %dx%d to %dx%d: \r\n", src.width, src.height, dst.width, dst.height);

    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);
        
    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("resize format from bgra8888 to rgba8888, time %dus, %dfps %dMpixel/s ........\r\n",
            us, 1000000U / us, (uint32_t)(test_height * test_width) / us);

    src.format = G2D_NV12;

    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("resize format from nv12 to rgba8888, time %dus, %dfps, %dMpixel/s ........\r\n",
            us, 1000000U / us, (uint32_t)(test_height * test_width) / us);

    /* Test downscaling */
    init_surface_common(&src, s_buf, test_width, test_height);

    dst.right = (test_width > 1280) ? 1280 : (test_width >> 1);
    dst.bottom = (test_height > 720) ? 720 : (test_height >> 1);
    dst.stride = (test_width > 1280) ? 1280 : (test_width >> 1);
    dst.width = (test_width > 1280) ? 1280 : (test_width >> 1);
    dst.height = (test_height > 720) ? 720 : (test_height >> 1);

    (void)PRINTF("g2d resize test from %dx%d to %dx%d: \r\n", src.width, src.height, dst.width, dst.height);

    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);
        
    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("resize format from bgra8888 to rgba8888, time %dus, %dfps, %dMpixel/s ........\r\n",
            us, 1000000U / us, (uint32_t)(test_height * test_width) / us);

    src.format = G2D_NV12;

    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("resize format from nv12 to rgba8888, time %dus, %dfps, %dMpixel/s ........\r\n",
            us, 1000000U / us, (uint32_t)(test_height * test_width) / us);
    
    /* Reset destination surface */
    init_surface_common(&dst, d_buf, test_width, test_height);

    /* Test cropping */
    src.format = G2D_BGRA8888;

    if ((test_height > 20) && (test_width > 20)) {
        src.left = 10;
        src.top = 10;
        src.right = test_width - 10;
        src.bottom = test_height - 10;
        
        start = get_timestamp();

        for (int32_t i = 0; i < TEST_LOOPS; i++) {
            (void)g2d_blit(handle, &src, &dst);
        }
        (void)g2d_finish(handle);
 
        us = get_test_runtime(start, TEST_LOOPS);
        (void)PRINTF("crop (%d,%d,%d,%d) to %dx%d  time %dus, %dfps, %dMpixel/s ........\r\n",
                src.left, src.top, src.right, src.bottom, 
                dst.width , dst.height, 
                us, 1000000U / us, (uint32_t)(test_height * test_width) / us);

        /* Reset source surface */
        init_surface_common(&src, s_buf, test_width, test_height);
        src.format = G2D_BGRA8888;

        /* Test destination cropping */
        dst.left = 10;
        dst.top = 10;
        dst.right = test_width - 10;
        dst.bottom = test_height - 10;

        start = get_timestamp();

        for (int32_t i = 0; i < TEST_LOOPS; i++) {
            (void)g2d_blit(handle, &src, &dst);
        }
        (void)g2d_finish(handle);
 
        us = get_test_runtime(start, TEST_LOOPS);
        (void)PRINTF("crop %dx%d to (%d,%d,%d,%d) time %dus, %dfps, %dMpixel/s ........\r\n",
                src.width, src.height,
                dst.left, dst.top, dst.right, dst.bottom, 
                us, 1000000U / us, (uint32_t)(test_height * test_width) / us);

        /* Reset destination surface */
        init_surface_common(&dst, d_buf, test_width, test_height);
    } else {
        (void)PRINTF("test window size is too small for cropping\r\n");
    }

    /* Test resize with rotation */
    src.left = 0;
    src.top = 0;
    src.right = (test_width > 1280) ? 1280 : (test_width >> 1);
    src.bottom = (test_height > 720) ? 720 : (test_height >> 1);
    src.stride = (test_width > 1280) ? 1280 : (test_width >> 1);
    src.width = (test_width > 1280) ? 1280 : (test_width >> 1);
    src.height = (test_height > 720) ? 720 : (test_height >> 1);

    dst.rot = G2D_ROTATION_90;
    dst.left = 0;
    dst.top = 0;
    dst.right =  test_width;
    dst.bottom = test_height;
    dst.stride = test_width;
    dst.width =  test_width;
    dst.height = test_height;

    (void)PRINTF("g2d 90 rotation with resize from %dx%d to %dx%d: \r\n", src.width, src.height,
            dst.width, dst.height);

    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("rotation with resize format from bgra8888 to rgba8888, time %dus, %dfps, %dMpixel/s ........\r\n",
                us, 1000000U / us, (uint32_t)(test_height * test_width) / us);

    /* Test resize with rotation - destination resize */
    init_surface_common(&src, s_buf, test_width, test_height);
    src.format = G2D_BGRA8888;

    dst.rot = G2D_ROTATION_90;
    dst.right = (test_width > 1280) ? 1280 : (test_width >> 1);
    dst.bottom = (test_height > 720) ? 720 : (test_height >> 1);
    dst.stride = (test_width > 1280) ? 1280 : (test_width >> 1);
    dst.width = (test_width > 1280) ? 1280 : (test_width >> 1);
    dst.height = (test_height > 720) ? 720 : (test_height >> 1);
    dst.format = G2D_RGBA8888;

    (void)PRINTF("g2d 90 rotation with resize test from %dx%d to %dx%d: \r\n", src.width, src.height,
            dst.width, dst.height);

    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_blit(handle, &src, &dst);
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("rotation with resize format from bgra8888 to rgba8888, time %dus, %dfps, %dMpixel/s ........\r\n",
                us, 1000000U / us, (uint32_t)(test_height * test_width) / us);

    return 0;
}

static int32_t run_copy_tests(void *handle, struct g2d_buf *s_buf, struct g2d_buf *d_buf,
                              int32_t test_width, int32_t test_height)
{
    uint64_t start;
    uint32_t us;

    (void)PRINTF("---------------- g2d copy & cache performance ----------------\r\n");

    /* Test g2d_copy functionality */
    (void)memset(s_buf->buf_vaddr, 0xab, (size_t)(test_height * test_width * 4));
    (void)memset(d_buf->buf_vaddr, 0xcd, (size_t)(test_height * test_width * 4));

    (void)g2d_copy(handle, d_buf, s_buf, (uint32_t)(test_height * test_width * 4));
    (void)g2d_finish(handle);

    if (memcmp(s_buf->buf_vaddr, d_buf->buf_vaddr, (size_t)(test_height * test_width * 4)) != 0) {
        (void)PRINTF("g2d_copy: dst buffer is not copied from src buffer correctly !\r\n");
        return 1;
    }

    /* Test g2d_copy performance */
    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)g2d_copy(handle, d_buf, s_buf, (uint32_t)(test_height * test_width * 4));
    }
    (void)g2d_finish(handle);

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("g2d copy non-cacheable time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    /* Test CPU copy performance for comparison */
    start = get_timestamp();

    for (int32_t i = 0; i < TEST_LOOPS; i++) {
        (void)memcpy(d_buf->buf_vaddr, s_buf->buf_vaddr, (size_t)(test_height * test_width * 4));
    }

    us = get_test_runtime(start, TEST_LOOPS);
    (void)PRINTF("cpu copy non-cacheable time %dus, %dfps, %dMpixel/s ........\r\n", us,
            1000000U / us, (uint32_t)(test_height * test_width) / us);

    return 0;
}

#ifdef __ICCARM__
/**
 * @brief Custom errno implementation for IAR compiler
 * @return Pointer to static errno variable
 */
int *__errno(void) {
    static int no = 0;
    return &no;
}
#endif
