/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* @brief This example application shows usage of MultiMedia Pipeline to recognize a face:
 * USB/Virtual Camera (IR:VGA & RGB:QVGA) -> split -> JPEG HW -> image converter -> draw labeled rectangles -> display
 *                   +-> JPEG HW -> image converter -> inference engine (model: scrfd_kps/antispoofing/mobilefacenet)
 * The model performs face detection then recognition using TF-Lite micro inference engine
 * the model output is displayed on UART console by application */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "atomic.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "math.h"

/* NXP includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

/* MPP includes */
#include "mpp_api.h"
#include "mpp_config.h"
#include "hal_utils.h"
#include "hal_os.h"
#include "hal_debug.h"
#include "common/draw_text.h"

/* utility functions */
#include "utils.h"
#include "face_box_utils.h"
#include "database_utils.h"

#include "app_constants.h"
#include "app_types.h"
#include "app_msg.h"

#include "mpp_app_task_notify.h"

/* Tensorflow lite model data input */
#include APP_TFLITE_MOBILEFACENET_DATA
#include APP_TFLITE_SCRFD_KPS_DATA
#include APP_TFLITE_ANTISPOOFING_DATA

/* Persons database */
#include APP_DATABASE_NAME

#ifndef APP_SKIP_CONVERT_FOR_DISPLAY
#define APP_SKIP_CONVERT_FOR_DISPLAY 0
#endif

#include APP_TFLITE_MOBILEFACENET_INFO
#include APP_TFLITE_SCRFD_KPS_INFO
#include APP_TFLITE_ANTISPOOFING_INFO

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* image buffer for the text area */
uint8_t g_text_img[TEXT_WIDTH*TEXT_HEIGHT*TEXT_BPP] __attribute__((section(".noinit.$cam_buff_sh_mem"), aligned(64)));
/* image buffer for the inference preview area */
#ifdef DEBUG_PREVIEW_RECOGNITION
__ALIGNED(64) uint8_t  g_inf_img[INFPVW_WIDTH * INFPVW_HEIGHT * INFPVW_BPP];
#endif

/* logo image array */
#include "images/NXP_Logo_RGB_Colour_320.h"

#ifndef APP_STRIPE_MODE
#define APP_STRIPE_MODE 0
#endif

/* pick default backend if not specified. */
#ifndef APP_GFX_BACKEND_NAME
#define APP_GFX_BACKEND_NAME NULL
#endif

static user_data_t user_data = {0};

typedef struct _args_t {
    char camera_name[32];
    char display_name[32];
    mpp_pixel_format_t src_format;
    mpp_pixel_format_t display_format;
} args_t;

/* info to be dislayed in text area */
typedef struct {
	char name[MAX_WORD_SIZE];
	float confidence;
	int detection_inference_time;
    int antispoofing_inference_time;
    int recognition_inference_time;
	char state[MAX_WORD_SIZE];
} text_info_t;

typedef struct {
    int name_start_x;
    int name_stop_x;
    int confidence_start_x;
    int confidence_stop_x;
    int detection_start_x;
    int detection_stop_x;
    int antispoofing_start_x;
    int antispoofing_stop_x;
    int recognition_start_x;
    int recognition_stop_x;
    int state_start_x;
    int state_stop_x;
} text_position_t;


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void app_task(void *params);
int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);
extern void ctrl_task_notify(uint8_t cmd, msg_t* para);
static void registration_timeout_callback(TimerHandle_t xTimer);
int user_data_access(user_data_t *user_data, uint32_t timeout);
void user_data_release(user_data_t *user_data);
void enable_camera_ir_stream(user_data_t *user_data);
void disable_camera_ir_stream(user_data_t *user_data);
void HAL_DCACHE_CleanByRange(uint32_t addr, uint32_t size);

static struct rpmsg_lite_instance *rpmsg_inst = NULL;
volatile uint16_t *mcmgr_event_data = NULL;
static QueueHandle_t s_mpp_task_queue = NULL;

text_position_t text_position = {0};
text_context_t  text_ctx = {0};

bool first_mpp_start = true;

/* registration timer vaiables */
static TickType_t registration_start_time = 0;
static TickType_t registration_end_time = 0;
static TimerHandle_t registration_timer = NULL;
static bool registration_timer_expired = false;
float face_reco_threshold = 0.0f;
/*******************************************************************************
 * Code
 ******************************************************************************/

 /*!
 * @brief drawing of text by cpu into a buffer.
 */
static void init_text_area(void* buf, uint32_t size, text_info_t* text)
{
    int space4items  = get_font_height() + 6;
    int start_y_middle = get_font_height()/2 + 2;
    int row_indentation = get_font_width()/2;
    int x_pos = 0;

    /*Clear the buffer. */
    memset(buf,0,size);
    init_text_buf(&text_ctx, buf, size, TEXT_WIDTH, TEXT_HEIGHT, TEXT_WIDTH);

    draw_text_line(&text_ctx, "User name:",row_indentation,start_y_middle);
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "   ", row_indentation, start_y_middle );
    text_position.name_start_x = x_pos;
    text_position.name_stop_x = x_pos;

    start_y_middle += space4items;
    draw_text_line(&text_ctx, "Confidence:", row_indentation, start_y_middle);
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "    ", row_indentation, start_y_middle );
    text_position.confidence_start_x = x_pos;
    text_position.confidence_stop_x = x_pos;

    start_y_middle += space4items;
    draw_text_line(&text_ctx, "Inf times:",row_indentation,start_y_middle);
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "    detect:", row_indentation, start_y_middle );
    text_position.detection_start_x = x_pos;
    text_position.detection_stop_x = x_pos;
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "    recog:", row_indentation, start_y_middle );
    text_position.recognition_start_x = x_pos;
    text_position.recognition_stop_x = x_pos;

#if USE_ANTISPOOFING
    start_y_middle += get_font_height();
    x_pos = draw_text_line(&text_ctx, "   spoof:", row_indentation, start_y_middle );
    text_position.antispoofing_start_x = x_pos;
    text_position.antispoofing_stop_x = x_pos;

    start_y_middle += space4items;
    text_position.state_start_x = row_indentation;
    text_position.state_stop_x = row_indentation;
#endif

	//make sure all data are pushed to the memory.
	HAL_DCACHE_CleanByRange((uint32_t)buf,size);
}

 /*!
 * @brief drawing of text by cpu into a buffer.
 */
static void draw_text_area(text_info_t* text)
{
    int space4items  = get_font_height() + 6;
    int start_y_middle = get_font_height()/2 + 2;
    char tmp[MAX_STRING_SIZE];
    int x_start, clear_width = 0;

    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%s",text->name);
    x_start = text_position.name_start_x;
    clear_width = text_position.name_stop_x - text_position.name_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.name_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );

    start_y_middle += space4items + get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%2d%%",(int)(text->confidence));
    x_start = text_position.confidence_start_x;
    clear_width = text_position.confidence_stop_x - text_position.confidence_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.confidence_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );

    start_y_middle += space4items + get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%dms",text->detection_inference_time);
    x_start = text_position.detection_start_x;
    clear_width = text_position.detection_stop_x - text_position.detection_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.detection_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );
    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%dms",text->recognition_inference_time);
    x_start = text_position.recognition_start_x;
    clear_width = text_position.recognition_stop_x - text_position.recognition_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.recognition_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );

#if USE_ANTISPOOFING
    start_y_middle += get_font_height();
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%dms",text->antispoofing_inference_time);
    x_start = text_position.antispoofing_start_x;
    clear_width = text_position.antispoofing_stop_x - text_position.antispoofing_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.antispoofing_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );

    start_y_middle += space4items;
    memset(tmp,0,sizeof(tmp));
    snprintf(tmp,sizeof(tmp),"%s",text->state);
    x_start = text_position.state_start_x;
    clear_width = text_position.state_stop_x - text_position.state_start_x;
    clear_text_line(&text_ctx, x_start, start_y_middle, clear_width);
    text_position.state_stop_x = draw_text_line(&text_ctx, tmp, x_start, start_y_middle );
#endif

	//make sure all data are pushed to the memory.
	HAL_DCACHE_CleanByRange((uint32_t) text_ctx.fb, text_ctx.size);
}

void mpp_app_start(mpp_app_params_t *params)
{
    BaseType_t ret = pdFAIL;
    TaskHandle_t handle = NULL;

    args_t *args = pvPortMalloc(sizeof(args_t));
    if (!args) {
        PRINTF("Allocation failed\n\rCannot start MPP task\r\n");
        return;
    }

    strcpy(args->display_name, APP_DISPLAY_NAME);
    strcpy(args->camera_name, APP_CAMERA_NAME);
    args->src_format = APP_CAMERA_FORMAT;
    args->display_format = APP_DISPLAY_FORMAT;

    mcmgr_event_data = params->mcmgr_event_data_p;
    rpmsg_inst = params->rpmsg_inst_p;
    face_reco_threshold = params->recognition_threshold;

    ret = xTaskCreate(
          app_task,
          "app_task",
          configMINIMAL_STACK_SIZE + 2400,
          (void *) args,
          APP_DEFAULT_PRIO,
          &handle);

    if (ret != pdTRUE)
    {
        PRINTF("Failed to create MPP task\r\n");
        vPortFree(args);
        return;
    }

    PRINTF("mpp start\r\n");
}

// Timer callback function
static void registration_timeout_callback(TimerHandle_t xTimer)
{
    registration_timer_expired = true;
}

int user_data_access(user_data_t *user_data, uint32_t timeout)
{
    TickType_t tick_timeout;

    if (timeout == 0)
    {
        if (Atomic_CompareAndSwap_u32(&user_data->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS)
            return 1;
        else
            return 0;
    }

    tick_timeout = timeout / portTICK_PERIOD_MS;
    TickType_t tick = xTaskGetTickCount();
    TickType_t crt_tick = 0;
    while (Atomic_CompareAndSwap_u32(&user_data->accessing, 1, 0) != ATOMIC_COMPARE_AND_SWAP_SUCCESS)
    {
        /* Busy wait */
        crt_tick =  xTaskGetTickCount();
        if ((crt_tick - tick) > tick_timeout)
        {
            PRINTF("ERR: Got timeout while waiting for user data access for more than %d ms\r\n", 
                    (crt_tick - tick) * portTICK_PERIOD_MS);
            return 0;
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }

    return 1;
}

void user_data_release(user_data_t *user_data)
{
    __atomic_store_n(&user_data->accessing, 0, __ATOMIC_SEQ_CST);
}

void print_conditional(user_data_t *user_data)
{
    if (user_data->last_model == MODEL_SCRFD_KPS)
    {
        if (user_data->detected_count > 0)
        {
            for (int i = 0; i < NUM_BOXES_MAX; i++)
            {
                if (user_data->final_boxes[i].area > 0)
                {
                    PRINTF("%s : box %d score %d(%%)\r\n", SCRFD_KPS_NAME, i,
                            (int)(user_data->final_boxes[i].score * 100.0f));
                    PRINTF("SCRFD inference time %u ms \r\n", user_data->inference_time_ms);
                }
            }
        }
    }
    else if(user_data->last_model == MODEL_MOBILEFACENET)
    {
        if (strcmp(user_data->result.recognized_name, "") != 0)
        {
            PRINTF("%s : name %s score %d(%%)\r\n", MOBILEFACENET_NAME,
                                                user_data->result.recognized_name, user_data->result.similarity_percentage);
            PRINTF("Mobilefacenet inference time %u ms \r\n", user_data->inference_time_ms);
        }
    }
    else // MODEL_ANTISPOOFING
    {
        PRINTF("The antispoofing inference time %u ms \r\n", user_data->inference_time_ms);
        if(user_data->liveness.result[1] > SPOOFING_THRESHOLD)
        {
            PRINTF("%s : Real face, confidence score %d\r\n", ANTISPOOFING_NAME, user_data->liveness.result[1]);
        }
        else
        {
            PRINTF("%s : Fake face, confidence score %d\r\n", ANTISPOOFING_NAME, user_data->liveness.result[0]);
        }
    
    }

#ifdef DISPLAY_CPU_LOAD
    mpp_stats_disable(MPP_STATS_GRP_API);
    PRINTF("CPU Load: %u(%%)\n\r", user_data->api_stats->api.cpu_load);
    mpp_stats_enable(MPP_STATS_GRP_API);
#endif

#ifdef DISPLAY_FPS
    mpp_stats_disable(MPP_STATS_GRP_MPP);
    PRINTF("FPS: %d, IPS: %d\n\r", user_data->mp_stats->mpp.fps, user_data->mp_bg_stats->mpp.fps);
    mpp_stats_enable(MPP_STATS_GRP_MPP);
#endif

    return;
}

void enable_camera_ir_stream(user_data_t *user_data)
{
    mpp_element_params_t update_cam_params;
    memset(&update_cam_params, 0 , sizeof(update_cam_params));

    update_cam_params.camera.n_streams = 2;
    update_cam_params.camera.stream[0].active = true;
    update_cam_params.camera.stream[0].type = DISPLAY_STREAM_TYPE;
    update_cam_params.camera.stream[1].active = true;
    update_cam_params.camera.stream[1].type = INFERENCE_STREAM_TYPE;
    update_cam_params.camera.in_advance_enqueue = IN_ADVANCE_FRAME_ENQUEUE;

    if (mpp_element_update(user_data->mp, user_data->cam_elem, &update_cam_params, false) != MPP_SUCCESS)
    {
        PRINTF("FAILED to update camera parameters\r\n");
    }
}

void disable_camera_ir_stream(user_data_t *user_data)
{
    mpp_element_params_t update_cam_params;
    memset(&update_cam_params, 0 , sizeof(update_cam_params));

    update_cam_params.camera.n_streams = 2;
    update_cam_params.camera.stream[0].active = true;
    update_cam_params.camera.stream[0].type = DISPLAY_STREAM_TYPE;
    update_cam_params.camera.stream[1].active = false;
    update_cam_params.camera.stream[1].type = INFERENCE_STREAM_TYPE;
    update_cam_params.camera.in_advance_enqueue = IN_ADVANCE_FRAME_ENQUEUE;

    if (mpp_element_update(user_data->mp, user_data->cam_elem, &update_cam_params, false) != MPP_SUCCESS)
    {
        PRINTF("FAILED to update camera parameters\r\n");
    }
}

 /*******************************************************************************
 * Helper Functions for Pipeline Element Addition
 ******************************************************************************/

static int add_camera_element(mpp_t mp, args_t *args, bool stripe_mode, 
                              struct rpmsg_lite_instance *rpmsg_inst,
                              volatile uint16_t *mcmgr_event_data,
                              mpp_elem_handle_t *cam_elem)
{
    static mpp_camera_params_t cam_params;
    memset(&cam_params, 0, sizeof(cam_params));
    cam_params.height = DISPLAY_STREAM_TYPE == RGB_STREAM ? RGB_CAMERA_HEIGHT : IR_CAMERA_HEIGHT;
    cam_params.width  = DISPLAY_STREAM_TYPE == RGB_STREAM ? RGB_CAMERA_WIDTH : IR_CAMERA_WIDTH;
    cam_params.format = args->src_format;
    cam_params.fps    = 30;
    cam_params.stripe = stripe_mode;
    cam_params.rpmsg_inst = rpmsg_inst;
    cam_params.mcmgr_event_data = mcmgr_event_data;
    cam_params.n_streams = 2;
    cam_params.stream[0].type = DISPLAY_STREAM_TYPE;
    cam_params.stream[0].active = true;
    cam_params.stream[0].height = DISPLAY_STREAM_TYPE == RGB_STREAM ? RGB_CAMERA_HEIGHT : IR_CAMERA_HEIGHT;
    cam_params.stream[0].width = DISPLAY_STREAM_TYPE == RGB_STREAM ? RGB_CAMERA_WIDTH : IR_CAMERA_WIDTH;
    cam_params.stream[1].type = INFERENCE_STREAM_TYPE;
    cam_params.stream[1].active = true;
    cam_params.stream[1].height = INFERENCE_STREAM_TYPE == IR_STREAM ? IR_CAMERA_HEIGHT : RGB_CAMERA_HEIGHT;
    cam_params.stream[1].width = INFERENCE_STREAM_TYPE == IR_STREAM ? IR_CAMERA_WIDTH : RGB_CAMERA_WIDTH;
    cam_params.in_advance_enqueue = IN_ADVANCE_FRAME_ENQUEUE;

    PRINTF("Debug: About to add a USB camera\r\n");
    int ret = mpp_camera_add(mp, args->camera_name, &cam_params, cam_elem);
    if (ret) {
        PRINTF("Failed to add camera %s\n", args->camera_name);
        return ret;
    }
    PRINTF("Debug: Added an USB camera \r\n");
    return 0;
}

static int add_jpeg_decoder_element(mpp_t mp, args_t *args, int width, int height)
{
    if (args->src_format != MPP_PIXEL_JPEG) {
        return 0; // Skip if not JPEG format
    }
    
    mpp_element_params_t elem_params_decoder;
    memset(&elem_params_decoder, 0, sizeof(mpp_element_params_t));
    elem_params_decoder.decode.dev_name = IMG_DECODE_DEV_NAME;
    elem_params_decoder.decode.width = width;
    elem_params_decoder.decode.height = height;

    if (strcmp(IMG_DECODE_DEV_NAME, "jpeg_CPU") == 0)
        elem_params_decoder.decode.out_format = MPP_PIXEL_BGR;
    else if (strcmp(IMG_DECODE_DEV_NAME, "jpeg_HW") == 0)
        elem_params_decoder.decode.out_format = MPP_PIXEL_YUYV;

    int ret = mpp_element_add(mp, MPP_ELEMENT_IMG_DECODE, &elem_params_decoder, NULL);
    if (ret) {
        PRINTF("Failed to add element DECODE\n");
        return ret;
    }
    PRINTF("Added HW jpeg decoder to the branch!\r\n");
    return 0;
}

static int add_inference_convert_element(mpp_t mp_bg, mpp_elem_handle_t *infer_conv_h, mpp_element_params_t *infer_conv_params)
{
    memset(infer_conv_params, 0, sizeof(mpp_element_params_t));
    
    /* pick GFX device */
    infer_conv_params->convert.dev_name = APP_GFX_BACKEND_INFER_NAME;
    /* set output buffer dims */
    infer_conv_params->convert.out_buf.width = SCRFD_KPS_WIDTH;
    infer_conv_params->convert.out_buf.height = SCRFD_KPS_HEIGHT;
    /* color convert */
    infer_conv_params->convert.pixel_format = SCRFD_KPS_PIXEL_FORMAT;
    infer_conv_params->convert.ops = MPP_CONVERT_COLOR;
    /* crop center of image */
    infer_conv_params->convert.crop.top = CROP_TOP;
    infer_conv_params->convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
    infer_conv_params->convert.crop.left = CROP_LEFT;
    infer_conv_params->convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
    infer_conv_params->convert.ops |= MPP_CONVERT_CROP;
    /* resize: scaling parameters */
    infer_conv_params->convert.scale.width = SCRFD_KPS_WIDTH;
    infer_conv_params->convert.scale.height = SCRFD_KPS_HEIGHT;
    infer_conv_params->convert.ops |= MPP_CONVERT_SCALE;
    /* then add a flip */
    if (SRC_DISPLAY_FLIP != FLIP_NONE) {
        infer_conv_params->convert.flip = SRC_DISPLAY_FLIP;
        infer_conv_params->convert.ops |=  MPP_CONVERT_ROTATE;
    }

    infer_conv_params->convert.stripe_in = false;
    infer_conv_params->convert.stripe_out = false;

    HAL_LOGI("parameters of first image conversion for inference branch: \r\n");
    HAL_LOGI("infer_conv_params.convert.crop.top = %d\r\n", infer_conv_params->convert.crop.top);
    HAL_LOGI("infer_conv_params.convert.crop.bottom = %d\r\n", infer_conv_params->convert.crop.bottom);
    HAL_LOGI("infer_conv_params.convert.crop.left = %d\r\n", infer_conv_params->convert.crop.left);
    HAL_LOGI("infer_conv_params.convert.crop.right = %d\r\n", infer_conv_params->convert.crop.right);
    HAL_LOGI("infer_conv_params.convert.scale.width = %d\r\n", infer_conv_params->convert.scale.width);
    HAL_LOGI("infer_conv_params.convert.scale.height = %d\r\n", infer_conv_params->convert.scale.height);

    int ret = mpp_element_add(mp_bg, MPP_ELEMENT_CONVERT, infer_conv_params, infer_conv_h);
    if (ret) {
        PRINTF("Failed to add element CONVERT\n");
        return ret;
    }
    return 0;
}

static int add_image_quality_check_element(mpp_t mp_bg, mpp_stats_t *img_quality_check_stats,
                                            mpp_element_params_t *img_quality_check_elem_params,
                                            mpp_elem_handle_t *img_quality_check_h)
{
    /* Add element for image quality check */
    memset(img_quality_check_elem_params, 0 , sizeof(mpp_element_params_t));
    img_quality_check_elem_params->stats = img_quality_check_stats;

    int ret = mpp_element_add(mp_bg, MPP_ELEMENT_IMG_QUALITY_CHECK, img_quality_check_elem_params, img_quality_check_h);
    if (ret ) {
        PRINTF("Failed to add image quality check element\n");
        return ret;
    }

    return 0;
}

static void prepare_model_params(mpp_element_params_t *antispoofing_params,
                                 mpp_element_params_t *mobilefacenet_params,
                                 mpp_element_params_t *scrfd_kps_params)
{
    /* prepare the antispoofing model params */
    memset(antispoofing_params, 0, sizeof(mpp_element_params_t));
    antispoofing_params->ml_inference.model_data = antispoofing_data;
    antispoofing_params->ml_inference.model_size = antispoofing_data_len;
    antispoofing_params->ml_inference.model_input_mean = ANTISPOOFING_INPUT_MEAN;
    antispoofing_params->ml_inference.model_input_std = ANTISPOOFING_INPUT_STD;
    antispoofing_params->ml_inference.inference_params.num_inputs = 1;
    antispoofing_params->ml_inference.inference_params.num_outputs = 1;
    antispoofing_params->ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    antispoofing_params->ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;

    /* prepare the mobilefacenet model params */
    memset(mobilefacenet_params, 0, sizeof(mpp_element_params_t));
    mobilefacenet_params->ml_inference.model_data = mobilefacenet_data;
    mobilefacenet_params->ml_inference.model_size = mobilefacenet_data_len;
    mobilefacenet_params->ml_inference.model_input_mean = MOBILEFACENET_INPUT_MEAN;
    mobilefacenet_params->ml_inference.model_input_std = MOBILEFACENET_INPUT_STD;
    mobilefacenet_params->ml_inference.inference_params.num_inputs = 1;
    mobilefacenet_params->ml_inference.inference_params.num_outputs = 1;
    mobilefacenet_params->ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    mobilefacenet_params->ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;

    /* prepare the scrfd_kps model params */
    memset(scrfd_kps_params, 0, sizeof(mpp_element_params_t));
    scrfd_kps_params->ml_inference.model_data = scrfd_kps_data;
    scrfd_kps_params->ml_inference.model_size = scrfd_kps_data_len;
    scrfd_kps_params->ml_inference.model_input_mean = SCRFD_KPS_INPUT_MEAN;
    scrfd_kps_params->ml_inference.model_input_std = SCRFD_KPS_INPUT_STD;
    scrfd_kps_params->ml_inference.inference_params.num_inputs = 1;
    scrfd_kps_params->ml_inference.inference_params.num_outputs = 9;
    scrfd_kps_params->ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    scrfd_kps_params->ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;
}

static int add_inference_element(mpp_t mp_bg, mpp_element_params_t *scrfd_kps_params, 
                                 mpp_elem_handle_t *infer_elem)
{
    /* configure TFlite element with scrfd_kps model */
    int ret = mpp_element_add(mp_bg, MPP_ELEMENT_INFERENCE, scrfd_kps_params, infer_elem);
    if (ret) {
        PRINTF("Failed to add element MPP_ELEMENT_INFERENCE");
        return ret;
    }
    return 0;
}

static int add_display_convert_element(mpp_t mp)
{
    static mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));
    
    /* pick GFX device */
    elem_params.convert.dev_name = APP_GFX_BACKEND_NAME;
    /* set output buffer dims */
    elem_params.convert.out_buf.width =  VIEW_WIDTH;
    elem_params.convert.out_buf.height = VIEW_HEIGHT;
    elem_params.convert.pixel_format = APP_DISPLAY_FORMAT;
    /* scaling parameters */
    elem_params.convert.scale.width =  VIEW_WIDTH;
    elem_params.convert.scale.height = VIEW_HEIGHT;

    HAL_LOGI("parameters of first image conversion for display branch: \r\n");
    HAL_LOGI("elem_params.convert.scale.width = %d\r\n", elem_params.convert.scale.width);
    HAL_LOGI("elem_params.convert.scale.height = %d\r\n", elem_params.convert.scale.height);
    HAL_LOGI("elem_params.convert.out_buf.width = %d\r\n", elem_params.convert.out_buf.width);
    HAL_LOGI("elem_params.convert.out_buf.height = %d\r\n", elem_params.convert.out_buf.height);

    if (SRC_DISPLAY_FLIP != FLIP_NONE)
        elem_params.convert.flip = SRC_DISPLAY_FLIP;

    elem_params.convert.ops |= MPP_CONVERT_COLOR | MPP_CONVERT_ROTATE | MPP_CONVERT_SCALE;

    int ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);
    if (ret) {
        PRINTF("Failed to add element CONVERT\n");
        return ret;
    }
    return 0;
}

static int add_labeled_rectangle_element(mpp_t mp, user_data_t *user_data, mpp_element_params_t *elem_params)
{
    memset(elem_params, 0, sizeof(mpp_element_params_t));
    memset(&user_data->labels, 0, sizeof(user_data->labels));

    /* params init */
    elem_params->labels.max_rect = MAX_LABEL_RECTS;
    elem_params->labels.detected_rect = 1;
    elem_params->labels.rectangles = user_data->labels;
    elem_params->labels.max_landmk = MAX_LABEL_RECTS * SCRFD_NUM_LANDMARKS;

    /* first add recognition zone box */
    user_data->labels[ZONE_BOX_INDEX].top    = RECO_ZONE_RECT_TOP;
    user_data->labels[ZONE_BOX_INDEX].left   = RECO_ZONE_RECT_LEFT;
    user_data->labels[ZONE_BOX_INDEX].bottom = RECO_ZONE_RECT_TOP + RECO_ZONE_RECT_HEIGHT;
    user_data->labels[ZONE_BOX_INDEX].right  = RECO_ZONE_RECT_LEFT + RECO_ZONE_RECT_WIDTH;
    user_data->labels[ZONE_BOX_INDEX].line_width = RECT_LINE_WIDTH;
    user_data->labels[ZONE_BOX_INDEX].line_color.rgb.G = 0xff;
    strcpy((char *)user_data->labels[ZONE_BOX_INDEX].label, ZONE_LABEL_RECO);

    int ret = mpp_element_add(mp, MPP_ELEMENT_LABELED_RECTANGLE, elem_params, &user_data->labrect_elem);
    if (ret) {
        PRINTF("Failed to add element LABELED_RECTANGLE (0x%x)\r\n", ret);
        return ret;
    }

    return 0;
}

static int add_compose_element(mpp_t mp, user_data_t *user_data, text_info_t *txt_info)
{
    static mpp_element_params_t elem_params_compose;
    memset(&elem_params_compose, 0, sizeof(mpp_element_params_t));
    mpp_stats_t compose_stats = {0};
    elem_params_compose.stats = &compose_stats;

    init_text_area(g_text_img, sizeof(g_text_img), txt_info);

    /* Output params */
    elem_params_compose.compose.out_angle = ROTATE_270;
    elem_params_compose.compose.out_flip = FLIP_NONE;
    elem_params_compose.compose.out_format = APP_DISPLAY_FORMAT;
    elem_params_compose.compose.out_width = APP_DISPLAY_WIDTH;
    elem_params_compose.compose.out_height = APP_DISPLAY_HEIGHT;

    /* Set number of images to compose */
    elem_params_compose.compose.nb_images = COMPOSE_MAX_IMAGES;

    /* Allocate memory for image list */
    static mpp_img_compose_param_t compose_images[COMPOSE_MAX_IMAGES];
    memset(compose_images, 0, sizeof(compose_images));
    elem_params_compose.compose.image_list = compose_images;

    /* Configure logo image */
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].width = LOGO_WIDTH;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].height = LOGO_HEIGHT;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].format = MPP_PIXEL_RGB;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].buffer = (void *) NXP_Logo_RGB888_Colour_320_map;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].dest_area.left = LOGO_LEFT_POS;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].dest_area.top = LOGO_TOP_POS;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].dest_area.right = LOGO_RIGHT_POS;
    elem_params_compose.compose.image_list[COMPOSE_LOGO_INDEX].dest_area.bottom = LOGO_BOTTOM_POS;

    /* Configure text image */
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].width = TEXT_WIDTH;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].height = TEXT_HEIGHT;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].format = MPP_PIXEL_RGB565;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].buffer = g_text_img;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.left = TEXT_LEFT_POS;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.top = TEXT_TOP_POS;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.right = TEXT_RIGHT_POS;
    elem_params_compose.compose.image_list[COMPOSE_TEXT_INDEX].dest_area.bottom = TEXT_BOTTOM_POS;

    /* Configure Inference preview image */
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].width = INFPVW_WIDTH;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].height = INFPVW_HEIGHT;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].format = MPP_PIXEL_RGB;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].buffer = NULL;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].dest_area.left = INFPVW_LEFT_POS;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].dest_area.top = INFPVW_TOP_POS;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].dest_area.right = INFPVW_RIGHT_POS;
    elem_params_compose.compose.image_list[COMPOSE_INFPVW_INDEX].dest_area.bottom = INFPVW_BOTTOM_POS;
    user_data->p_params_compose = &elem_params_compose;

    /* Input area - full frame */
    elem_params_compose.compose.input_area.left = 0;
    elem_params_compose.compose.input_area.top = APP_DISPLAY_HEIGHT - (int)(APP_DISPLAY_WIDTH * FRAME_ASP_RATIO);
    elem_params_compose.compose.input_area.bottom = APP_DISPLAY_HEIGHT - 1;
    elem_params_compose.compose.input_area.right = APP_DISPLAY_WIDTH - 1;

    int ret = mpp_element_add(mp, MPP_ELEMENT_IMG_COMPOSE, &elem_params_compose, &user_data->compose_elem);
    if (ret) {
        PRINTF("Failed to add element IMG_COMPOSE (0x%x)\r\n", ret);
        return ret;
    }
    return 0;
}

static int add_display_element(mpp_t mp, args_t *args)
{
    static mpp_display_params_t disp_params;
    memset(&disp_params, 0, sizeof(disp_params));
    disp_params.format = APP_DISPLAY_FORMAT;
    disp_params.width  = APP_DISPLAY_WIDTH;
    disp_params.height = APP_DISPLAY_HEIGHT;
    disp_params.stripe = false;
    
    int ret = mpp_display_add(mp, args->display_name, &disp_params);
    if (ret) {
        PRINTF("Failed to add display %s\n", args->display_name);
        return ret;
    }
    return 0;
}

static int setup_pipeline_elements(mpp_t mp, args_t *args, bool stripe_mode,
                                   user_data_t *user_data, text_info_t *txt_info,
                                   mpp_element_params_t *antispoofing_params,
                                   mpp_element_params_t *mobilefacenet_params,
                                   mpp_element_params_t *scrfd_kps_params,
                                   mpp_elem_handle_t *infer_conv_h,
                                   mpp_element_params_t *infer_conv_params,
                                   mpp_element_params_t *label_rect_params,
                                   mpp_stats_t *img_quality_check_stats,
                                   mpp_element_params_t *img_quality_check_elem_params,
                                   mpp_elem_handle_t *img_quality_check_h)
{
    int ret = 0;
    static mpp_elem_handle_t cam_elem;
    static mpp_t mp_split;
    static mpp_t mp_bg;
    mpp_params_t mpp_params;

    // Add camera element
    ret = add_camera_element(mp, args, stripe_mode, rpmsg_inst, mcmgr_event_data, &cam_elem);
    if (ret) goto err;
    user_data->cam_elem = cam_elem;

    /* Split the pipeline into 2 branches:
     * - first for the conversion to model
     * - second for the label-rect draw & display
     * This order is needed to avoid running inference on an image containing label-rect */
    memset(&mpp_params, 0, sizeof(mpp_params));
    mpp_params.evt_callback_f = &mpp_event_listener;
    mpp_params.mask = MPP_EVENT_ALL;
    mpp_params.cb_userdata = user_data;
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;
    ret = mpp_split(mp, 1, &mpp_params, &mp_split);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }
    user_data->mp_split = mp_split;

    // Add JPEG decoder for split branch (inference stream)
    ret = add_jpeg_decoder_element(mp_split, args, 
        INFERENCE_STREAM_TYPE == IR_STREAM ? IR_CAMERA_WIDTH : RGB_CAMERA_WIDTH, 
        INFERENCE_STREAM_TYPE == IR_STREAM ? IR_CAMERA_HEIGHT : RGB_CAMERA_HEIGHT);
    if (ret) goto err;

    // Create background branch for ML inference
    static mpp_stats_t mp_bg_stats;
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;
    mpp_params.stats = &mp_bg_stats;
    ret = mpp_background(mp_split, &mpp_params, &mp_bg);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }
    user_data->mp_bg = mp_bg;
    user_data->mp_bg_stats = &mp_bg_stats;

    // Add inference convert element
    ret = add_inference_convert_element(mp_bg, infer_conv_h, infer_conv_params);
    if (ret) goto err;

    ret = add_image_quality_check_element(mp_bg, img_quality_check_stats, 
                                         img_quality_check_elem_params, 
                                         img_quality_check_h);
    if (ret) goto err;

    // Prepare all model parameters (needed for later use in the application)
    prepare_model_params(antispoofing_params, mobilefacenet_params, scrfd_kps_params);

    // Add inference element (initially configured with scrfd_kps)
    ret = add_inference_element(mp_bg, scrfd_kps_params, &user_data->infer_elem);
    if (ret) goto err;

    // Close the inference pipeline with a null sink
    ret = mpp_nullsink_add(mp_bg);
    if (ret) {
        PRINTF("Failed to add NULL sink\n");
        goto err;
    }

    // Add JPEG decoder for display branch
    ret = add_jpeg_decoder_element(mp, args, 
            DISPLAY_STREAM_TYPE == RGB_STREAM ? RGB_CAMERA_WIDTH : IR_CAMERA_WIDTH, 
            DISPLAY_STREAM_TYPE == RGB_STREAM ? RGB_CAMERA_HEIGHT : IR_CAMERA_HEIGHT);
    if (ret) goto err;

    // Add display convert element
    ret = add_display_convert_element(mp);
    if (ret) goto err;

    // Add labeled rectangle element
    ret = add_labeled_rectangle_element(mp, user_data, label_rect_params);
    if (ret) goto err;

    // Add compose element
    ret = add_compose_element(mp, user_data, txt_info);
    if (ret) goto err;

    // Add display element
    ret = add_display_element(mp, args);
    if (ret) goto err;

    return 0;

err:
    return ret;
}

int mpp_face_reg_request_handler(user_data_t *user_data, msg_t *received_msg, mpp_element_params_t *infer_conv_params, mpp_elem_handle_t infer_conv_h, mpp_element_params_t *scrfd_kps_params)
{
    int ret = 0;

    // Extract the name from the message parameter
    face_reg_req_msg_payload_t *reg_params = &received_msg->reg_payload;
    char *registration_name = reg_params->name;
    int registration_timeout = reg_params->timeout * 1000; // convert to milliseconds

    if (!user_data_access(user_data, USER_DATA_WAIT_TIMEOUT_MS))
    {
        PRINTF("ERR: Could not start face registration\r\n");
        return 1;
    }

        // Store the name for registration (you might need to add this to user_data)
    if (registration_name != NULL && registration_name[0] != '\0' && registration_timeout != 0)
    {
        // Store the name somewhere accessible (add to user_data_t if needed)
        strcpy(user_data->registration_name, registration_name);
        user_data->registration_timeout = registration_timeout;

        // Change state to detecting
        user_data->state = STATE_DETECTING;
        /* activate registraion mode */
        user_data->registration_mode_on = 1;

        if (first_mpp_start == true)
        {
            /* start preempt-able pipeline branch */
            ret = mpp_start(user_data->mp_bg, 0, false);
            if (ret) {
                PRINTF("Failed to start preempt-able pipeline branch");
                return ret;
            }
            /* start secondary pipeline branch */
            ret = mpp_start(user_data->mp_split, 0, false);
            if (ret) {
                PRINTF("Failed to start secondary pipeline branch");
                return ret;
            }
            /* start main pipeline branch */
            ret = mpp_start(user_data->mp, first_mpp_start, false);
            if (ret) {
                PRINTF("Failed to start main pipeline branch");
                return ret;
            }

            first_mpp_start = false;
        }
        else
        {
            /* stop the inference pippelines */
            if (mpp_is_running(user_data->mp_bg))
                mpp_stop(user_data->mp_bg);
            if (mpp_is_running(user_data->mp_split))
            {
                disable_camera_ir_stream(user_data);
                mpp_stop(user_data->mp_split);
            }

            /* Start main pipeline if not running */
            if (!mpp_is_running(user_data->mp))
                mpp_start(user_data->mp, 0, false);

            vTaskDelay(pdMS_TO_TICKS(100)); // Short delay to ensure pipeline stops
        
            /* Start with the face detection model */
            /* update convert params for SCRFD */
            infer_conv_params->convert.crop.top = CROP_TOP;
            infer_conv_params->convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
            infer_conv_params->convert.crop.left = CROP_LEFT;
            infer_conv_params->convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
            infer_conv_params->convert.out_buf.width = SCRFD_KPS_WIDTH;
            infer_conv_params->convert.out_buf.height = SCRFD_KPS_HEIGHT;
            infer_conv_params->convert.scale.width = SCRFD_KPS_WIDTH;
            infer_conv_params->convert.scale.height = SCRFD_KPS_HEIGHT;
            infer_conv_params->convert.pixel_format = SCRFD_KPS_PIXEL_FORMAT;
            ret = mpp_element_update(user_data->mp_bg, infer_conv_h, infer_conv_params, false);
            if (ret) {
                PRINTF("Failed to update element convert for scrfd_kps\r\n");
                return ret;
            }

            /* switch to SCRFD_KPS */
            ret = mpp_element_update(user_data->mp_bg, user_data->infer_elem, scrfd_kps_params, false);
            if (ret) {
                PRINTF("Failed to update element inference for scrfd_kps\r\n");
                return ret;
            }
            enable_camera_ir_stream(user_data);
            /* start preempt-able pipeline branch */
            ret = mpp_start(user_data->mp_bg, 0, false);
            if (ret) {
                PRINTF("Failed to start preempt-able pipeline branch");
                return ret;
            }
            /* start secondary pipeline branch */
            ret = mpp_start(user_data->mp_split, 0, false);
            if (ret) {
                PRINTF("Failed to start secondary pipeline branch");
                return ret;
            }
        }

        user_data->cur_model = MODEL_SCRFD_KPS;

        /* start registration timer */
        registration_start_time = xTaskGetTickCount();
        registration_end_time = 0; // Reset end time

        // Configure and start existing timer
        if (registration_timer != NULL) {
            xTimerChangePeriod(registration_timer, pdMS_TO_TICKS(user_data->registration_timeout), 0);
            registration_timer_expired = false;
            xTimerStart(registration_timer, 0);
            PRINTF("Registration timer started for %d ms\r\n", user_data->registration_timeout);
        }
    }
    else
    {
        if (registration_name == NULL || registration_name[0] == '\0')
            PRINTF("[ERR] Provided registration name is NULL. Could not start face registration\r\n");
        if (registration_timeout == 0)
            PRINTF("[ERR] Provided registration timeout is 0. Could not start face registration\r\n");
    }

    user_data_release(user_data);

    return ret;
}

int mpp_fr_enable_lcd_on_handler(user_data_t *user_data, mpp_element_params_t *infer_conv_params,
                                 mpp_elem_handle_t infer_conv_h, mpp_element_params_t *scrfd_kps_params,
                                 mpp_element_params_t* img_quality_check_elem_params,
                                 mpp_elem_handle_t img_quality_check_h)
{
    int ret = 0;

    if (!user_data_access(user_data, USER_DATA_WAIT_TIMEOUT_MS))
    {
        PRINTF("ERR: Could not start lcd and FR\r\n");
        return 1;
    }

    /* Check if this is the first time we start the pipeline */
    if (!first_mpp_start)
    {
        /* If it's not, stop pipelines if they are running */
        if (mpp_is_running(user_data->mp_bg))
            mpp_stop(user_data->mp_bg);
        if (mpp_is_running(user_data->mp_split))
        {
            disable_camera_ir_stream(user_data);
            mpp_stop(user_data->mp_split);
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Short delay to ensure pipeline stops

        /* Start with the face detection model */
        /* update convert params for SCRFD */
        infer_conv_params->convert.crop.top = CROP_TOP;
        infer_conv_params->convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
        infer_conv_params->convert.crop.left = CROP_LEFT;
        infer_conv_params->convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
        infer_conv_params->convert.out_buf.width = SCRFD_KPS_WIDTH;
        infer_conv_params->convert.out_buf.height = SCRFD_KPS_HEIGHT;
        infer_conv_params->convert.scale.width = SCRFD_KPS_WIDTH;
        infer_conv_params->convert.scale.height = SCRFD_KPS_HEIGHT;
        infer_conv_params->convert.pixel_format = SCRFD_KPS_PIXEL_FORMAT;
        ret = mpp_element_update(user_data->mp_bg, infer_conv_h, infer_conv_params, false);
        if (ret) {
            PRINTF("Failed to update element convert for scrfd_kps\r\n");
            return ret;
        }

        /* Disabled image quality check */
        img_quality_check_elem_params->img_quality_check.disable = true;
        ret = mpp_element_update(user_data->mp_bg, img_quality_check_h, img_quality_check_elem_params, false);
        if (ret) {
            PRINTF("Failed to update image quality check element for scrfd_kps\r\n");
            return ret;
        }

        /* switch to SCRFD_KPS */
        ret = mpp_element_update(user_data->mp_bg, user_data->infer_elem, scrfd_kps_params, false);
        if (ret) {
            PRINTF("Failed to update element inference for scrfd_kps\r\n");
            return ret;
        }

        user_data->cur_model = MODEL_SCRFD_KPS;
        user_data->state = STATE_DETECTING;
        enable_camera_ir_stream(user_data);
    }

    user_data_release(user_data);

    /* start preempt-able pipeline branch */
    ret = mpp_start(user_data->mp_bg, 0, false);
    if (ret) {
        PRINTF("Failed to start preempt-able pipeline branch");
        return ret;
    }
    /* start secondary pipeline branch */
    ret = mpp_start(user_data->mp_split, 0, false);
    if (ret) {
        PRINTF("Failed to start secondary pipeline branch");
        return ret;
    }
    if (!mpp_is_running(user_data->mp)) {
        /* start main pipeline branch */
        ret = mpp_start(user_data->mp, first_mpp_start, false);
        if (ret) {
            PRINTF("Failed to start main pipeline branch");
            return ret;
        }
    }

    first_mpp_start = false;

    return ret;
}

int mpp_fr_disable_lcd_on_handler(user_data_t *user_data)
{
    int ret = 0;

    /* Stop the inference branches, but keep display on */
    if (mpp_is_running(user_data->mp_bg))
        mpp_stop(user_data->mp_bg);
    if (mpp_is_running(user_data->mp_split))
    {
        disable_camera_ir_stream(user_data);
        mpp_stop(user_data->mp_split);
    }

    vTaskDelay(pdMS_TO_TICKS(100)); // Short delay to ensure pipeline stops

    /* update labeled rectangle */
    mpp_element_params_t rect_params;
    memset(&rect_params, 0, sizeof(rect_params));
    /* detected_count contains only the detection zone box */
    rect_params.labels.detected_rect = 1;
    rect_params.labels.max_rect = MAX_LABEL_RECTS;
    rect_params.labels.rectangles = user_data->labels;
    mpp_element_update(user_data->mp, user_data->labrect_elem, &rect_params, false);

    /* Start display if not started already */
    if (!mpp_is_running(user_data->mp))
    {
        if (first_mpp_start)
        {
            /* Hack to set the state of the seconday branhes to stopped instead of not started */
            mpp_start(user_data->mp_bg, 0, false);
            mpp_start(user_data->mp_split, 0, false);
            disable_camera_ir_stream(user_data);
            mpp_stop(user_data->mp_bg);
            mpp_stop(user_data->mp_split);
        }

        /* In case this is the first mpp start, we need to allocated the memory for the pipelines */
        ret = mpp_start(user_data->mp, first_mpp_start, false);
        if (ret) {
            PRINTF("Failed to start main pipeline branch");
            return ret;
        }
        first_mpp_start = false;
    }

    /* Clear the user data state to avoid any residual state */
    user_data->state = STATE_DETECTING;

    return ret;
}

int mpp_fr_disable_lcd_off_handler(user_data_t *user_data)
{
    int ret = 0;

    if (mpp_is_running(user_data->mp_bg))
        mpp_stop(user_data->mp_bg);
    if (mpp_is_running(user_data->mp_split))
    {
        disable_camera_ir_stream(user_data);
        mpp_stop(user_data->mp_split);
    }
    if (mpp_is_running(user_data->mp))
        mpp_stop(user_data->mp);

    vTaskDelay(pdMS_TO_TICKS(100)); // Short delay to ensure pipeline stops

    /* Clear the user data state to avoid any residual state */
    user_data->state = STATE_DETECTING;

    return ret;
}

/*******************************************************************************
 * State Handler Functions
 ******************************************************************************/

static int handle_state_detected(user_data_t *user_data, mpp_t mp, mpp_t mp_bg, 
                                  mpp_elem_handle_t infer_conv_h,
                                  mpp_element_params_t *infer_conv_params,
                                  mpp_element_params_t *antispoofing_params,
                                  mpp_element_params_t *label_rect_params)
{
    int ret = 0;
    
    /* stop branches with convert and inference elements */
    mpp_stop(user_data->mp_bg);
    /* A face was detected, process it */
    ctrl_task_notify(CTRL_TASK_FACE_DETECTED, NULL);
    
#if USE_ANTISPOOFING
    /* update convert params for the antispoofing model */
    if (get_face_crop_area(&user_data->final_boxes[0], &infer_conv_params->convert.crop))
    {
        PRINTF("Failed to get face crop area\r\n");
        return -1;
    }
    
    if (SRC_DISPLAY_FLIP == FLIP_HORIZONTAL)
    {
        uint32_t temp_width = infer_conv_params->convert.crop.right - infer_conv_params->convert.crop.left;
        infer_conv_params->convert.crop.left = INF_SRC_WIDTH - infer_conv_params->convert.crop.right - 1;
        infer_conv_params->convert.crop.right = infer_conv_params->convert.crop.left + temp_width;
    }
    
    infer_conv_params->convert.out_buf.width = ANTISPOOFING_WIDTH;
    infer_conv_params->convert.out_buf.height = ANTISPOOFING_HEIGHT;
    infer_conv_params->convert.scale.width = ANTISPOOFING_WIDTH;
    infer_conv_params->convert.scale.height = ANTISPOOFING_HEIGHT;
    infer_conv_params->convert.pixel_format = ANTISPOOFING_PIXEL_FORMAT;
    
    ret = mpp_element_update(mp_bg, infer_conv_h, infer_conv_params, true);
    if (ret)
    {
        PRINTF("Failed to update element convert for the antispoofing model\r\n");
        return ret;
    }

    /* switch to antispoofing model */
    ret = mpp_element_update(mp_bg, user_data->infer_elem, antispoofing_params, true);
    if (ret)
    {
        PRINTF("Failed to update element inference for the antispoofing model\r\n");
        return ret;
    }
    
    if (user_data->labrect_elem != 0)
    {
        label_rect_params->labels.detected_rect = user_data->detected_count + 1;
        label_rect_params->labels.max_rect = MAX_LABEL_RECTS;
        label_rect_params->labels.rectangles = user_data->labels;
        mpp_element_update(mp, user_data->labrect_elem, label_rect_params, true);
    }
    
    user_data->cur_model = MODEL_ANTISPOOFING;
    user_data->state = STATE_CHECKING_ANTISPOOFING;
    mpp_start(mp_bg, 0, true);
#else
    /* Skipping Antispoofing, going directly to MobileFaceNet */
    user_data->state = STATE_REAL;
#endif

    return ret;
}

static int handle_state_spoof(user_data_t *user_data, text_info_t *txt_info,
                               int detection_inference_time, int antispoofing_inference_time,
                               TickType_t tick, TickType_t *notifyTime)
{
    /* stop branches with convert and inference elements */
    /* until the user is notified */
    mpp_stop(user_data->mp_bg);

    txt_info->detection_inference_time = detection_inference_time;
#if USE_ANTISPOOFING
    txt_info->antispoofing_inference_time = antispoofing_inference_time;
#endif
    strcpy(txt_info->state, "Fake face");
    txt_info->recognition_inference_time = 0;
    txt_info->confidence = 0.0f;
    draw_text_area(txt_info);

    /* start timer */
    *notifyTime = tick;
    user_data->state = STATE_NOTIFYING_USER;

    return 0;
}

static int handle_state_real(user_data_t *user_data, mpp_t mp, mpp_t mp_bg,
                              mpp_elem_handle_t infer_conv_h,
                              mpp_element_params_t *infer_conv_params,
                              mpp_element_params_t *mobilefacenet_params,
                              mpp_element_params_t *label_rect_params,
                              mpp_element_params_t* img_quality_check_elem_params,
                              mpp_elem_handle_t img_quality_check_h)
{
    int ret = 0;
    
    /* PRINTF("Switching to MobileFaceNet \r\n"); */
    /* update convert params for mobilefacenet */
#if USE_ANTISPOOFING
    mpp_stop(mp_bg);
#endif
    
    if (get_face_crop_area(&user_data->final_boxes[0], &infer_conv_params->convert.crop))
    {
        PRINTF("Failed to get face crop area\r\n");
        return -1;
    }
    
    if (SRC_DISPLAY_FLIP == FLIP_HORIZONTAL)
    {
        uint32_t temp_width = infer_conv_params->convert.crop.right - infer_conv_params->convert.crop.left;
        infer_conv_params->convert.crop.left = INF_SRC_WIDTH - infer_conv_params->convert.crop.right - 1;
        infer_conv_params->convert.crop.right = infer_conv_params->convert.crop.left + temp_width;
    }

    infer_conv_params->convert.out_buf.width = MOBILEFACENET_WIDTH;
    infer_conv_params->convert.out_buf.height = MOBILEFACENET_HEIGHT;
    infer_conv_params->convert.scale.width = MOBILEFACENET_WIDTH;
    infer_conv_params->convert.scale.height = MOBILEFACENET_HEIGHT;
    infer_conv_params->convert.pixel_format = MOBILEFACENET_PIXEL_FORMAT;
    
    ret = mpp_element_update(user_data->mp_bg, infer_conv_h, infer_conv_params, true);
    if (ret)
    {
        PRINTF("Failed to update element convert for mobilefacenet\r\n");
        return ret;
    }

    img_quality_check_elem_params->img_quality_check.disable = false;
    ret = mpp_element_update(user_data->mp_bg, img_quality_check_h, img_quality_check_elem_params, true);
    if (ret) {
        PRINTF("Failed to update image quality check element for mobilefacenet\r\n");
        return ret;
    }

    /* switch to mobilefacenet model */
    ret = mpp_element_update(user_data->mp_bg, user_data->infer_elem, mobilefacenet_params, true);
    if (ret)
    {
        PRINTF("Failed to update element inference for mobilefacenet\r\n");
        return ret;
    }

    /* update zone label */
    /* detected_count contains at least the detection zone box */
    if (user_data->labrect_elem != 0)
    {
        label_rect_params->labels.detected_rect = user_data->detected_count + 1;
        label_rect_params->labels.max_rect = MAX_LABEL_RECTS;
        label_rect_params->labels.rectangles = user_data->labels;
        mpp_element_update(mp, user_data->labrect_elem, label_rect_params, true);
    }
    
    user_data->cur_model = MODEL_MOBILEFACENET;
    user_data->state = STATE_RECOGNIZING;
    mpp_start(user_data->mp_bg, 0, true);
    
    return 0;
}

static int handle_state_recognized(user_data_t *user_data, text_info_t *txt_info,
                                    int detection_inference_time, int recognition_inference_time,
                                    int antispoofing_inference_time,
                                    TickType_t tick, TickType_t *notifyTime)
{
    /* stop branches with convert and inference elements */
    /* until the user is notified */
    mpp_stop(user_data->mp_bg);

    /* A face was recognized, process it */
    /* Create and fill recognition result structure from user data result */
    msg_t *ctrl_task_notify_msg = (msg_t *) pvPortMalloc(sizeof(msg_t));
    strcpy(ctrl_task_notify_msg->fr_payload.name, user_data->result.recognized_name);
    ctrl_task_notify_msg->fr_payload.similarity = user_data->result.similarity_percentage;

    /* update text area */
    strcpy(txt_info->name, user_data->result.recognized_name);
    txt_info->confidence = user_data->result.similarity_percentage;
    txt_info->detection_inference_time = detection_inference_time;
    txt_info->recognition_inference_time = recognition_inference_time;
#if USE_ANTISPOOFING   
    txt_info->antispoofing_inference_time = antispoofing_inference_time;
    strcpy(txt_info->state, "Real face");
#endif
    draw_text_area(txt_info);

    /* communicate updates to ctrl task*/
    ctrl_task_notify(CTRL_TASK_FACE_RECOGNIZED, ctrl_task_notify_msg);

    /* start timer */
    *notifyTime = tick;
    user_data->state = STATE_NOTIFYING_USER;

    return 0;
}

static int handle_state_not_recognized(user_data_t *user_data, mpp_t mp,
                                        text_info_t *txt_info,
                                        int detection_inference_time, int recognition_inference_time,
                                        int antispoofing_inference_time,
                                        mpp_element_params_t *label_rect_params,
                                        TickType_t tick, TickType_t *notifyTime)
{
    /* A face was not recognized, process it */
    /* stop branches with convert and inference elements */
    /* until the user is notified */
    mpp_stop(user_data->mp_bg);

    if (user_data->registration_mode_on == 1 && user_data->image_brightness_ok && user_data->image_contrast_ok)
    {
        /* register new face embedding */
        int ctrl_task_msg;
        set_new_face_embeddings((const float *)user_data->result.embedding);
        registration_end_time = xTaskGetTickCount();
        
        if (!database_add(user_data->registration_name))
        {
            ctrl_task_msg = CTRL_TASK_FACE_REG_TIMEOUT;
        }
        else
        {
            ctrl_task_msg = CTRL_TASK_FACE_REG_COMPLETE;
            PRINTF("Registration completed successfully in %d ms\r\n",
                    (registration_end_time - registration_start_time) * portTICK_PERIOD_MS);
        }

        /* Send a message to our task to disable the FR */
        mpp_task_notify(MPP_TASK_FR_DISABLE_LCD_ON, NULL);
        /* Send a message to control task */
        ctrl_task_notify(ctrl_task_msg, NULL);

        // Stop the timer when registration completes (success or failure)
        if (registration_timer != NULL)
            xTimerStop(registration_timer, 0);
        registration_timer_expired = false;

        /* registration should be cleared */
        strcpy(user_data->registration_name, "\0");
        user_data->registration_mode_on = 0;

        /* update zone label */
        /* detected_count contains at least the detection zone box */
        if (user_data->labrect_elem != 0)
        {
            label_rect_params->labels.detected_rect = user_data->detected_count + 1;
            label_rect_params->labels.max_rect = MAX_LABEL_RECTS;
            label_rect_params->labels.rectangles = user_data->labels;
            mpp_element_update(mp, user_data->labrect_elem, label_rect_params, true);
        }
    }
    else
    {
        ctrl_task_notify(CTRL_TASK_FACE_NOT_RECOGNIZED, NULL);

        if (user_data->labrect_elem != 0)
        {
            label_rect_params->labels.detected_rect = user_data->detected_count + 1;
            label_rect_params->labels.max_rect = MAX_LABEL_RECTS;
            label_rect_params->labels.rectangles = user_data->labels;
            const char *quality_label = 
                (!user_data->image_brightness_ok && !user_data->image_contrast_ok) ? FACE_LABEL_POOR_QUALITY :
                (!user_data->image_brightness_ok) ? FACE_LABEL_POOR_BRIGHTNESS :
                (!user_data->image_contrast_ok) ? FACE_LABEL_POOR_CONTRAST : NULL;

            if (quality_label != NULL)
            {
                strncpy((char *)label_rect_params->labels.rectangles[FIRST_FACE_INDEX].label, 
                        quality_label, sizeof(label_rect_params->labels.rectangles[FIRST_FACE_INDEX].label));
            }
            mpp_element_update(mp, user_data->labrect_elem, label_rect_params, true);
        }
        
        strcpy(txt_info->name, "Unknown");
        txt_info->confidence = 0.0f;
        txt_info->detection_inference_time = detection_inference_time;
        txt_info->recognition_inference_time = recognition_inference_time;
    #if USE_ANTISPOOFING    
        txt_info->antispoofing_inference_time = antispoofing_inference_time;
        strcpy(txt_info->state, "Real face");
    #endif
        draw_text_area(txt_info);
    }

    /* start timer */
    *notifyTime = tick;
    user_data->state = STATE_NOTIFYING_USER;

    return 0;
}

static int handle_state_notifying_user(user_data_t *user_data, TickType_t tick,
                                        TickType_t notifyTime, TickType_t notifyDelay)
{
    if (tick > notifyTime + notifyDelay)
    {
        /* stop timer */
        user_data->state = STATE_USER_NOTIFIED;
    }
    
    return 0;
}

static int handle_state_user_notified(user_data_t *user_data, mpp_elem_handle_t infer_conv_h,
                                       mpp_element_params_t *infer_conv_params,
                                       mpp_element_params_t *scrfd_kps_params,
                                       mpp_element_params_t* img_quality_check_elem_params,
                                       mpp_elem_handle_t img_quality_check_h)
{
    int ret = 0;
    
    /* PRINTF("Switching to SCRFD KPS \r\n"); */
    /* update convert params for scrfd_kps */
    infer_conv_params->convert.crop.top = CROP_TOP;
    infer_conv_params->convert.crop.bottom = CROP_TOP + CROP_SIZE_TOP - 1;
    infer_conv_params->convert.crop.left = CROP_LEFT;
    infer_conv_params->convert.crop.right = CROP_LEFT + CROP_SIZE_LEFT - 1;
    infer_conv_params->convert.out_buf.width = SCRFD_KPS_WIDTH;
    infer_conv_params->convert.out_buf.height = SCRFD_KPS_HEIGHT;
    infer_conv_params->convert.scale.width = SCRFD_KPS_WIDTH;
    infer_conv_params->convert.scale.height = SCRFD_KPS_HEIGHT;
    infer_conv_params->convert.pixel_format = SCRFD_KPS_PIXEL_FORMAT;
    
    ret = mpp_element_update(user_data->mp_bg, infer_conv_h, infer_conv_params, false);
    if (ret) 
    {
        PRINTF("Failed to update element convert for scrfd_kps\r\n");
        return ret;
    }

    img_quality_check_elem_params->img_quality_check.disable = true;
    ret = mpp_element_update(user_data->mp_bg, img_quality_check_h, img_quality_check_elem_params, false);
    if (ret) {
        PRINTF("Failed to update image quality check element for scrfd_kps\r\n");
        return ret;
    }

    /* switch to SCRFD_KPS */
    ret = mpp_element_update(user_data->mp_bg, user_data->infer_elem, scrfd_kps_params, false);
    if (ret) 
    {
        PRINTF("Failed to update element inference for scrfd_kps\r\n");
        return ret;
    }
    enable_camera_ir_stream(user_data);
    user_data->cur_model = MODEL_SCRFD_KPS;
    strcpy((char *)user_data->labels[ZONE_BOX_INDEX].label, ZONE_LABEL_RECO);
    user_data->state = STATE_DETECTING;
    mpp_start(user_data->mp_split, 0, false);
    mpp_start(user_data->mp_bg, 0, false);

    return 0;
}

static void app_task(void *params)
{
    args_t *args = (args_t *) params;
    bool stripe_mode = false;
    user_data.cur_model = MODEL_SCRFD_KPS;
    user_data.state = STATE_DETECTING;
    user_data.db = g_embedding_db;
    user_data.db_max = DATABASE_MAX_SIZE;
#ifdef DEBUG_PREVIEW_RECOGNITION
    user_data.inference_view = g_inf_img;
#else
    user_data.inference_view = NULL;
#endif
    int ret = 0;
    /* text info default configuration */
    text_info_t txt_info;
    strcpy(txt_info.name, "");
    txt_info.confidence = 0.0f;
    txt_info.detection_inference_time = 0;
    txt_info.recognition_inference_time = 0;

    int detection_inference_time = 0;
    int recognition_inference_time = 0;
    int antispoofing_inference_time = 0;
#if USE_ANTISPOOFING
    txt_info.antispoofing_inference_time = 0;
#endif
    PRINTF("[%s]\r\n", mpp_get_version());

    PRINTF("Inference Engine: TensorFlow-Lite Micro \r\n");

    /* show constants */
    PRINTF("VIEW_SRC_WIDTH = %d\r\n", VIEW_SRC_WIDTH);
    PRINTF("VIEW_SRC_HEIGHT = %d\r\n", VIEW_SRC_HEIGHT);
    PRINTF("INF_SRC_WIDTH = %d\r\n", INF_SRC_WIDTH);
    PRINTF("INF_SRC_HEIGHT = %d\r\n", INF_SRC_HEIGHT);
    PRINTF("CROP_LEFT = %d\r\n", CROP_LEFT);
    PRINTF("CROP_TOP = %d\r\n", CROP_TOP);
    PRINTF("CROP_SIZE_LEFT = %d\r\n", CROP_SIZE_LEFT);
    PRINTF("CROP_SIZE_TOP = %d\r\n", CROP_SIZE_TOP);
    PRINTF("RECO_CROP_LEFT = %d\r\n", RECO_CROP_LEFT);
    PRINTF("RECO_CROP_TOP = %d\r\n", RECO_CROP_TOP);
    PRINTF("RECO_CROP_SIZE_LEFT = %d\r\n", RECO_CROP_SIZE_LEFT);
    PRINTF("RECO_CROP_SIZE_TOP = %d\r\n", RECO_CROP_SIZE_TOP);
    PRINTF("\r\n");
    PRINTF("DISPLAY_LARGE_DIM = %d\r\n", DISPLAY_LARGE_DIM);
    PRINTF("DISPLAY_SMALL_DIM = %d\r\n", DISPLAY_SMALL_DIM);
    PRINTF("VIEW_WIDTH = %d\r\n", VIEW_WIDTH);
    PRINTF("VIEW_HEIGHT = %d\r\n", VIEW_HEIGHT);
    PRINTF("DETECTION_ZONE_RECT_TOP = %d\r\n", DETECTION_ZONE_RECT_TOP);
    PRINTF("DETECTION_ZONE_RECT_LEFT = %d\r\n", DETECTION_ZONE_RECT_LEFT);
    PRINTF("DETECTION_ZONE_RECT_WIDTH = %d\r\n", DETECTION_ZONE_RECT_WIDTH);
    PRINTF("DETECTION_ZONE_RECT_HEIGHT = %d\r\n", DETECTION_ZONE_RECT_HEIGHT);

    init_database(g_embedding_db);

    /* init API */
    static mpp_api_params_t api_param = {0};
#if ((defined APP_RC_CYCLE_INC) && (defined APP_RC_CYCLE_MIN))
    /* fine-tune RC cycle for stripe mode */
    api_param.rc_cycle_inc = APP_RC_CYCLE_INC;
    api_param.rc_cycle_min = APP_RC_CYCLE_MIN;
#endif

    /* fix max pipeline task priority. */
    api_param.pipeline_task_max_prio = APP_PIPELINE_TASK_MAX_PRIO;
    api_param.pipeline_rc_task_prio = APP_RC_TASK_MAX_PRIO;
    api_param.pipeline_pr_task_prio = APP_PR_TASK_MAX_PRIO;

    static mpp_stats_t api_stats;
    memset(&api_stats, 0, sizeof(api_stats));
    api_param.stats = &api_stats;

    ret = mpp_api_init(&api_param);
    if (ret)
        goto err;

    static mpp_t mp;
    static mpp_params_t mpp_params;
    static mpp_stats_t mp_stats;
    memset(&mpp_params, 0, sizeof(mpp_params));
    mpp_params.evt_callback_f = &mpp_event_listener;
    mpp_params.mask = MPP_EVENT_ALL;
    mpp_params.cb_userdata = &user_data;
    mpp_params.exec_flag = MPP_EXEC_RC;
    mpp_params.stats = &mp_stats;

    mp = mpp_create(&mpp_params, &ret);
    if (mp == MPP_INVALID)
        goto err;
    PRINTF("Debug: MPP created \r\n");
    user_data.mp = mp;

    static mpp_elem_handle_t infer_conv_h;
    static mpp_element_params_t infer_conv_params;
    static mpp_element_params_t antispoofing_params;
    static mpp_element_params_t mobilefacenet_params;
    static mpp_element_params_t scrfd_kps_params;
    static mpp_element_params_t label_rect_params;
    static mpp_stats_t img_quality_check_stats;
    static mpp_element_params_t img_quality_check_elem_params;
    static mpp_elem_handle_t img_quality_check_h;

    ret = setup_pipeline_elements(mp, args, stripe_mode, &user_data, &txt_info, &antispoofing_params, 
                                  &mobilefacenet_params, &scrfd_kps_params, &infer_conv_h, &infer_conv_params,
                                  &label_rect_params, &img_quality_check_stats, &img_quality_check_elem_params,
                                  &img_quality_check_h);
    if (ret)
        goto err;

    mpp_stats_enable(MPP_STATS_GRP_API);
    mpp_stats_enable(MPP_STATS_GRP_MPP);

    user_data.api_stats = &api_stats;
    user_data.mp_stats = &mp_stats;

    TickType_t xLastPrintTime = 0, tick = 0, notifyTime = 0;
    const TickType_t xFrequency = OUTPUT_PRINT_PERIOD_MS / portTICK_PERIOD_MS;
    const TickType_t notifyDelay = OUTPUT_NOTIFY_PERIOD_MS / portTICK_PERIOD_MS;
    uint32_t last_inf_frame_num = user_data.inference_frame_num;

    s_mpp_task_queue = xQueueCreate(MPP_CMD_QUEUE_SIZE, sizeof(msg_t *));
    if (s_mpp_task_queue == NULL) {
        PRINTF("Failed to create mpp receive queue\r\n");
        goto err;
    }
    mpp_task_set_queue(s_mpp_task_queue);

    // create registration timeout timer
    registration_timer = xTimerCreate(
            "RegTimer",
            pdMS_TO_TICKS(5000), // Default period, will be changed when needed
            pdFALSE, // One-shot timer
            NULL,
            registration_timeout_callback
    );

    if (registration_timer == NULL) {
        PRINTF("Failed to create registration timer\r\n");
        goto err;
    }

    for (;;) {
        /* Check for incoming messages from control task */
        msg_t *received_msg;

        /* Non-blocking receive */
        if (xQueueReceive(s_mpp_task_queue, &received_msg, pdMS_TO_TICKS(5)) == pdPASS)
        {
            /* Handle message received from control task */
            switch(received_msg->msgID)
            {
                case MPP_TASK_FACE_REG_REQ:
                {
                    ret = mpp_face_reg_request_handler(&user_data, received_msg, &infer_conv_params, infer_conv_h, &scrfd_kps_params);
                    if (ret)
                        goto err;
                    break;
                }

                case MPP_TASK_FR_ENABLE_LCD_ON:
                    ret = mpp_fr_enable_lcd_on_handler(&user_data, &infer_conv_params, infer_conv_h, &scrfd_kps_params,
                                                       &img_quality_check_elem_params, img_quality_check_h);
                    if (ret)
                        goto err;
                    break;

                case MPP_TASK_FR_DISABLE_LCD_ON:
                    ret = mpp_fr_disable_lcd_on_handler(&user_data);
                    if (ret)
                        goto err;
                    break;

                case MPP_TASK_FR_DISABLE_LCD_OFF:
                    ret = mpp_fr_disable_lcd_off_handler(&user_data);
                    if (ret)
                        goto err;
                    break;

                default:
                    PRINTF("Command %d not supported\r\n", received_msg->msgID);
                    break;
            }

            if (received_msg != NULL)
                vPortFree(received_msg);
        }

        tick = xTaskGetTickCount();

        /* check for timeout */
        if (user_data.registration_mode_on == 1 && registration_timer_expired)
        {
            registration_end_time = tick;

            /* stop branches with convert and inference elements */
            /* until the user is notified */
            mpp_stop(user_data.mp_bg);

            PRINTF("Registration timeout - forcing cancellation after %d ms\r\n",
                    (registration_end_time - registration_start_time) * portTICK_PERIOD_MS);

            // Stop and cleanup registration timer
            if (registration_timer != NULL) 
                xTimerStop(registration_timer, 0);
            registration_timer_expired = false;

            /* registration should be cleared */
            strcpy(user_data.registration_name, "\0");
            user_data.registration_mode_on = 0;

            mpp_task_notify(MPP_TASK_FR_DISABLE_LCD_ON, NULL);
            ctrl_task_notify(CTRL_TASK_FACE_REG_TIMEOUT, NULL);

            /* start timer */
            notifyTime = tick;
            user_data.state = STATE_NOTIFYING_USER;
        }

        /* manage periodic print */
        if ((tick > (xLastPrintTime + xFrequency)) && user_data_access(&user_data, 0))
        {
            if (last_inf_frame_num != user_data.inference_frame_num)
            {
                xLastPrintTime = tick;
                // Store separately the detection , antispoofing and recognition inference times
                if (user_data.last_model == MODEL_SCRFD_KPS)
                {
                    detection_inference_time = user_data.inference_time_ms;
                    if (user_data.state == STATE_DETECTING && user_data.detected_count == 0)
                    {
                        strcpy(txt_info.name, "");
                        txt_info.confidence = 0.0f;
                        txt_info.detection_inference_time = detection_inference_time;
                        txt_info.recognition_inference_time = 0;
                    #if USE_ANTISPOOFING
                        txt_info.antispoofing_inference_time = 0;
                        strcpy(txt_info.state, "");
                    #endif
                        draw_text_area(&txt_info);
                    }
                
                }
                else if (user_data.last_model == MODEL_MOBILEFACENET)
                {
                    recognition_inference_time = user_data.inference_time_ms;
                }
            #if USE_ANTISPOOFING
                else // MODEL_ANTISPOOFING
                {
                    antispoofing_inference_time = user_data.inference_time_ms;
                }
            #endif
            #ifdef PRINT_INFERENCE_RESULTS
                print_conditional(&user_data);
            #endif /* PRINT_INFERENCE_RESULTS */
                last_inf_frame_num = user_data.inference_frame_num;
            }
            user_data_release(&user_data);
        }

        if (user_data_access(&user_data, 0))
        {
            switch (user_data.state)
            {
            case STATE_DETECTING:
                /* Nothing to do, wait for detection results */
                break;

            case STATE_DETECTED:
            {
                if (handle_state_detected(&user_data, mp, user_data.mp_bg, infer_conv_h, 
                                          &infer_conv_params, &antispoofing_params, 
                                          &label_rect_params) != 0)
                {
                    goto err;
                }
                break;
            }

            case STATE_CHECKING_ANTISPOOFING:
                /* code */
                break;

            case STATE_SPOOF:
            {
                handle_state_spoof(&user_data, &txt_info, detection_inference_time, 
                                    antispoofing_inference_time, tick, &notifyTime);
                break;
            }

            case STATE_REAL:
            {
                if (handle_state_real(&user_data, mp, user_data.mp_bg, infer_conv_h,
                                      &infer_conv_params, &mobilefacenet_params,
                                      &label_rect_params,
                                      &img_quality_check_elem_params,
                                      img_quality_check_h) != 0)
                {
                    goto err;
                }
                break;
            }

            case STATE_RECOGNIZING:
                /* Nothing to do yet, wait for recognition results */
                break;

            case STATE_RECOGNIZED:
            {
                handle_state_recognized(&user_data, &txt_info, detection_inference_time,
                                        recognition_inference_time, antispoofing_inference_time,
                                        tick, &notifyTime);
                break;
            }

            case STATE_NOT_RECOGNIZED:
            {
                handle_state_not_recognized(&user_data, mp, &txt_info, detection_inference_time,
                                            recognition_inference_time, antispoofing_inference_time,
                                            &label_rect_params, tick, &notifyTime);
                break;
            }

            case STATE_NOTIFYING_USER:
            {
                handle_state_notifying_user(&user_data, tick, notifyTime, notifyDelay);
                break;
            }

            case STATE_USER_NOTIFIED:
            {
                if (handle_state_user_notified(&user_data, infer_conv_h, &infer_conv_params,
                                               &scrfd_kps_params,
                                               &img_quality_check_elem_params, img_quality_check_h) != 0)
                {
                    goto err;
                }
                break;
            }

            default:
                PRINTF("Unknown state: %d\r\n", user_data.state);
                goto err;

                break;
            }
            user_data_release(&user_data);
        }
    }

err:
    for (;;) {
        PRINTF("Error building application pipeline : ret %d\r\n", ret);
        vTaskSuspend(NULL);
    }
}