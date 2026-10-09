/**
 * @file lv_conf.h
 */

#ifndef LV_CONF_H
#define LV_CONF_H

/* clang-format off */

/*
 * COLOR FORMAT
 */

#if defined(DEMO_BUFFER_BYTE_PER_PIXEL)

#if DEMO_BUFFER_BYTE_PER_PIXEL == 2
#define LV_COLOR_FORMAT_DEFAULT      LV_COLOR_FORMAT_RGB565
#elif DEMO_BUFFER_BYTE_PER_PIXEL == 3
#define LV_COLOR_FORMAT_DEFAULT      LV_COLOR_FORMAT_RGB888
#elif DEMO_BUFFER_BYTE_PER_PIXEL == 4
#define LV_COLOR_FORMAT_DEFAULT      LV_COLOR_FORMAT_XRGB8888
#elif DEMO_BUFFER_BYTE_PER_PIXEL == 1
#define LV_COLOR_FORMAT_DEFAULT      LV_COLOR_FORMAT_L8
#else
#error Unsupported color depth
#endif

#endif /* defined(DEMO_BUFFER_BYTE_PER_PIXEL) */

/*
 * BUFFER ALIGNMENT
 */

#if defined(LVGL_FB_ALIGN)

/*Align the stride of all layers and images to this bytes*/
#define LV_DRAW_BUF_STRIDE_ALIGN                LVGL_FB_ALIGN

/*Align the start address of draw_buf addresses to this bytes*/
#define LV_DRAW_BUF_ALIGN                       LVGL_FB_ALIGN

#endif

/*
 * ATTRIBUTE OVERRIDES
 * (Cannot be set through Kconfig in v9.6, must be defined here)
 *
 * LVGL v9.6 removed LV_ATTRIBUTE_* from Kconfig because they are C expressions.
 * The Kconfig options LV_ATTRIBUTE_LARGE_RAM_ARRAY_SECTION and
 * LV_ATTRIBUTE_LARGE_CONST_SECTION carry only the linker section *name*;
 * the stringify helper below turns it into __attribute__((section("..."))).
 */

#define _LV_STRINGIFY(s) #s
#define _LV_SECTION_ATTR(s) __attribute__((section(_LV_STRINGIFY(s))))

#if CONFIG_LV_ATTRIBUTE_MEM_ALIGN_SIZE
#define LV_ATTRIBUTE_MEM_ALIGN __attribute__((aligned(CONFIG_LV_ATTRIBUTE_MEM_ALIGN_SIZE)))
#else
#define LV_ATTRIBUTE_MEM_ALIGN __attribute__((aligned(4)))
#endif

#ifdef CONFIG_LV_ATTRIBUTE_LARGE_RAM_ARRAY_SECTION
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY _LV_SECTION_ATTR(CONFIG_LV_ATTRIBUTE_LARGE_RAM_ARRAY_SECTION)
#endif

#ifdef CONFIG_LV_ATTRIBUTE_LARGE_CONST_SECTION
#define LV_ATTRIBUTE_LARGE_CONST _LV_SECTION_ATTR(CONFIG_LV_ATTRIBUTE_LARGE_CONST_SECTION)
#endif

/*
 * ASSERT HANDLER OVERRIDE
 */

#include <assert.h>
#define LV_ASSERT_HANDLER assert(0);

/* clang-format on */
#endif /*LV_CONF_H*/
