/*
 * Copyright 2022-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>

#include "FreeRTOS.h"
#include "task.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "vg_lite.h"
#include "vglite_support.h"

#define TESSELLATION_WINDOW_X 128    /* pixels */
#define TESSELLATION_WINDOW_Y 128    /* pixels */

#define COMMAND_BUFFER_SIZE 0x10000  /* 64KB */
#define VGLITE_HEAP_SIZE    0x300000 /* 3MB */

#define PI 3.14159

#define PSRAM_BASE_ADDR 0x60000000
#define PSRAM_END_ADDR  0x64000000

#ifndef CUSTOM_VGLITE_MEMORY_CONFIG
#error "This application requires the CUSTOM_VGLITE_MEMORY_CONFIG=1 flag!"
#endif

/* VGLite driver heap */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t contiguous_mem[VGLITE_HEAP_SIZE], 64);

void *vglite_heap_base    = contiguous_mem;
uint32_t vglite_heap_size = VGLITE_HEAP_SIZE;

int16_t star_data[] = {VLC_OP_MOVE, 0,    -160, VLC_OP_LINE, 47,  -65, VLC_OP_LINE, 152, -49, VLC_OP_LINE, 76,  25,
                       VLC_OP_LINE, 94,   129,  VLC_OP_LINE, 0,   80,  VLC_OP_LINE, -94, 129, VLC_OP_LINE, -76, 25,
                       VLC_OP_LINE, -152, -49,  VLC_OP_LINE, -47, -65, VLC_OP_END};

/* Define a render target in pSRAM: */
vg_lite_buffer_t rt = {.width             = 640,
                       .height            = 640,
                       .format            = VG_LITE_BGR565,
                       .tiled             = VG_LITE_LINEAR,
                       .image_mode        = VG_LITE_NORMAL_IMAGE_MODE,
                       .transparency_mode = VG_LITE_IMAGE_OPAQUE,
                       .handle            = NULL,
                       .memory            = (void *)PSRAM_BASE_ADDR,
                       .address           = PSRAM_BASE_ADDR};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vglite_task(void *pvParameters);

void vg_lite_bus_error_handler(void);

static status_t init_graphic_resources(vg_lite_buffer_t *arc, vg_lite_path_t *star);

static status_t render_frame(vg_lite_buffer_t *target, vg_lite_buffer_t *arc, vg_lite_path_t *star, unsigned frameId);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();
    BOARD_PrepareVGLiteController();

    PRINTF("\r\n");
    PRINTF("Starting up VGLite graphics demo...\r\n");
    /* Create application task */
    if (xTaskCreate(vglite_task, "vglite_task", configMINIMAL_STACK_SIZE + 200, NULL, configMAX_PRIORITIES - 1, NULL) !=
        pdPASS)
    {
        PRINTF("ERROR: Failed to create main application task!\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Main graphic demo task.
 */
static void vglite_task(void *pvParameters)
{
    vg_lite_error_t vg_err;
    vg_lite_buffer_t arc;
    vg_lite_path_t star;
    unsigned i                = 0;
    unsigned long frame_count = 0;

    PRINTF("Initializing GPU driver...\r\n");
    vg_lite_set_command_buffer_size(COMMAND_BUFFER_SIZE);

    vg_err = vg_lite_init(TESSELLATION_WINDOW_X, TESSELLATION_WINDOW_Y);
    if (vg_err != VG_LITE_SUCCESS)
    {
        PRINTF("\r\nERROR: Failed to initialize VGLite driver (err = %d)!\r\n", vg_err);
        vTaskDelete(NULL);
    }

    PRINTF("Initializing resources...\r\n");
    /* Allocate and prepare the yellow arc image */
    memset(&arc, 0, sizeof(vg_lite_buffer_t));
    arc.width             = 640;
    arc.height            = 640;
    arc.format            = VG_LITE_ARGB1555;
    arc.tiled             = VG_LITE_LINEAR;
    arc.image_mode        = VG_LITE_NORMAL_IMAGE_MODE;
    arc.transparency_mode = VG_LITE_IMAGE_OPAQUE;

    vg_err = vg_lite_allocate(&arc);
    if (vg_err != VG_LITE_SUCCESS)
    {
        PRINTF("\r\nERROR: Failed to allocate graphic resources (err = %d)!\r\n", vg_err);
        vTaskDelete(NULL);
    }

    /* Allocate render target */
    rt.memory  = NULL;
    rt.address = 0;
    vg_err     = vg_lite_allocate(&rt);
    if (vg_err != VG_LITE_SUCCESS)
    {
        PRINTF("\r\nERROR: Failed to allocate render target (err = %d)!\r\n", vg_err);
        vTaskDelete(NULL);
    }

    if (init_graphic_resources(&arc, &star) != kStatus_Success)
    {
        PRINTF("\r\nERROR: Failed to initialize graphic resources!\r\n");
        vTaskDelete(NULL);
    }

    PRINTF("Running demo...\r\n");
    while (1)
    {
        if (render_frame(&rt, &arc, &star, i) != kStatus_Success)
        {
            PRINTF("\r\nERROR: Failed to render frame!\r\n");
            break;
        }

        /* Reset frame index from time to time */
        if (i >= 72)
        {
            frame_count += i;
            PRINTF("Generated frames: %lu...\r\n", frame_count);
            i = 1;
        }
        else
        {
            i++;
        }
    }

    PRINTF("Releasing resources and shutting down...\r\n");

    /* Release graphics resources */
    vg_lite_free(&arc);

    vg_lite_close();

    PRINTF("Stop.\r\n");
    vTaskDelete(NULL);
}

/*!
 * @brief Initializes graphic resources for the demo.
 */
static status_t init_graphic_resources(vg_lite_buffer_t *arc, vg_lite_path_t *star)
{
    vg_lite_error_t vg_err;
    vg_lite_path_t arc_path;
    int16_t arc_data[] = {VLC_OP_MOVE, 320, 30,  VLC_OP_CUBIC, 160, 30,           30,  160, 30,  320, VLC_OP_CUBIC, 30,
                          480,         160, 610, 320,          610, VLC_OP_CUBIC, 480, 610, 610, 480, 610,          320,
                          VLC_OP_LINE, 550, 320, VLC_OP_CUBIC, 550, 447,          447, 550, 320, 550, VLC_OP_CUBIC, 193,
                          550,         90,  447, 90,           320, VLC_OP_CUBIC, 90,  193, 193, 90,  320,          90,
                          VLC_OP_END};
    vg_lite_matrix_t m;

    vg_err = vg_lite_init_path(&arc_path, VG_LITE_S16, VG_LITE_MEDIUM, sizeof(arc_data), arc_data, 30, 30, 610, 610);
    if (vg_err != VG_LITE_SUCCESS)
    {
        PRINTF("\r\nERROR: vg_lite_init_path() #1 failed (err = %d)!\r\n", vg_err);
        return kStatus_Fail;
    }

    vg_err = vg_lite_init_path(star, VG_LITE_S16, VG_LITE_MEDIUM, sizeof(star_data), star_data, -160, -160, 160, 160);
    if (vg_err != VG_LITE_SUCCESS)
    {
        PRINTF("\r\nERROR: vg_lite_init_path() #2 failed (err = %d)!\r\n", vg_err);
        return kStatus_Fail;
    }

    /* Use transparent black as background */
    vg_err = vg_lite_clear(arc, NULL, 0);
    if (vg_err != VG_LITE_SUCCESS)
    {
        PRINTF("\r\nERROR: vg_lite_clear() failed (err = %d)!\r\n", vg_err);
        return kStatus_Fail;
    }

    vg_lite_identity(&m);
    vg_err = vg_lite_draw(arc, &arc_path, VG_LITE_FILL_EVEN_ODD, &m, VG_LITE_BLEND_SRC_OVER, 0xff00ffff);
    if (vg_err != VG_LITE_SUCCESS)
    {
        PRINTF("\r\nERROR: vg_lite_draw() failed (err = %d)!\r\n", vg_err);
        return kStatus_Fail;
    }

    vg_lite_finish();

    return kStatus_Success;
}

/*!
 * @brief Renders one frame using the provided resources.
 */
static status_t render_frame(vg_lite_buffer_t *target, vg_lite_buffer_t *arc, vg_lite_path_t *star, unsigned frameId)
{
    vg_lite_error_t vg_err;
    vg_lite_matrix_t m;
    vg_lite_float_t arc_angle, star_angle, arc_angle_rad;
    vg_lite_float_t x, y;

    star_angle    = (frameId * 5) % 360;
    arc_angle     = 360 - star_angle;
    arc_angle_rad = ((arc_angle - 45.0) * PI) / 180.0;
    x             = 320.0 + 453.0 * sinf(arc_angle_rad);
    y             = 320.0 - 453.0 * cosf(arc_angle_rad);

    /* Use an opaque blue background */
    vg_err = vg_lite_clear(target, NULL, 0xffe87460);
    if (vg_err != VG_LITE_SUCCESS)
    {
        PRINTF("\r\nERROR: vg_lite_clear() failed (err = %d)!\r\n", vg_err);
        return kStatus_Fail;
    }

    vg_lite_identity(&m);
    vg_lite_translate(x, y, &m);
    vg_lite_rotate(arc_angle, &m);
    vg_err = vg_lite_blit(target, arc, &m, VG_LITE_BLEND_SRC_OVER, 0, VG_LITE_FILTER_POINT);
    if (vg_err != VG_LITE_SUCCESS)
    {
        PRINTF("\r\nERROR: vg_lite_blit() failed (err = %d)!\r\n", vg_err);
        return kStatus_Fail;
    }

    vg_lite_identity(&m);
    vg_lite_translate(320, 320, &m);
    vg_lite_rotate(star_angle, &m);

    vg_err = vg_lite_draw(target, star, VG_LITE_FILL_EVEN_ODD, &m, VG_LITE_BLEND_SRC_OVER, 0xfff0f000);
    if (vg_err != VG_LITE_SUCCESS)
    {
        PRINTF("\r\nERROR: vg_lite_draw() failed (err = %d)!\r\n", vg_err);
        return kStatus_Fail;
    }

    vg_lite_finish();

    return kStatus_Success;
}

/*!
 * @brief Signals a GPU bus error.
 */
void vg_lite_bus_error_handler(void)
{
    PRINTF("ERROR: GPU bus error!\r\n");
}
