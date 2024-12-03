/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app_constants.h"
#include "models/utils.h"

/* returns true if 'rect' is inside recognition zone */
static bool check_face_inside(mpp_labeled_rect_t *rect)
{
    bool ret = false;
    if ( (rect->left > RECO_ZONE_RECT_LEFT) && (rect->right < RECO_ZONE_RECT_RIGHT)
            && (rect->top > RECO_ZONE_RECT_TOP) && (rect->bottom < RECO_ZONE_RECT_BOTTOM) )
        ret = true;
    else
        ret = false;
    return ret;
}

/* returns true if 'rect' center is close enough from recognition zone center */
static bool check_face_center(mpp_labeled_rect_t *rect)
{
    bool ret = false;
    /* zone center will not change */
    static const int zone_center_x = (RECO_ZONE_RECT_LEFT + RECO_ZONE_RECT_RIGHT) / 2;
    static const int zone_center_y = (RECO_ZONE_RECT_BOTTOM + RECO_ZONE_RECT_TOP) / 2;
    int rect_center_x = (rect->left + rect->right) / 2;
    int rect_center_y = (rect->bottom + rect->top) / 2;

    if ( (ABS(rect_center_x - zone_center_x) < MAX_CENTER_DIST)
        && (ABS(rect_center_y - zone_center_y) < MAX_CENTER_DIST) )
        ret = true;
    else
        ret = false;
    return ret;
}

/* returns 'true' if 'rect' area matches recognition zone area
 * else return 'false' and sets boolean 'far':
 * 'true' means too far, 'false' means too close */
static bool check_face_size(mpp_labeled_rect_t *rect, bool *is_far)
{
    bool ret = false;
    if ( ((rect->bottom - rect->top) * (rect->right - rect->left)) < MIN_FACE_AREA )
    {
        ret = false;
        *is_far = true;
    }
    else if ( ((rect->bottom - rect->top) * (rect->right - rect->left)) > MAX_FACE_AREA )
    {
        ret = false;
        *is_far = false;
    }
    else
        ret = true;
    return ret;
}

/* Translate boxes into labeled rectangles using display characteristics
 * check box position to set label (far/close/outside/ok)
 * returns true when face's box is matching size & position criteria */
bool boxes_to_rects(box_data final_boxes[], uint32_t num_boxes, uint32_t max_boxes, mpp_labeled_rect_t *rects)
{
    uint32_t box_counter = 1;
    int width = ULTRAFACE_WIDTH;
    int height = ULTRAFACE_HEIGHT;
    char *label = FACE_LABEL_CENTER;
    bool is_far = false;
    bool ret = false;

    /* other rectangles show detected objects */
    for (uint32_t i = 0; i < num_boxes && box_counter < max_boxes; i++)
    {
        if (final_boxes[i].area == 0)
            continue;
        /* input tensor preview is scaled and moved to fit on screen, and so its bounding boxes */
        rects[box_counter].left = (int)((final_boxes[i].left * DETECTION_ZONE_RECT_WIDTH)/ width) + BOXES_OFFSET_LEFT;
        rects[box_counter].right = (int)((final_boxes[i].right * DETECTION_ZONE_RECT_WIDTH)/ width) + BOXES_OFFSET_LEFT;
        rects[box_counter].bottom = (int)((final_boxes[i].bottom * DETECTION_ZONE_RECT_HEIGHT)/height) + BOXES_OFFSET_TOP;
        rects[box_counter].top = (int)((final_boxes[i].top * DETECTION_ZONE_RECT_HEIGHT)/height) + BOXES_OFFSET_TOP;
        rects[box_counter].line_width = RECT_LINE_WIDTH;
        rects[box_counter].line_color.rgb.B = 0xff;

        /* check face position to set indication in label */
        if (check_face_inside(&rects[box_counter]))
        {
            if (check_face_center(&rects[box_counter]))
            {
                if (check_face_size(&rects[box_counter], &is_far))
                {
                    label = FACE_LABEL_OK;
                    ret = true;
                }
                else if (is_far)
                    label = FACE_LABEL_FAR;
                else
                    label = FACE_LABEL_CLOSE;
            }
            else
                label = FACE_LABEL_CENTER;
        }
        else
            label = FACE_LABEL_OUTSIDE;

        strncpy((char *)rects[box_counter].label, label, sizeof(rects[box_counter].label));
        box_counter++;
    }
    return ret;
}
