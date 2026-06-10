/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _DPU_EXAMPLE_H_
#define _DPU_EXAMPLE_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DPU_DI_MIPI                  0
#define DPU_DI_LVDS                  1
#define DEMO_PANEL_LCD_SPEC          11
#define DEMO_PANEL_LVDS2HDMI         15
#define DEMO_PANEL_WAVESHARE_DSI2DPI 20

/* Use the LVDS interface. */
#ifndef DPU_EXAMPLE_DI
#define DPU_EXAMPLE_DI DPU_DI_MIPI
#endif

#ifndef DEMO_PANEL
#define DEMO_PANEL  DEMO_PANEL_WAVESHARE_DSI2DPI
#endif

#ifndef LDB_DUAL_PANEL
#define LDB_DUAL_PANEL  0
#endif

/* Whether use external convertor such as LVDS2HDMI convertor (IT6263) */
#ifndef APP_DISPLAY_EXTERNAL_CONVERTOR
#define APP_DISPLAY_EXTERNAL_CONVERTOR 0
#endif

#if (DPU_EXAMPLE_DI == DPU_DI_LVDS)
#define APP_DPU_DISPLAY_INDEX 0

#elif DPU_EXAMPLE_DI == DPU_DI_MIPI
#define APP_DPU_DISPLAY_INDEX 0
#endif

/* APP use MIPI DSI interface base. */
#define DSI_MAIN        DISPLAY__MIPI_DSI_HOST__DSI_V2_HOST_MAIN
#define DSI_PHY         DISPLAY__MIPI_DSI_HOST__DSI_V2_HOST_PHY
#define DSI_HOST        DISPLAY__MIPI_DSI_HOST__DSI_V2_HOST_DSI
#define DSI_IPI         DISPLAY__MIPI_DSI_HOST__DSI_V2_HOST_IPI
#define DSI_INT         DISPLAY__MIPI_DSI_HOST__DSI_V2_HOST_INT
#define DSI_CSR         DISPLAY__DSI_CSR
/* Configuration for DPU. */
#define APP_DPU_BASE DISPLAY_SEERIS_BASE
#define APP_DPU_LD_BASE DISPLAY_SEERIS_LD_BASE
#define APP_DPU      ((DISPLAY_SEERIS_Type *)APP_DPU_BASE)
#define APP_DPU_LD   ((DISPLAY_SEERIS_LD_Type *)APP_DPU_LD_BASE)
#define APP_DPU_BLIT ((DISPLAY_SEERIS_Type *)0x4B430000U)
#define APP_DPU_DPHY ((DISPLAY_MIPI_DSI_PHY_Type *)0x4B110000U)

/* Display stream 0 base address setting */
/*InterruptEnable0
Interrupt Enable register 0.
0x3f1000 + 0x8 (Display IRQ3)
0x3e1000 + 0x8 (Display IRQ2)
0x3d1000 + 0x8 (Display IRQ1)
0x3c1000 + 0x8 (Display IRQ0)
0x31000 + 0x8 (Blit IRQ)
0x1000 + 0x8 (IRQ)
*/
#if (0 == APP_DPU_DISPLAY_INDEX)
#define APP_DPU_DC   ((DISPLAY_SEERIS_Type *)0x4B7C0000U)
/* Display stream 1 base address setting */
#else
#define APP_DPU_DC   ((DISPLAY_SEERIS_Type *)0x4B7E0000U)
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
#if (DEMO_PANEL == DEMO_PANEL_WAVESHARE_DSI2DPI)
/*
 * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
 * (600 + 10 + 10 + 10) x (1024 + 100 + 100 + 100) x 60 = 630 x 1324 x 60Hz = 50047200 Hz
 */
/* 1280*800 LVDS pannel */
#define APP_PANEL_HEIGHT   600
#define APP_PANEL_WIDTH    1024
#define APP_HSW            100
#define APP_HFP            100
#define APP_HBP            100
#define APP_VSW            10
#define APP_VFP            10
#define APP_VBP            10
#define APP_PIXEL_CLOCK_HZ 50000000
#endif

#if (DEMO_PANEL == DEMO_PANEL_LCD_SPEC)
/*
 * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
 * (800 + 6 + 2 + 15) * (1280 + 32 + 48 + 80) * 60Hz = 823 * 1440 * 60Hz = 1,185,120 * 60 = 71,107,200 Hz
 */
#define APP_PANEL_HEIGHT   800
#define APP_PANEL_WIDTH    1280
#define APP_HSW            32
#define APP_HFP            48
#define APP_HBP            80
#define APP_VSW            6
#define APP_VFP            2
#define APP_VBP            15
/* 497.7 / 7 = 71.1 MHz */
#endif
#else

#if (DEMO_PANEL == DEMO_PANEL_LVDS2HDMI)
/*
 * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
 * (1080 + 4 + 36 + 5) * (1920 + 88 + 148 + 44) * 60 = 1125 * 2200 * 60Hz = 148500000 Hz
 */
/* 1920*1080 IT6263 LVDS2HDMI card */
#define APP_PANEL_HEIGHT   1080
#define APP_PANEL_WIDTH    1920
#define APP_HFP            88
#define APP_HBP            148
#define APP_HSW            44
#define APP_VFP            4
#define APP_VBP            36
#define APP_VSW            5
#endif
#endif

/* Configuration for display interface. */
#if DPU_EXAMPLE_DI == DPU_DI_MIPI
#define APP_MIPI_DSI_BASE MIPI_DSI_BASE
#define APP_MIPI_DSI      ((MIPI_DSI_Type *)APP_MIPI_DSI_BASE)
/* The MIPI DSI support 2 line, value in reg is 0x1. */
#define APP_MIPI_DSI_LANE_NUM                   2
#define APP_IPI_PIXEL_DEPTH                     960
#define MIPI_DSI_BPP                            24
#define MIPI_DSI_DIV                            32
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
#define APP_LD_INT_GROUP0             0
#define APP_LD_INT_GROUP1             kDPU_Group1LocalDimming0Irq1Interrupt
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

