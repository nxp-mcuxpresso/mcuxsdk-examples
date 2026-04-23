/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stdio.h"

#include "FreeRTOS.h"
#include "atomic.h"

#include "fsl_common.h"
#include "fsl_debug_console.h"

#include "mpp_api.h"
#include "mpp_config.h"
#include "mpp_api_types_internal.h"

#include "face_box_utils.h"
#include "app_constants.h"
#include "app_types.h"

#include "mobilefacenet_output_postproc_quantized.h"
#include "scrfd_kps_output_postproc.h"
#include "antispoofing_output_postproc_quantized.h"

#include APP_TFLITE_MOBILEFACENET_INFO
#include APP_TFLITE_SCRFD_KPS_INFO
#include APP_TFLITE_ANTISPOOFING_INFO

extern int user_data_access(user_data_t *user_data, uint32_t timeout);
extern void user_data_release(user_data_t *user_data);
extern void disable_camera_ir_stream(user_data_t *user_data);

extern float face_reco_threshold;

static int g_detect_id = 0;
static int g_reco_id = 0;

int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data)
{
    status_t ret = 0;
    const mpp_inference_cb_param_t *inf_output;
    const img_quality_metrics_t *img_quality_metrics;
    recognition_result result;
    antispoofing_result liveness;
#ifdef DEBUG_PREVIEW_RECOGNITION
    int imgsize = 0;
    int model_width = 0;
    int model_height = 0;
    mpp_pixel_format_t model_format = MPP_PIXEL_RGB;
#endif

    /* user_data handle contains application private data */
    user_data_t *app_priv = (user_data_t *)user_data;

    switch(evt) {
    case MPP_EVENT_INFERENCE_INPUT_READY:
#ifdef DEBUG_PREVIEW_RECOGNITION
        /* copy inference input view data */
        if (app_priv->cur_model == MODEL_MOBILEFACENET)
        {
            model_height = MOBILEFACENET_HEIGHT;
            model_width = MOBILEFACENET_WIDTH;
            model_format = MPP_PIXEL_RGB;
        }
        else if (app_priv->cur_model == MODEL_ANTISPOOFING)
        {
            model_height = ANTISPOOFING_HEIGHT;
            model_width = ANTISPOOFING_WIDTH;
            model_format = MPP_PIXEL_GRAY;
        }
        else    /* SCRFD do not preview */
            break;

        /* store face image */
        imgsize = model_width * model_height * 3;  /* RGB888 */
        if (app_priv->inference_view)
        {
            memcpy(app_priv->inference_view, (uint8_t *)evt_data, imgsize);
        }
        /* show detected face preview */
        app_priv->p_params_compose->compose.image_list[COMPOSE_INFPVW_INDEX].width = model_width;
        app_priv->p_params_compose->compose.image_list[COMPOSE_INFPVW_INDEX].height = model_height;
        app_priv->p_params_compose->compose.image_list[COMPOSE_INFPVW_INDEX].format = model_format;
        app_priv->p_params_compose->compose.image_list[COMPOSE_INFPVW_INDEX].buffer = app_priv->inference_view;
        mpp_element_update(app_priv->mp, app_priv->compose_elem, app_priv->p_params_compose, true);
#endif
        break;

    case MPP_EVENT_INFERENCE_OUTPUT_READY:
        /* cast evt_data pointer to correct structure matching the event */
        inf_output = (const mpp_inference_cb_param_t *) evt_data;

        /* check that we can modify the user data (not accessed by other task) */
        if (user_data_access(app_priv, 0))
        {
            if (app_priv->cur_model == MODEL_MOBILEFACENET)
            {
                _elem_t *elem = (_elem_t *) app_priv->infer_elem;
                g_reco_id = elem->io.in_buf[0]->frame_id;
                MOBILEFACENET_ProcessOutput(
                        inf_output,
                        app_priv->db,
                        app_priv->db_max,
                        &result,
                        face_reco_threshold);

                /* copy recognition results */
                memcpy(&app_priv->result, &result, sizeof(result));
                app_priv->last_model = MODEL_MOBILEFACENET;
#ifdef DEBUG
                if (g_reco_id != g_detect_id)
                    PRINTF("Warning: Recognize frame id=%d, detected on frame id=%d\r\n", g_reco_id, g_detect_id);
#endif
                /* manage recognition vs registration */
                if (app_priv->result.similarity_percentage > 0 && app_priv->image_brightness_ok && app_priv->image_contrast_ok)
                {
                    PRINTF("*** Recognized %s ***\r\n", app_priv->result.recognized_name);

                    if (app_priv->state == STATE_RECOGNIZING)
                        app_priv->state = STATE_RECOGNIZED;
                }
                else
                {
                    /* *** Face not recognized! *** */
                    if (app_priv->state == STATE_RECOGNIZING)
                        app_priv->state = STATE_NOT_RECOGNIZED;
                }
            }
            else if (app_priv->cur_model == MODEL_ANTISPOOFING)
            {
                ANTISPOOFING_ProcessOutput(
                        inf_output,
                        &liveness);
                memcpy(&app_priv->liveness, &liveness, sizeof(liveness));

                app_priv->last_model = MODEL_ANTISPOOFING;

                /* Check whether the face is real or fake, if it's real proceed with recognition else stop the pipeline. */
                if ((app_priv->state == STATE_CHECKING_ANTISPOOFING) && (app_priv->liveness.result[1] > SPOOFING_THRESHOLD))
                    app_priv->state = STATE_REAL;
                else
                    app_priv->state = STATE_SPOOF;
            }
            else
            {
                _elem_t *elem = (_elem_t *) app_priv->infer_elem;
                g_detect_id = elem->io.in_buf[0]->frame_id;
                ret = SCRFDKPS_ProcessOutput(
                        inf_output,
                        app_priv->final_boxes,
                        NUM_BOXES_MAX,
                        true); // return only the face with the largest area

                app_priv->last_model = MODEL_SCRFD_KPS;

#ifdef DEBUG
                if (g_reco_id == g_detect_id)
                    PRINTF("Warning: Running detection on the same frame (%d) as last recognition\r\n", g_detect_id);
#endif

                if (ret != kStatus_Success)
                {
                    PRINTF("mpp_event_listener: SCRFD process output error!");
                }
                else
                {
                    /* count valid results */
                    app_priv->detected_count = 0;
                    for (uint32_t i = 0; i < NUM_BOXES_MAX; i++)
                    {
                        if (app_priv->final_boxes[i].score > 0)
                            app_priv->detected_count++;
                    }
                }

                /* update labeled rectangle */
                mpp_element_params_t params;
                memset(&params, 0, sizeof(params));
                /* detected_count contains at least the detection zone box */
                params.labels.detected_rect = app_priv->detected_count + 1;
                params.labels.max_rect = MAX_LABEL_RECTS;
                params.labels.rectangles = app_priv->labels;
                /* Allocate landmarks array if not already done */
                mpp_landmark_t landmarks[MAX_LABEL_RECTS * SCRFD_NUM_LANDMARKS];
                memset(landmarks, 0, sizeof(landmarks));
                params.labels.max_landmk = MAX_LABEL_RECTS * SCRFD_NUM_LANDMARKS;
                params.labels.detected_landmk = 0;
                params.labels.landmarks = app_priv->landmarks;
                bool face_ok = boxes_to_rects(app_priv->final_boxes, NUM_BOXES_MAX, MAX_LABEL_RECTS, params.labels.rectangles);

                if ( (app_priv->state == STATE_DETECTING) && (face_ok) )
                {
                    app_priv->state = STATE_DETECTED;
                    disable_camera_ir_stream(app_priv);
                    if (app_priv->mp_split != mpp)
                        mpp_stop(app_priv->mp_split);
                    else
                        PRINTF("mp_split and inference branches are the same.Skipe calling mpp_stop\r\n");
                }

                if ( (app_priv->labrect_elem != 0) && ( app_priv->mp != NULL ) )
                    mpp_element_update(app_priv->mp, app_priv->labrect_elem, &params, true);
            }

            app_priv->inference_time_ms = inf_output->inference_time_ms;
            app_priv->inference_frame_num++;

            /* end of modification of user data */
            user_data_release(app_priv);
        }
        else
        {
            /* In case there is an user data access conflict, force the update of the pipeline only when
             * current model is MOBILEFACENET or ANTISPOOFING to ensure the pipeline is ran again on the
             * same frame */
            if (app_priv->cur_model == MODEL_MOBILEFACENET || app_priv->cur_model == MODEL_ANTISPOOFING)
            {
                mpp_force_update(mpp);
            }
        }
        break;

    case MPP_EVENT_QUALITY_CHECK_READY:
        /* cast evt_data pointer to correct structure matching the event */
        img_quality_metrics = (const img_quality_metrics_t *) evt_data;
        if (img_quality_metrics == NULL)
            return 0;
        if (app_priv->cur_model == MODEL_MOBILEFACENET)
        {
             /* store quality check result for later use */
            if (user_data_access(app_priv, 0))
            {
                /* determine image quality based on brightness and contrast thresholds */
                app_priv->image_brightness_ok = (img_quality_metrics->brightness >= MIN_BRIGHTNESS_THRESHOLD && 
                                    img_quality_metrics->brightness <= MAX_BRIGHTNESS_THRESHOLD);
                app_priv->image_contrast_ok = (img_quality_metrics->contrast >= MIN_CONTRAST_THRESHOLD &&
                                    img_quality_metrics->contrast <= MAX_CONTRAST_THRESHOLD);

                user_data_release(app_priv);
            }
        }
		break;
    case MPP_EVENT_INVALID:
    default:
        /* nothing to do */
        break;
    }

    return 0;
}