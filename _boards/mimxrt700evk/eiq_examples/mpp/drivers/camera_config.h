/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef CAMERA_CONFIG_H_
#define CAMERA_CONFIG_H_

/* PSRAM addresses for camera buffer 640x480x4 = 0x12C000 */
#define CAM_PSRAM 0
#define CAMERA_BUFFER_ADDR0 (0x60400000U)
#define CAMERA_BUFFER_ADDR1 (0x60600000U)

#define DEMO_FLEXIO_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()
#define FLEXIO_MAX_FREQUENCY (DEMO_FLEXIO_CLOCK_FREQUENCY / 2U)
#define FLEXIO_MIN_FREQUENCY (DEMO_FLEXIO_CLOCK_FREQUENCY / 512U)
/* FLEXIO output PWM frequency */
#define DEMO_FLEXIO_PWM_FREQ_HZ                (12000000U)
#define DEMO_FLEXIO_PWM_TO_XCLK                (1U) /* Select FXIO2_D1 as XCLK output */
#define DEMO_FLEXIO_TIMER_XCLK                 (1U)  /* Flexio timer1 used For PWM */

/* FLEXIO time0 for shifter to receive data */
#define DEMO_FLEXIO_TIMER_PCLK                 (0U)
#define DEMO_FLEXIO_DATA_START_IDX             (6U)
#define DEMO_FELXIO_HREF_IDX                   (2U)
#define DEMO_FELXIO_PCLK_IDX                   (15U)
#define DEMO_FLEXIO_CAMERA_DATA_WIDTH          (8U)
#define DEMO_FLEXIO_SHIFTER_START_IDX          (0U)
#define DEMO_FLEXIO_SHIFTER_NUM                (8U)

/* FlexCommon8 I2C */
#define I2C_BAUDRATE               100000U
#define SCCB_CLOCK_FREQ CLOCK_GetLPFlexCommClkFreq(8u)
#define SCCB_BASE ((LPI2C_Type *)LPI2C8_BASE)

/*!< @brief OV7670 I2C address. */
#define DEMO_XSYNC_IRQ        GPIO20_IRQn
#define DEMO_XSYNC_PORT       (GPIO2)
#define DEMO_XSYNC_PIN        (5U)
#define DEMO_HREF_PORT        (GPIO2)
#define DEMO_HREF_PIN         (2U)
#define DEMO_PCLK_PORT        (GPIO2)
#define DEMO_PCLK_PIN         (15U)
#define DEMO_RESET_PORT       (GPIO2)
#define DEMO_RESET_PIN        (0U)

/*================= Camera ==================== */
#define CAMERA_BPP            (2U)
#define CAMERA_FRAME_RATE     (30U)
#define CAMERA_FRAMEBUF_NUM   (2U)
#define CAMERA_WIDTH          (640U)
#define CAMERA_HEIGHT         (480U)
#define CAMERA_LINE_DATA_SIZE    (CAMERA_WIDTH *CAMERA_BPP)
#define CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)

/*================ Buffer ====================== */
#define BUFF_WIDTH         (CAMERA_WIDTH)
#define BUFF_HEIGHT        (CAMERA_HEIGHT)

#define QUEUE_SIZE    (2U)
typedef volatile struct tag_CameraData
{
//    uint32_t valid;
//    uint32_t lock;
    uint32_t len; /* pixel bytes */
    void *pBuf;
} CameraBuffer_t;

typedef volatile struct tag_FlexioCameraTransfer
{
    CameraBuffer_t queue[QUEUE_SIZE];
    uint8_t driverIdx;
    uint8_t userIdx;
//    uint8_t flag;
} FlexioDvpTransfer;

extern FlexioDvpTransfer g_cameraQueue;
#endif
