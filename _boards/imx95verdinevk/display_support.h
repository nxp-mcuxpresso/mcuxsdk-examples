/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _DPU_EXAMPLE_H_
#define _DPU_EXAMPLE_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DPU_DI_LVDS     0
#define DPU_DI_MIPI     1
#define LVDS_DUAL_PANEL 2 
#define CAP_TOUCH_LVDS  3
#define CAP_TOUCH_DSI   4
#define MX8_DSI_OLED2   5
#define MIPI2HDMI       6

/* Use the MIPI interface. */
#ifndef DPU_EXAMPLE_DI
#define DPU_EXAMPLE_DI DPU_DI_MIPI
#endif

#ifndef DEMO_PANEL
#define DEMO_PANEL  MX8_DSI_OLED2
#endif

#ifndef LDB_DUAL_PANEL
#define LDB_DUAL_PANEL  0
#endif

/* Whether use external convertor such as MIPI2HDMI convertor (ADV7535) */
#ifndef APP_DISPLAY_EXTERNAL_CONVERTOR
#define APP_DISPLAY_EXTERNAL_CONVERTOR 0
#endif

#if (DPU_EXAMPLE_DI == DPU_DI_LVDS)
#define APP_DPU_DISPLAY_INDEX 1

#elif DPU_EXAMPLE_DI == DPU_DI_MIPI
#define APP_DPU_DISPLAY_INDEX 0
#endif

/* Configuration for DPU. */
#define APP_DPU_BASE DISPLAY_SEERIS_BASE
#define APP_DPU      ((DISPLAY_SEERIS_Type *)APP_DPU_BASE)
#define APP_DPU_BLIT ((DISPLAY_SEERIS_Type *)0x4B430000U)
/* Display stream 0 base address setting */
#if (0 == APP_DPU_DISPLAY_INDEX)
#define APP_DPU_DC   ((DISPLAY_SEERIS_Type *)0x4B780000U)
/* Display stream 1 base address setting */
#else
#define APP_DPU_DC   ((DISPLAY_SEERIS_Type *)0x4B7A0000U)
#endif

#define APP_DPU_TIMING_FLAGS \
    (kDPU_DisplayHsyncActiveLow | kDPU_DisplayVsyncActiveLow | kDPU_DisplayDataEnableActiveHigh)

#define DSICSI_EN_PWDN  (4U)
#define DSICSI_RST_SYNC (8U)
#define LVDS1_RST       (1U)
#define LVDS1_EN        (2U)
#define LVDS0_RST       (9U)
#define LVDS0_EN        (10U)
#define LVDS_PWM        (3U)


#if !APP_DISPLAY_EXTERNAL_CONVERTOR
#if (DEMO_PANEL == MX8_DSI_OLED2)
/*
 * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
 * (2340 + 4 + 10 + 10) * (1080 + 4 + 12 + 8) * x 60Hz = 2364 * 1104 * x 60Hz = 2,609,856 x 57 = 148,761,792 Hz
 */
#define APP_PANEL_HEIGHT 2340
#define APP_PANEL_WIDTH  1080
#define APP_HSW          4
#define APP_HFP          12
#define APP_HBP          8
#define APP_VSW          4
#define APP_VFP          10
#define APP_VBP          10
#endif

#if (DEMO_PANEL == CAP_TOUCH_DSI)
/*
 * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
 * (800 + 8 + 8 + 24) * (1280 + 24 + 16 + 64) * x 60 = 840 * 1384 * x 60Hz = 69,753,600 Hz
 */
/* 1280*800 DSI pannel */
#if 1
#define APP_PANEL_HEIGHT   800
#define APP_PANEL_WIDTH    1280
#define APP_HSW            24
#define APP_HFP            16
#define APP_HBP            64
#define APP_VSW            8
#define APP_VFP            8
#define APP_VBP            24
#endif
#endif


#if (DEMO_PANEL == CAP_TOUCH_LVDS)
/*
 * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
 * (800 + 6 + 2 + 15) * (1280 + 32 + 48 + 80) * x 60 = 823 * 1440 * x 60Hz = 71107200 Hz
 */
/* 1280*800 LVDS pannel */
#define APP_PANEL_HEIGHT   800
#define APP_PANEL_WIDTH    1280
#define APP_HSW            32
#define APP_HFP            48
#define APP_HBP            80
#define APP_VSW            6
#define APP_VFP            2
#define APP_VBP            15
#endif

#else

#if (DEMO_PANEL == MIPI2HDMI)
/*
 * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
 * (1080 + 4 + 36 + 5) * (1920 + 88 + 148 + 44) * x 60 = 1125 * 2200 * x 60Hz = 148500000 Hz
 */
/* 1920*1080 ADV7535 MIPI2HDMI card */
#define APP_PANEL_HEIGHT   1080
#define APP_PANEL_WIDTH    1920
#define APP_HFP            88
#define APP_HBP            148
#define APP_HSW            44
#define APP_VFP            4
#define APP_VBP            36
#define APP_VSW            5

#define APP_PIXEL_CLOCK_HZ 148444444
#endif

#endif

/* Configuration for display interface. */
#if DPU_EXAMPLE_DI == DPU_DI_MIPI
#define APP_MIPI_DSI_BASE MIPI_DSI_BASE
#define APP_MIPI_DSI      ((MIPI_DSI_Type *)APP_MIPI_DSI_BASE)
/* The MIPI DSI support 4 line. */
#define APP_MIPI_DSI_LANE_NUM 4
#define MIPI_DSI_BPP          24
#define MIPI_DSI_DIV          32
#elif DPU_EXAMPLE_DI == DPU_DI_LVDS
#define APP_LDB_BASE LVDS_BASE
#define APP_LDB      ((LDB_Type *)APP_LDB_BASE)
#define APP_LDB_CH   0
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if (0 == APP_DPU_DISPLAY_INDEX)
#define APP_CONTENT_STREAM_PIPELINE   kDPU_PipelineExtDst0
#define APP_CONTENT_STREAM_EXTDST     kDPU_ExtDst0
#define APP_CONTENT_STREAM_INT_GROUP0 kDPU_Group0ExtDst0ShadowLoadInterrupt
#define APP_CONTENT_STREAM_INT_GROUP1 0U
#define APP_SAFETY_STREAM_PIPELINE    kDPU_PipelineExtDst4
#define APP_SAFETY_STREAM_EXTDST      kDPU_ExtDst4
#define APP_SAFETY_STREAM_INT_GROUP0  kDPU_Group0ExtDst4ShadowLoadInterrupt
#define APP_SAFETY_STREAM_INT_GROUP1  0U
#define APP_SIG_SHD_LD_INT_GROUP0     kDPU_Group0Sig0ShadowLoadInterrupt
#define APP_SIG_SHD_LD_INT_GROUP1     0U
#define APP_DISPLAY_STREAM_INT_GROUP0 kDPU_Group0Display0ShadowLoadInterrupt
#define APP_DISPLAY_STREAM_INT_GROUP1 0U
#define APP_CONTENT_DOMAINBLEND       kDPU_DomainBlend0
#else
#define APP_CONTENT_STREAM_PIPELINE   kDPU_PipelineExtDst1
#define APP_CONTENT_STREAM_EXTDST     kDPU_ExtDst1
#define APP_CONTENT_STREAM_INT_GROUP0 kDPU_Group0ExtDst1ShadowLoadInterrupt
#define APP_CONTENT_STREAM_INT_GROUP1 0U
#define APP_SAFETY_STREAM_PIPELINE    kDPU_PipelineExtDst5
#define APP_SAFETY_STREAM_EXTDST      kDPU_ExtDst5
#define APP_SAFETY_STREAM_INT_GROUP0  kDPU_Group0ExtDst5ShadowLoadInterrupt
#define APP_SAFETY_STREAM_INT_GROUP1  0U
#define APP_SIG_SHD_LD_INT_GROUP0     0U
#define APP_SIG_SHD_LD_INT_GROUP1     kDPU_Group1Sig1ShadowLoadInterrupt
#define APP_DISPLAY_STREAM_INT_GROUP0 kDPU_Group0Display1ShadowLoadInterrupt
#define APP_DISPLAY_STREAM_INT_GROUP1 0U
#define APP_CONTENT_DOMAINBLEND       kDPU_DomainBlend1
#endif

#define APP_BLIT_SHD_LD_INT_GROUP0 kDPU_Group0Store9ShadowLoadInterrupt
#define APP_BLIT_SHD_LD_INT_GROUP1 0U
#define APP_BLIT_SEQ_INT_GROUP0    kDPU_Group0Store9SeqCompleteInterrupt
#define APP_BLIT_SEQ_INT_GROUP1    0U
#define APP_BLIT_COM_INT_GROUP0    kDPU_Group0Store9FrameCompleteInterrupt
#define APP_BLIT_COM_INT_GROUP1    0U
#define APP_BLIT_COM_INT_GROUP2    0U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Prepare the display, such as enable display module clock and interrupts. */
extern void BOARD_PrepareDisplay(void);
extern void APP_InitPixelLink(void);
void BOARD_InitDisplayInterface(void);

#endif /* _DPU_EXAMPLE_H_ */