/*
 * Copyright 2024 NXP
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

#include "face_box_utils.h"
#include "app_constants.h"
#include "app_types.h"

#include "models/mobilefacenet/mobilefacenet_output_postproc_quantized.h"
#include "models/ultraface_slim_quant_int8/ultraface_output_postproc.h"

#include APP_DATABASE_NAME

int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data)
{
    status_t ret = 0;
    const mpp_inference_cb_param_t *inf_output;
    recognition_result result;
    static int s_user_count = DATABASE_NUM_PEOPLE;    /* number of people in database */

    /* user_data handle contains application private data */
    user_data_t *app_priv = (user_data_t *)user_data;

    switch(evt) {
    case MPP_EVENT_INFERENCE_OUTPUT_READY:
        /* cast evt_data pointer to correct structure matching the event */
        inf_output = (const mpp_inference_cb_param_t *) evt_data;

        /* check that we can modify the user data (not accessed by other task) */
        if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS) {

            if (app_priv->cur_model == MODEL_MOBILEFACENET)
            {
                MOBILEFACENET_ProcessOutput(
                        inf_output,
                        Embedding_database,
                        DATABASE_MAX_PEOPLE,
                        &result);

                /* copy recognition results */
                memcpy(&app_priv->result, &result, sizeof(result));
                app_priv->last_model = MODEL_MOBILEFACENET;
            }
            else
            {
                ret = ULTRAFACE_ProcessOutput(
                        inf_output,
                        app_priv->final_boxes,
                        NUM_BOXES_MAX);

                app_priv->last_model = MODEL_ULTRAFACE;
                /* count valid results */
                app_priv->detected_count = 0;
                for (uint32_t i = 0; i < NUM_BOXES_MAX; i++)
                {
                    if (app_priv->final_boxes[i].score > 0)
                        app_priv->detected_count++;
                }
            }
            if (ret != kStatus_Success)
                PRINTF("mpp_event_listener: process output error!");

            app_priv->inference_time_ms = inf_output->inference_time_ms;

            /* end of modification of user data */
            __atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
        }

        /* analyze model results */
        if (app_priv->cur_model == MODEL_MOBILEFACENET)
        {
            // manage recognition vs registration
            if (app_priv->result.similarity_percentage > 0)
            {
                strcpy((char *)app_priv->labels[0].label, ZONE_LABEL_RECOGNIZED);
                strcat((char *)app_priv->labels[0].label, app_priv->result.recognized_name);
                PRINTF("*** Recognized %s ***\r\n", app_priv->result.recognized_name);
                /* TODO protect with atomic */
                if (app_priv->state == STATE_RECOGNIZING)
                    app_priv->state = STATE_RECOGNIZED;
            }
            else
            {
                if (s_user_count >= DATABASE_MAX_PEOPLE)
                {
                    PRINTF("Face database is full!\r\n");
                }
                else
                {
                    /* register user with arbitrary name based on index */
                    char name[32];
                    sprintf(name, "User#%d", s_user_count);
                    /* put user greetings in label */
                    strcpy((char *)app_priv->labels[0].label, ZONE_LABEL_REGISTERED);
                    strcat((char *)app_priv->labels[0].label, name);
                    /* store in database */
                    strncpy(Embedding_database[s_user_count].name, name, MAX_NAME_SIZE);
                    memcpy(Embedding_database[s_user_count].embedding, result.embedding, sizeof(result.embedding));
                    PRINTF("Registered %s\r\n", name);
                    s_user_count++;
                }
                /* TODO protect with atomic */
                if (app_priv->state == STATE_RECOGNIZING)
                    app_priv->state = STATE_RECOGNIZED; // TODO switch to STATE_REGISTERING to let user input its name
            }
        }

        /* update labeled rectangle */
        mpp_element_params_t params;
        /* detected_count contains at least the detection zone box */
        params.labels.detected_count = app_priv->detected_count + 1;
        params.labels.max_count = MAX_LABEL_RECTS;
        params.labels.rectangles = app_priv->labels;
        bool face_ok = boxes_to_rects(app_priv->final_boxes, NUM_BOXES_MAX, MAX_LABEL_RECTS, params.labels.rectangles);
        /* TODO protect with atomic */
        if ( (face_ok) && (app_priv->state == STATE_DETECTING) )
            app_priv->state = STATE_DETECTED;

        if ( (app_priv->labrect_elem != 0) && ( app_priv->mp != NULL ) )
        {
            mpp_element_update(app_priv->mp, app_priv->labrect_elem, &params);
        }

        app_priv->inference_frame_num++;
        break;
    case MPP_EVENT_INVALID:
    default:
        /* nothing to do */
        break;
    }

    return 0;
}
