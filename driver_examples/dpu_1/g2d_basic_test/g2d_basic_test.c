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

/*******************************************************************************
 * Definitions
 *******************************************************************************/
#define TEST_WIDTH         1920
#define TEST_HEIGHT        1080
#define TICKS_PER_USEC     24U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);

uint32_t hal_print(const char *format, ...);
#define g2d_printf hal_print

int hal_get_current_time(void);

enum g2d_format
{
    G2D_RGB565               = 0,
    G2D_RGBA8888             = 1,
    G2D_RGBX8888             = 2,
    G2D_BGRA8888             = 3,
    G2D_BGRX8888             = 4,
    G2D_BGR565               = 5,

    G2D_ARGB8888             = 6,
    G2D_ABGR8888             = 7,
    G2D_XRGB8888             = 8,
    G2D_XBGR8888             = 9,
    G2D_RGB888               = 10,
    G2D_BGR888               = 11,

    G2D_RGBA5551             = 12,
    G2D_RGBX5551             = 13,
    G2D_BGRA5551             = 14,
    G2D_BGRX5551             = 15,

    G2D_RGBA1010102          = 16,

    G2D_GRAY8                = 19,

    G2D_NV12                 = 20,
    G2D_I420                 = 21,
    G2D_YV12                 = 22,
    G2D_NV21                 = 23,
    G2D_YUYV                 = 24,
    G2D_YVYU                 = 25,
    G2D_UYVY                 = 26,
    G2D_VYUY                 = 27,
    G2D_NV16                 = 28,
    G2D_NV61                 = 29,
};

enum g2d_blend_func
{
    G2D_ZERO                  = 0,
    G2D_ONE                   = 1,
    G2D_SRC_ALPHA             = 2,
    G2D_ONE_MINUS_SRC_ALPHA   = 3,
    G2D_DST_ALPHA             = 4,
    G2D_ONE_MINUS_DST_ALPHA   = 5,
    G2D_PRE_MULTIPLIED_ALPHA  = 0x10,
    G2D_DEMULTIPLY_OUT_ALPHA  = 0x20,
};

enum g2d_cap_mode
{
    G2D_BLEND                 = 0,
    G2D_DITHER                = 1,
    G2D_GLOBAL_ALPHA          = 2,
    G2D_BLEND_DIM             = 3,
    G2D_BLUR                  = 4,
    G2D_YUV_BT_601            = 5,
    G2D_YUV_BT_709            = 6,
    G2D_YUV_BT_601FR          = 7,
    G2D_YUV_BT_709FR          = 8,
    G2D_WARPING               = 9,
};

enum g2d_feature
{
    G2D_SCALING               = 0,
    G2D_ROTATION,
    G2D_SRC_YUV,
    G2D_DST_YUV,
    G2D_MULTI_SOURCE_BLT,
    G2D_FAST_CLEAR,
    G2D_WARP_DEWARP,
};

enum g2d_rotation
{
    G2D_ROTATION_0            = 0,
    G2D_ROTATION_90           = 1,
    G2D_ROTATION_180          = 2,
    G2D_ROTATION_270          = 3,
    G2D_FLIP_H                = 4,
    G2D_FLIP_V                = 5,
};

enum g2d_cache_mode
{
    G2D_CACHE_CLEAN           = 0,
    G2D_CACHE_FLUSH           = 1,
    G2D_CACHE_INVALIDATE      = 2,
};

enum g2d_status
{
    G2D_STATUS_FAIL           =-1,
    G2D_STATUS_OK             = 0,
    G2D_STATUS_NOT_SUPPORTED  = 1,
};

typedef unsigned int g2d_phys_addr_t;

struct g2d_surface
{
    enum g2d_format format;
    g2d_phys_addr_t planes[3];
    int left;
    int top;
    int right;
    int bottom;
    int stride;
    int width;
    int height;
    enum g2d_blend_func blendfunc;
    int global_alpha;
    int clrcolor;
    enum g2d_rotation rot;
};

struct g2d_buf
{
    void *buf_handle;
    void *buf_vaddr;
    g2d_phys_addr_t buf_paddr;
    int  buf_size;
};

int g2d_open(void **handle);
int g2d_close(void *handle);
int g2d_clear(void *handle, struct g2d_surface *area);
int g2d_blit(void *handle, struct g2d_surface *src, struct g2d_surface *dst);
int g2d_copy(void *handle, struct g2d_buf *d, struct g2d_buf* s, int size);
int g2d_enable(void *handle, enum g2d_cap_mode cap);
int g2d_disable(void *handle, enum g2d_cap_mode cap);
int g2d_cache_op(struct g2d_buf *buf, enum g2d_cache_mode op);
struct g2d_buf *g2d_alloc(int size, int cacheable);
int g2d_free(struct g2d_buf *buf);
int g2d_finish(void *handle);
int g2d_query_feature(void *handle, enum g2d_feature feature, int *available);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/

static void fill_source_buffer(struct g2d_buf* buf, int rows, int cols) 
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
        {
            char* p = (char *)(((char *)buf->buf_vaddr) + (i * cols + j) * 4);
            p[0] = p[1] = p[2] = p[3] = ((i * cols + j) % 255);
        }
    } 
}

static void fill_destination_buffer(struct g2d_buf* buf, int rows, int cols) 
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) 
        {
            char* p = (char *)(((char *)buf->buf_vaddr) + (i * cols + j) * 4);
            p[0] = p[1] = p[2] = p[3] = ((i * cols + j + 128) % 255);
        }
    } 
}

// get avg. runtime of a looped G2D test (microseconds)
static uint32_t get_test_runtime(uint64_t start, const int loops) 
{
    uint64_t stop = hal_get_current_time();
    if (stop < start) {
        g2d_printf("%s: tick 64 bit wraparound detected\n", __FUNCTION__);
    }

    // convert clock ticks to microseconds
    uint64_t runtime = (stop - start) / TICKS_PER_USEC / loops;
    if (runtime > 0xFFFFFFFF) {
        g2d_printf("%s: runtime 32-bit print threshold reached\n", __FUNCTION__);
    }

    return (uint32_t)runtime;
}


// test G2D operations on the Blit Engine
int main(void) 
{
    BOARD_InitHardware();

    void *handle = NULL;
    int i, j;
    struct g2d_buf *s_buf, *d_buf;
    struct g2d_surface src, dst;
    uint64_t start;
    uint32_t us;
    int g2d_feature_available = 0;
    const int test_loops = 16;

    // set the test window dimensions
    const int test_width  = (TEST_WIDTH + 15) & ~15;
    const int test_height = (TEST_HEIGHT + 15) & ~15;

    g2d_printf("Width: %d, Height: %d\n", 
                test_width, test_height);

    const int frame_pixels = test_height * test_width;
    const int frame_bytes = frame_pixels * 4;

    g2d_printf("---------------- g2d_open/close stress test ----------\n");
    for (i = 0; i < 2048; i++) 
    {
        if (g2d_open(&handle)) {  
            g2d_printf("g2d_open/close stress test fail.\n");
            return 0;
        }
        g2d_close(handle);  
    }

    if (g2d_open(&handle)) {
        g2d_printf("g2d_open failed.\n");
        return 0;
    }

    g2d_printf("---------------- g2d_alloc stress test ---------------\n");
    for (i = 0; i < 128; i++) 
    {
        s_buf = g2d_alloc(1024 * 1024 * ((i % 4) + 1), 1);
        if (s_buf) {
            g2d_free(s_buf);
        } else {
            g2d_printf("g2d_alloc stress test fail\n");
        }

        d_buf = g2d_alloc(1024 * 1024 * ((i % 16) + 1), 0);
        if (d_buf) {
            g2d_free(d_buf);
        } else {
            g2d_printf("g2d_alloc stress test fail\n");
        }
    }

    s_buf = g2d_alloc(frame_bytes, 0);
    d_buf = g2d_alloc(frame_bytes, 0);

    src.planes[0] = s_buf->buf_paddr;
    src.planes[1] = s_buf->buf_paddr + frame_pixels;
    src.planes[2] = s_buf->buf_paddr + frame_pixels * 2;
    src.left = 0;
    src.top = 0;
    src.right = test_width;
    src.bottom = test_height;
    src.stride = test_width;
    src.width = test_width;
    src.height = test_height;
    src.rot = G2D_ROTATION_0;
    src.format = G2D_RGBA8888;

    dst.planes[0] = d_buf->buf_paddr;
    dst.planes[1] = d_buf->buf_paddr + frame_pixels;
    dst.planes[2] = d_buf->buf_paddr + frame_pixels * 2;
    dst.left = 0;
    dst.top = 0;
    dst.right = test_width;
    dst.bottom = test_height;
    dst.stride = test_width;
    dst.width = test_width;
    dst.height = test_height;
    dst.rot = G2D_ROTATION_0;
    dst.format = G2D_RGBA8888;

    g2d_query_feature(handle, G2D_DST_YUV, &g2d_feature_available);
    if (g2d_feature_available == 1) {
        g2d_printf("---------------- test dst YUV feature ----------------\n");

        src.format = G2D_RGBA8888;
        dst.format = G2D_YUYV;

        memset(s_buf->buf_vaddr, 0xcc, frame_bytes);
        memset(d_buf->buf_vaddr, 0x0, frame_bytes);

        // time started
        start = hal_get_current_time();

        for (i = 0; i < test_loops; i++) {
            g2d_blit(handle, &src, &dst);
        }
        g2d_finish(handle);

        // average microseconds elapsed
        us = get_test_runtime(start, test_loops);

        g2d_printf("RGBA to YUY2 time %dus, %dfps, %dMpixel/s ........\n", us,
                1000000 / us, frame_pixels / us);

    }
    else {
        g2d_printf("%d: feature not available\n", g2d_feature_available);
    }

    src.format = G2D_RGBA8888;
    dst.format = G2D_RGBA8888;

    fill_source_buffer(s_buf, test_height, test_width);
    fill_destination_buffer(d_buf, test_height, test_width);

    g2d_printf("---------------- g2d blit performance ----------------\n");
    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf("RGBA->RGBA time %dus, %dfps, %dMpixel/s ........\n", us,
            1000000 / us, frame_pixels / us);


    /******** test alpha blending with Porter-Duff modes *************/
    
    // Clear: alpha blending mode G2D_ZERO, G2D_ZERO
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ZERO;
    dst.blendfunc = G2D_ZERO;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            char *p = (char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if (p[0] != 0 || p[0] != p[1] || p[0] != p[2] || p[0] != p[3]) {
                g2d_printf("2d blended r/g/b/a (%d/%d/%d/%d) are not zero in clear mode!\n",
                        p[0], p[1], p[2], p[3]);
            }
        }
    }
    g2d_printf(". ");

    // SRC: alpha blending mode G2D_ONE, G2D_ZERO
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ONE;
    dst.blendfunc = G2D_ZERO;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned char Cs, As, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
                g2d_printf("2d blended r/g/b values(%d/%d/%d) are not same in SRC mode!\n",
                    p[0], p[1], p[2]);
            }

            Co = Ao = Cs = As = (i * test_width + j) % 255;

            if (Co != p[0] || Ao != p[3]) {
                g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in SRC mode, Co %d, Ao %d\n",
                        p[0], p[3], Co, Ao);
            }
        }
    }
    g2d_printf(". ");

    // Random rectangle resize with blending mode G2D_ONE, G2D_ZERO
    memset(s_buf->buf_vaddr, 0x55, frame_bytes);

    for (int tests = 0; tests < test_loops; tests++) 
    {
        memset(d_buf->buf_vaddr, 0xAA, frame_bytes);

        // resize bit block within test window
        src.right = dst.right = rand() % test_width;
        src.left = dst.left = rand() % dst.right;
        src.bottom = dst.bottom = rand() % test_height;
        src.top = dst.top = rand() % dst.bottom;

        g2d_enable(handle, G2D_BLEND);
        g2d_blit(handle, &src, &dst);
        g2d_disable(handle, G2D_BLEND);
        g2d_finish(handle);

        // Expect the data in dst buffer to be changed only within the rectangle
        for (i = 0; i < test_height; i++) {
            for (j = 0; j < test_width; j++)  {

                int color = *(int *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);
                
                // if pixel is within rectangle
                if ((j >= dst.left) && (j < dst.right) && (i >= dst.top) && (i < dst.bottom)) 
                {
                    // color should be same as source buffer
                    if (color != 0x55555555) 
                    {
                        g2d_printf("[%d, %d] Expected value 0x%x, Real value 0x%x\n", j, i,
                                    0x55555555, color);
                    }
                } 
                // otherwise, color should be unchanged
                else {
                    if (color != 0xAAAAAAAA) 
                    {
                        g2d_printf("[%d, %d] Expected value 0x%x, Real value 0x%x\n", j, i,
                                    0xAAAAAAAA, color);
                    }
                }
            }
        }
    }
    
    src.left = dst.left = 0;
    src.top = dst.top = 0;
    src.right = dst.right = test_width;
    src.bottom = dst.bottom = test_height;
    g2d_printf(". ");

    // DST: alpha blending mode G2D_ZERO, G2D_ONE
    fill_source_buffer(s_buf, test_height, test_width);
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ZERO;
    dst.blendfunc = G2D_ONE;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned char Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
                g2d_printf("2d blended r/g/b values(%d/%d/%d) are not same in DST mode!\n",
                            p[0], p[1], p[2]);
            }

            Co = Ao = Cd = Ad = ((i * test_width + j + 128) % 255);

            if (Co != p[0] || Ao != p[3]) {
                g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in DST mode, Co "
                            "%d, Ao %d\n",
                            p[0], p[3], Co, Ao);
                
            }
        }
    }
    g2d_printf(". ");

    // SRC OVER: alpha blending mode G2D_ONE, G2D_ONE_MINUS_SRC_ALPHA
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ONE;
    dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned int iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
                g2d_printf("2d blended r/g/b values(%d/%d/%d) are not same in SRC OVER "
                            "mode!\n",
                            p[0], p[1], p[2]);
            }

            Cs = As = (i * test_width + j) % 255;
            Cd = Ad = ((i * test_width + j + 128) % 255);

            iCo = ((unsigned int)Cs * 255 + (unsigned int)Cd * (255 - As)) / 255;
            iAo = ((unsigned int)As * 255 + (unsigned int)Ad * (255 - As)) / 255;

            if (iCo > 255)
                Co = 255;
            else
                Co = (unsigned char)iCo;

            if (iAo > 255)
                Ao = 255;
            else
                Ao = (unsigned char)iAo;

            // compare the result with +/-1 threshold
            if (abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2) {
                g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in SRC OVER "
                        "mode, Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d\n",
                        p[0], p[3], Cs, As, Cd, Ad, Co, Ao);
            }
        }
    }
    g2d_printf(". ");

    // DST OVER: alpha blending mode G2D_ONE_MINUS_DST_ALPHA, G2D_ONE
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ONE_MINUS_DST_ALPHA;
    dst.blendfunc = G2D_ONE;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned int iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
            g2d_printf("2d blended r/g/b values(%d/%d/%d) are not same in DST OVER "
                    "mode!\n",
                    p[0], p[1], p[2]);
                break;
            }

            Cs = As = (i * test_width + j) % 255;
            Cd = Ad = ((i * test_width + j + 128) % 255);

            iCo = ((unsigned int)Cs * (255 - Ad) + (unsigned int)Cd * 255) / 255;
            iAo = ((unsigned int)As * (255 - Ad) + (unsigned int)Ad * 255) / 255;

            if (iCo > 255)
            Co = 255;
            else
            Co = (unsigned char)iCo;

            if (iAo > 255)
            Ao = 255;
            else
            Ao = (unsigned char)iAo;

            // compare the result with +/-1 threshold
            if (abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2) {
            g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in DST OVER "
                    "mode, Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d\n",
                    p[0], p[3], Cs, As, Cd, Ad, Co, Ao);
            }
        }
    }
    g2d_printf(". ");

    // SRC IN: alpha blending mode G2D_DST_ALPHA, G2D_ZERO
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_DST_ALPHA;
    dst.blendfunc = G2D_ZERO;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned char Cs, As, Ad, Co, Ao;
            char *p = (char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
                g2d_printf("2d blended r/g/b values(%d/%d/%d) are not same in SRC IN mode!\n",
                p[0], p[1], p[2]);
            }

            Cs = As = (i * test_width + j) % 255;
            Ad = ((i * test_width + j + 128) % 255);

            Co = (unsigned char)(((unsigned int)Cs * Ad) / 255);
            Ao = (unsigned char)(((unsigned int)As * Ad) / 255);

            // compare the result with +/-1 threshold
            if (abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2) {
                g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in SRC IN mode, "
                    "Cs %d, As %d, Ad %d, Co %d, Ao %d\n",
                    p[0], p[3], Cs, As, Ad, Co, Ao);
            }
        }
    }
    g2d_printf(". ");

    // DST IN: alpha blending mode G2D_ZERO, G2D_SRC_ALPHA
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ZERO;
    dst.blendfunc = G2D_SRC_ALPHA;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned char As, Cd, Ad, Co, Ao;
            char *p = (char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
            g2d_printf(
                "2d blended r/g/b values(%d/%d/%d) are not same in DST IN mode!\n",
                p[0], p[1], p[2]);
            }

            As = (i * test_width + j) % 255;
            Cd = Ad = ((i * test_width + j + 128) % 255);

            Co = (unsigned char)(((unsigned int)Cd * As) / 255);
            Ao = (unsigned char)(((unsigned int)Ad * As) / 255);

            // compare the result with +/-1 threshold
            if (abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2) {
            g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in DST IN mode, "
                    "As %d, Cd %d, Ad %d, Co %d, Ao %d\n",
                    p[0], p[3], As, Cd, Ad, Co, Ao);
            }
        }
    }
    g2d_printf(". ");

    // SRC OUT: alpha blending mode G2D_ONE_MINUS_DST_ALPHA, G2D_ZERO
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ONE_MINUS_DST_ALPHA;
    dst.blendfunc = G2D_ZERO;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned char Cs, As, Ad, Co, Ao;
            char *p = (char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
            g2d_printf(
                "2d blended r/g/b values(%d/%d/%d) are not same in SRC OUT mode!\n",
                p[0], p[1], p[2]);
            }

            Cs = As = (i * test_width + j) % 255;
            Ad = ((i * test_width + j + 128) % 255);

            Co = (unsigned char)(((unsigned int)Cs * (255 - Ad)) / 255);
            Ao = (unsigned char)(((unsigned int)As * (255 - Ad)) / 255);

            // compare the result with +/-1 threshold
            if (abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2) {
            g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in SRC OUT "
                    "mode, Cs %d, As %d, Ad %d, Co %d, Ao %d\n",
                    p[0], p[3], Cs, As, Ad, Co, Ao);
            }
        }
    }
    g2d_printf(". ");

    // DST OUT: alpha blending mode G2D_ZERO, G2D_ONE_MINUS_SRC_ALPHA
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ZERO;
    dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned char As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
            g2d_printf(
                "2d blended r/g/b values(%d/%d/%d) are not same in DST OUT mode!\n",
                p[0], p[1], p[2]);
            }

            As = (i * test_width + j) % 255;
            Cd = Ad = ((i * test_width + j + 128) % 255);

            Co = (unsigned char)(((unsigned int)Cd * (255 - As)) / 255);
            Ao = (unsigned char)(((unsigned int)Ad * (255 - As)) / 255);

            // compare the result with +/-1 threshold
            if (abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2) {
            g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in DST OUT "
                    "mode, As %d, Cd %d, Ad %d, Co %d, Ao %d\n",
                    p[0], p[3], As, Cd, Ad, Co, Ao);
            }
        }
    }
    g2d_printf(". ");

    // SRC ATOP: alpha blending mode G2D_DST_ALPHA, G2D_ONE_MINUS_SRC_ALPHA
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_DST_ALPHA;
    dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned int iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
            g2d_printf("2d blended r/g/b values(%d/%d/%d) are not same in SRC ATOP "
                    "mode!\n",
                    p[0], p[1], p[2]);
            }

            Cs = As = (i * test_width + j) % 255;
            Cd = Ad = ((i * test_width + j + 128) % 255);

            iCo = ((unsigned int)Cs * Ad + (unsigned int)Cd * (255 - As)) / 255;
            iAo = ((unsigned int)As * Ad + (unsigned int)Ad * (255 - As)) / 255;

            if (iCo > 255)
            Co = 255;
            else
            Co = (unsigned char)iCo;

            if (iAo > 255)
            Ao = 255;
            else
            Ao = (unsigned char)iAo;

            // compare the result with +/-1 threshold
            if (abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2) {
            g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in SRC ATOP "
                    "mode, Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d\n",
                    p[0], p[3], Cs, As, Cd, Ad, Co, Ao);
            }
        }
    }
    g2d_printf(". ");

    // DST ATOP: alpha blending mode G2D_ONE_MINUS_DST_ALPHA, G2D_SRC_ALPHA
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ONE_MINUS_DST_ALPHA;
    dst.blendfunc = G2D_SRC_ALPHA;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned int iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
            g2d_printf("2d blended r/g/b values(%d/%d/%d) are not same in DST ATOP "
                    "mode!\n",
                    p[0], p[1], p[2]);
            }

            Cs = As = (i * test_width + j) % 255;
            Cd = Ad = ((i * test_width + j + 128) % 255);

            iCo = ((unsigned int)Cs * (255 - Ad) + (unsigned int)Cd * As) / 255;
            iAo = ((unsigned int)As * (255 - Ad) + (unsigned int)Ad * As) / 255;

            if (iCo > 255)
            Co = 255;
            else
            Co = (unsigned char)iCo;

            if (iAo > 255)
            Ao = 255;
            else
            Ao = (unsigned char)iAo;

            // compare the result with +/-1 threshold
            if (abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2) {
            g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in DST ATOP "
                    "mode, Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d\n",
                    p[0], p[3], Cs, As, Cd, Ad, Co, Ao);
            }
        }
    }
    g2d_printf(". ");

    // XOR: test alpha blending mode G2D_ONE_MINUS_DST_ALPHA, G2D_ONE_MINUS_SRC_ALPHA 
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ONE_MINUS_DST_ALPHA;
    dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned int iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
                g2d_printf("2d blended r/g/b values(%d/%d/%d) are not same in XOR mode!\n",
                        p[0], p[1], p[2]);
            }

            Cs = As = (i * test_width + j) % 255;
            Cd = Ad = ((i * test_width + j + 128) % 255);

            iCo =
                ((unsigned int)Cs * (255 - Ad) + (unsigned int)Cd * (255 - As)) / 255;
            iAo =
                ((unsigned int)As * (255 - Ad) + (unsigned int)Ad * (255 - As)) / 255;

            if (iCo > 255)
                Co = 255;
            else
                Co = (unsigned char)iCo;

            if (iAo > 255)
                Ao = 255;
            else
                Ao = (unsigned char)iAo;

            // compare the result with +/-1 threshold
            if (abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2) 
            {
                g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in XOR mode, Cs "
                            "%d, As %d, Cd %d, Ad %d, Co %d, Ao %d\n",
                            p[0], p[3], Cs, As, Cd, Ad, Co, Ao);
            }
        }
    }
    g2d_printf(". ");

    // Global Alpha: alpha blending mode G2D_ZERO, G2D_SRC_ALPHA
    fill_destination_buffer(d_buf, test_height, test_width);

    src.blendfunc = G2D_ZERO;
    dst.blendfunc = G2D_SRC_ALPHA;

    src.global_alpha = 0xab;
    dst.global_alpha = 0xff;

    g2d_enable(handle, G2D_BLEND);
    g2d_enable(handle, G2D_GLOBAL_ALPHA);

    g2d_blit(handle, &src, &dst);

    g2d_disable(handle, G2D_GLOBAL_ALPHA);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);
    
    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) +
                                                (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
                g2d_printf("2d blended r/g/b values(%d/%d/%d) are not same in DST IN mode!\n",
                            p[0], p[1], p[2]);
            }

            Cs = As = (i * test_width + j) % 255;
            Cd = Ad = ((i * test_width + j + 128) % 255);

            Co = (unsigned char)(((unsigned int)Cd * As * src.global_alpha) /
                                (255 * 255));
            Ao = (unsigned char)(((unsigned int)Ad * As * src.global_alpha) /
                                (255 * 255));

            // compare the result with +/-1 threshold
            if ((abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2)) {
                g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in DST IN mode, "
                            "Cs %d, As %d, Ad %d, Co %d, Ao %d\n",
                            p[0], p[3], Cs, As, Ad, Co, Ao);
            }
        }
    }
    g2d_printf(". ");

    // Global Alpha: alpha blending mode G2D_ONE, G2D_ONE_MINUS_SRC_ALPHA
    fill_destination_buffer(d_buf, test_height, test_width);  

    src.blendfunc = G2D_ONE;
    dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA;

    src.global_alpha = 0x69;
    dst.global_alpha = 0xff;

    g2d_enable(handle, G2D_BLEND);
    g2d_enable(handle, G2D_GLOBAL_ALPHA);

    g2d_blit(handle, &src, &dst);

    g2d_disable(handle, G2D_GLOBAL_ALPHA);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            unsigned int iCo, iAo, iCo_on_pxp, iAo_on_pxp;
            unsigned char Cs, As, Cd, Ad, Co, Ao, Co_on_pxp, Ao_on_pxp;

            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
                g2d_printf("2d blended r/g/b values(%d/%d/%d) are not same in SRC IN mode!\n",
                    p[0], p[1], p[2]);
            }

            Cs = As = ((i * test_width + j) % 255);
            Cd = Ad = ((i * test_width + j + 128) % 255);

            iCo = ((unsigned int)Cs * src.global_alpha +
                    (unsigned int)Cd * (255 - (As * src.global_alpha / 255))) / 255;
            iAo = ((unsigned int)(As * src.global_alpha / 255) * 255 +
                    (unsigned int)Ad * (255 - (As * src.global_alpha / 255))) / 255;

            iCo_on_pxp = ((unsigned int)Cs +
                            (unsigned int)Cd * (255 - (As * src.global_alpha / 255))/255);
            iAo_on_pxp = ((unsigned int)(As * src.global_alpha / 255) +
                            (unsigned int)Ad * (255 - (As * src.global_alpha / 255)) / 255);

            Co = (iCo > 255) ? 255 : (unsigned char)iCo;
            Ao = (iAo > 255) ? 255 : (unsigned char)iAo;

            Co_on_pxp = (iCo_on_pxp > 255) ? 255 : (unsigned char)iCo_on_pxp;
            Ao_on_pxp = (iAo_on_pxp > 255) ? 255 : (unsigned char)iAo_on_pxp;

            // compare the result with +/-1 threshold
            if ((abs(Co - p[0]) > 2 || abs(Ao - p[3]) > 2) && 
                (abs(Co_on_pxp - p[0]) > 2 || abs(Ao_on_pxp - p[3]) > 2)) 
          {
                g2d_printf("2d blended color(%d) or alpha(%d) is incorrect in SRC IN mode Cs %d, As %d, "
                            "Cd %d, Ad %d, Co %d, Ao %d, global alpha=%d\n",
                            p[0], p[3], Cs, As, Cd, Ad, Co, Ao,
                            src.global_alpha);
            }
        }
    }
    g2d_printf(". ");

    // Pre-multipied & de-muliply test: alpha blending mode G2D_ONE, G2D_ONE_MINUS_SRC_ALPHA 
    fill_destination_buffer(d_buf, test_height, test_width); 

    src.blendfunc = G2D_ONE | G2D_PRE_MULTIPLIED_ALPHA;
    dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA | G2D_PRE_MULTIPLIED_ALPHA;

    g2d_enable(handle, G2D_BLEND);
    g2d_blit(handle, &src, &dst);
    g2d_disable(handle, G2D_BLEND);
    g2d_finish(handle);

    for (int i = 0; i < test_height; i++) {
        for (int j = 0; j < test_width; j++) {
            unsigned int iCo, iAo;
            unsigned char Cs, As, Cd, Ad, Co, Ao;
            unsigned char *p = (unsigned char *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

            if (p[0] != p[1] || p[0] != p[2]) {
                g2d_printf("(%d/%d/%d) not same in premultiped mode!\n", p[0], p[1], p[2]);
            }

            Cs = As = (i * test_width + j) % 255;
            Cd = Ad = ((i * test_width + j + 128) % 255);

            iCo = ((unsigned int)Cs * As * 255 + (unsigned int)Cd * Ad * (255 - As)) / (255 * 255);
            iAo = ((unsigned int)As * 255 + (unsigned int)Ad * (255 - As)) / 255;

            if (iCo > 255)
                Co = 255;
            else
                Co = (unsigned char)iCo;

            if (iAo > 255)
                Ao = 255;
            else
                Ao = (unsigned char)iAo;

            // compare the result with +/-2 threshold
            if (abs(Co - p[0]) > 4 || abs(Ao - p[3]) > 1) 
            {
                g2d_printf("premult fail: color(%d) alpha(%d), Cs %d, As %d, Cd %d, Ad %d, Co %d, Ao %d\n",
                        p[0], p[3], Cs, As, Cd, Ad, Co, Ao);
            }
        }
    }

    g2d_printf(".\n");

    // test blending performance
    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_enable(handle, G2D_BLEND);

        g2d_blit(handle, &src, &dst);

        g2d_disable(handle, G2D_BLEND);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf("g2d blending time %dus, %dfps, %dMpixel/s ........\n", us,
            1000000 / us, frame_pixels / us);

    g2d_printf("---------------- g2d clear performance ----------------\n");

    /* Test clear */
    memset(d_buf->buf_vaddr, 0xcd, frame_bytes);

    dst.clrcolor = 0xffeeddcc;

    g2d_clear(handle, &dst);
    g2d_finish(handle);

    // check if the generated color is correct
    for (i = 0; i < frame_pixels; i++) {
        int clrcolor = *(int *)(((char *)d_buf->buf_vaddr) + i * 4);
        if (clrcolor != dst.clrcolor) {
            g2d_printf("[%d] Clear color 0x%x, Error color 0x%x\n", i, dst.clrcolor,
                    clrcolor);
            break;
        }
    }
    g2d_printf(". ");

    /* Test random rectangle clear */
    memset(s_buf->buf_vaddr, 0xcd, frame_bytes);

    dst.format = G2D_RGBA8888;
    dst.clrcolor = 0xffeeddcc;

    for (int tests = 0; tests < test_loops; tests++) 
    {
        memset(d_buf->buf_vaddr, 0xcd, frame_bytes);

        do
        {   
            dst.right = rand() % test_width;
        } while (dst.right <= 0);
        do
        {   
            dst.bottom = rand() % test_height;
        } while (dst.bottom <= 0);

        dst.left = rand() % dst.right;
        dst.top = rand() % dst.bottom;

        g2d_clear(handle, &dst);
        g2d_finish(handle);

        // check if the generated color is correct

        for (i = 0; i < test_height; i++) {

            for (j = 0; j < test_width; j++) {
    

                int clrcolor = *(int *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);

                if ((j >= dst.left) && (j < dst.right) && (i >= dst.top) && (i < dst.bottom)) 
                {
                    if (clrcolor != dst.clrcolor) {
                        g2d_printf("[%d, %d] Expected value 0x%x, Real color value 0x%x\n", j,
                                    i, dst.clrcolor, clrcolor);
    
                    }
                } 
                else {
                    if (clrcolor != 0xcdcdcdcd) {
                        g2d_printf("[%d, %d] Expected value 0x%x, Real color value 0x%x\n", j,
                                    i, 0xcdcdcdcd, clrcolor);
    
                    }
                }
            }
        }
        g2d_printf(". ");
    }
    g2d_printf("\n");

    dst.left = src.left;
    dst.right = src.right; 
    dst.top = src.top;
    dst.bottom = src.bottom;

    
    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_clear(handle, &dst);
    }
    g2d_finish(handle);
    
    us = get_test_runtime(start, test_loops);
    g2d_printf("g2d clear time %dus, %dfps, %dMpixel/s ........\n", us,
            1000000 / us, frame_pixels / us);


    g2d_printf("---------------- g2d rotation performance ----------------\n");
    for (int i = 0; i < test_height; i++) {
        for (int j = 0; j < test_width; j++) 
        {
            *(int*)(((char*)s_buf->buf_vaddr) + (i * test_width + j) * 4) =
                i * test_width + j;
        }
    }
    memset(d_buf->buf_vaddr, 0xcd, frame_bytes);

    src.left = 0;
    src.top = 0;
    src.right = test_width;
    src.bottom = test_height;
    src.stride = test_width;
    src.width = test_width;
    src.height = test_height;

    // swap dst dimensions for 90/270 rotation
    dst.left = 0;
    dst.top = 0;
    dst.right  = test_height; 
    dst.width  = test_height;
    dst.stride = test_height;
    dst.bottom = test_width;
    dst.height = test_width;

    // 1. 90 degree rotation test
    src.rot = G2D_ROTATION_0;
    dst.rot = G2D_ROTATION_90;

    g2d_blit(handle, &src, &dst);
    g2d_finish(handle);

    for (i = 0; i < test_width; i++) {
        for (j = 0; j < test_height; j++) {
            int correct_val = (test_height - 1 - j) * test_width + i;
            int rotated_val = *(int *)(((char *)d_buf->buf_vaddr) + (i * test_height + j) * 4);
            if (rotated_val != correct_val) 
            {
                g2d_printf("[%d][%d]: 90 rotation value should be %d instead of %d(0x%x)\n",
                            i, j, correct_val, rotated_val, rotated_val);
            }
        }
    }

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf("90 rotation time %dus, %dfps, %dMpixel/s ........\n", us,
            1000000 / us, frame_pixels / us);

    // 2. 270 degree rotation test
    src.rot = G2D_ROTATION_0;
    dst.rot = G2D_ROTATION_270;

    g2d_blit(handle, &src, &dst);
    g2d_finish(handle);

    for (i = 0; i < test_width; i++) {
        for (j = 0; j < test_height; j++) {
            int correct_val = test_width * j + (test_width - 1 - i);
            int rotated_val =
                *(int *)(((char *)d_buf->buf_vaddr) + (i * test_height + j) * 4);
            if (rotated_val != correct_val) {
                g2d_printf("[%d][%d]: 270 rotation value should be %d instead of %d(0x%x)\n",
                            i, j, correct_val, rotated_val, rotated_val);
            }
        }
    }

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf("270 rotation time %dus, %dfps, %dMpixel/s ........\n", us,
            1000000 / us, frame_pixels / us);

    // reset dimensions for the other rotation modes
    dst.right  = test_width; 
    dst.width  = test_width;
    dst.stride = test_width;
    dst.bottom = test_height;
    dst.height = test_height;

    // 3. 180 degree rotation test
    src.rot = G2D_ROTATION_0;
    dst.rot = G2D_ROTATION_180;

    g2d_blit(handle, &src, &dst);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            int correct_val = (test_height - 1 - i) * test_width + (test_width - 1 - j);
            int rotated_val = *(int *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);
            if (rotated_val != correct_val) {
                g2d_printf(
                    "[%d][%d]: 180 rotation value should be %d instead of %d(0x%x)\n",
                    i, j, correct_val, rotated_val, rotated_val);
            }
        }
    }

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);
        
    us = get_test_runtime(start, test_loops);
    g2d_printf("180 rotation time %dus, %dfps, %dMpixel/s ........\n", us,
            1000000 / us, frame_pixels / us);

    // 4. flip h test
    src.rot = G2D_ROTATION_0;
    dst.rot = G2D_FLIP_H;

    g2d_blit(handle, &src, &dst);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            int correct_val = test_width * i + (test_width - 1 - j);
            int rotated_val =
                *(int *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);
            if (rotated_val != correct_val) {
                g2d_printf("[%d][%d]: flip-h value should be %d instead of %d(0x%x)\n", i,
                        j, correct_val, rotated_val, rotated_val);
            }
        }
    }

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf("g2d flip-h time %dus, %dfps, %dMpixel/s ........\n", us,
            1000000 / us, frame_pixels / us);

    // 5. flip v test
    src.rot = G2D_ROTATION_0;
    dst.rot = G2D_FLIP_V;

    g2d_blit(handle, &src, &dst);
    g2d_finish(handle);

    for (i = 0; i < test_height; i++) {
        for (j = 0; j < test_width; j++) {
            int correct_val = test_width * (test_height - 1 - i) + j;
            int rotated_val = *(int *)(((char *)d_buf->buf_vaddr) + (i * test_width + j) * 4);
            if (rotated_val != correct_val) {
                g2d_printf("[%d][%d]: flip-v value should be %d instead of %d(0x%x)\n", i,
                            j, correct_val, rotated_val, rotated_val);
            }
        }
    }

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf("g2d flip-v time %dus, %dfps, %dMpixel/s ........\n", us,
            1000000 / us, frame_pixels / us);

    g2d_printf("---------------- g2d YUV rotation performance ----------------\n");
    memset(d_buf->buf_vaddr, 0xcd, frame_bytes);
    
    // YUV 90 degree rotation
    dst.right  = test_height; 
    dst.width  = test_height;
    dst.stride = test_height;
    dst.bottom = test_width;
    dst.height = test_width;

    src.format = G2D_YUYV;
    src.rot = G2D_ROTATION_0;

    dst.format = G2D_RGBA8888;
    dst.rot = G2D_ROTATION_90;

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf("YUYV 90 rotation time %dus, %dfps, %dMpixel/s ........\n", us,
            1000000 / us, frame_pixels / us);

    // YUV 270 degree rotation
    dst.rot = G2D_ROTATION_270;
    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf("YUYV 270 rotation time %dus, %dfps, %dMpixel/s ........\n", us,
            1000000 / us, frame_pixels / us);

    g2d_printf("---------------- g2d resize test performance ----------------\n");
    // reformat and resize source
    src.left = 0;
    src.top = 0;
    src.right = (test_width > 1280) ? 1280 : test_width >> 1;
    src.bottom = (test_height > 720) ? 720 : test_height >> 1;
    src.stride = (test_width > 1280) ? 1280 : test_width >> 1;
    src.width = (test_width > 1280) ? 1280 : test_width >> 1;
    src.height = (test_height > 720) ? 720 : test_height >> 1;
    src.format = G2D_BGRA8888;
    src.rot = G2D_ROTATION_0;

    dst.left = 0;
    dst.top = 0;
    dst.right = test_width;
    dst.bottom = test_height;
    dst.stride = test_width;
    dst.width = test_width;
    dst.height = test_height;
    dst.format = G2D_RGBA8888;
    dst.rot = G2D_ROTATION_0;

    g2d_printf("g2d resize test from %dx%d to %dx%d: \n", src.width, src.height, dst.width, dst.height);

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) 
    {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);
        
    us = get_test_runtime(start, test_loops);
    g2d_printf(" -> bgra8888 to rgba8888 time %dus, %dfps %dMpixel/s ........\n", 
            us, 1000000 / us, frame_pixels / us);


    src.format = G2D_NV12;

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf(" -> nv12 to rgba8888 time %dus, %dfps, %dMpixel/s ........\n", 
            us, 1000000 / us, frame_pixels / us);

    src.right = test_width;
    src.bottom = test_height;
    src.stride = test_width;
    src.width = test_width;
    src.height = test_height;

    // reformat and resize destination
    dst.right = (test_width > 1280) ? 1280 : test_width >> 1;
    dst.bottom = (test_height > 720) ? 720 : test_height >> 1;
    dst.stride = (test_width > 1280) ? 1280 : test_width >> 1;
    dst.width = (test_width > 1280) ? 1280 : test_width >> 1;
    dst.height = (test_height > 720) ? 720 : test_height >> 1;

    g2d_printf("g2d resize test from %dx%d to %dx%d: \n", src.width, src.height, dst.width, dst.height);

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) 
    {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);
        
    us = get_test_runtime(start, test_loops);
    g2d_printf(" -> bgra8888 to rgba8888 time %dus, %dfps, %dMpixel/s ........\n", 
            us, 1000000 / us, frame_pixels / us);


    src.format = G2D_NV12;

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);


    us = get_test_runtime(start, test_loops);
    g2d_printf(" -> nv12 to rgba8888 time %dus, %dfps, %dMpixel/s ........\n",
            us, 1000000 / us, frame_pixels / us);
    
    dst.right = test_width;
    dst.bottom = test_height;
    dst.stride = test_width;
    dst.width = test_width;
    dst.height = test_height;

    // reformat and crop source
    src.format = G2D_BGRA8888;

    if ((test_height > 20) && (test_width > 20)) {
        src.left = 10;
        src.top = 10;
    }
    else {
        g2d_printf("test window size is too small for cropping\n");
    }
    src.right = test_width - 10;
    src.bottom = test_height - 10;
    
    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);
 
    us = get_test_runtime(start, test_loops);
    g2d_printf("crop (%d,%d,%d,%d) to %dx%d  time %dus, %dfps, %dMpixel/s ........\n",
            src.left, src.top, src.right, src.bottom, 
            dst.width , dst.height, 
            us, 1000000 / us, frame_pixels / us);

    src.left = 0;
    src.top = 0;
    src.right = test_width;
    src.bottom = test_height;

    // reformat and crop destination
    if ((test_height > 20) && (test_width > 20)) {
        dst.left = 10;
        dst.top = 10;
    }
    dst.right = test_width - 10;
    dst.bottom = test_height - 10;

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);
 
    us = get_test_runtime(start, test_loops);
    g2d_printf("crop %dx%d to (%d,%d,%d,%d) time %dus, %dfps, %dMpixel/s ........\n",
            src.width, src.height,
            dst.left, dst.top, dst.right, dst.bottom, 
            us, 1000000 / us, frame_pixels / us);

    dst.left = 0;
    dst.top = 0;
    dst.right = test_width;
    dst.bottom = test_height;

    // TODO: add correctness check for below: 90 rotation with resize

    // reformat and resize source with 90 rotation
    src.left = 0;
    src.top = 0;
    src.right = (test_width > 1280) ? 1280 : test_width >> 1;
    src.bottom = (test_height > 720) ? 720 : test_height >> 1;
    src.stride = (test_width > 1280) ? 1280 : test_width >> 1;
    src.width = (test_width > 1280) ? 1280 : test_width >> 1;
    src.height = (test_height > 720) ? 720 : test_height >> 1;

    dst.rot = G2D_ROTATION_90;
    dst.left = 0;
    dst.top = 0;
    dst.right =  test_width;
    dst.bottom = test_height;
    dst.stride = test_width;
    dst.width =  test_width;
    dst.height = test_height;

    g2d_printf("g2d 90 rotation with resize from %dx%d to %dx%d: \n", src.width, src.height,
            dst.width, dst.height);

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf(" -> bgra8888 to rgba8888, time %dus, %dfps, %dMpixel/s ........\n",
            us, 1000000 / us, frame_pixels / us);

    // reformat and resize destination with 90 rotation
    src.right =  test_width;
    src.bottom = test_height;
    src.stride = test_width;
    src.width =  test_width;
    src.height = test_height;
    src.rot = G2D_ROTATION_0;
    src.format = G2D_BGRA8888;

    dst.rot = G2D_ROTATION_90;
    dst.right = (test_width > 1280) ? 1280 : test_width >> 1;
    dst.bottom = (test_height > 720) ? 720 : test_height >> 1;
    dst.stride = (test_width > 1280) ? 1280 : test_width >> 1;
    dst.width = (test_width > 1280) ? 1280 : test_width >> 1;
    dst.height = (test_height > 720) ? 720 : test_height >> 1;
    dst.format = G2D_RGBA8888;

    g2d_printf("g2d 90 rotation with resize test from %dx%d to %dx%d: \n", src.width, src.height,
            dst.width, dst.height);

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_blit(handle, &src, &dst);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf(" -> bgra8888 to rgba8888, time %dus, %dfps, %dMpixel/s ........\n",
            us, 1000000 / us, frame_pixels / us);

    dst.left   = src.left = 0;
    dst.top    = src.top  = 0;
    dst.right  = src.right  = test_width;
    dst.bottom = src.bottom = test_height;
    dst.stride = src.stride = test_width;
    dst.width  = src.width  = test_width;
    dst.height = src.height = test_height;
    dst.rot    = src.rot    = G2D_ROTATION_0;
    dst.format = src.format = G2D_RGBA8888;

    /****************************************** test g2d_copy
     * *********************************************************/
    memset(s_buf->buf_vaddr, 0xab, frame_bytes);
    memset(d_buf->buf_vaddr, 0xcd, frame_bytes);

    g2d_copy(handle, d_buf, s_buf, frame_bytes);
    g2d_finish(handle);

    if (memcmp(s_buf->buf_vaddr, d_buf->buf_vaddr, frame_bytes)) {
        g2d_printf("g2d_copy: dst buffer is not copied from src buffer correctly !\n");
    }

    g2d_printf("---------------- g2d copy & cache performance ----------------\n");
    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        g2d_copy(handle, d_buf, s_buf, frame_bytes);
    }
    g2d_finish(handle);

    us = get_test_runtime(start, test_loops);
    g2d_printf("g2d copy non-cacheable time %dus, %dfps, %dMpixel/s ........\n", us,
            1000000 / us, frame_pixels / us);

    start = hal_get_current_time();

    for (i = 0; i < test_loops; i++) {
        memcpy(d_buf->buf_vaddr, s_buf->buf_vaddr, frame_bytes);
    }
        
    g2d_free(s_buf);
    g2d_free(d_buf);

    g2d_close(handle);

    g2d_printf("G2D basic test done\n");
    return 0;
}