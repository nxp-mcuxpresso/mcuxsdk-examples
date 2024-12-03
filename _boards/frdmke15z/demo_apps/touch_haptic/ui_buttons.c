/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ui_buttons.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

typedef enum _ui_buttons_lock_state_en
{
    UI_BUTTONS_LOCK_STATE_UNLOCKED       = 0u,
    UI_BUTTONS_LOCK_STATE_LOCK_TRIGGER   = 1u,
    UI_BUTTONS_LOCK_STATE_LOCKED         = 2u,
    UI_BUTTONS_LOCK_STATE_UNLOCK_TRIGGER = 3u,
} ui_buttons_lock_state_en;

/*******************************************************************************
 * Variables
 ******************************************************************************/
ui_button El1 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 1,
    .countPressMax            = 100,
    .autorepeatTouchVal       = 10,
};
ui_button El2 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 2,
    .countPressMax            = 100,
    .autorepeatTouchVal       = 10,
};
ui_button El3 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 3,
    .countPressMax            = 100,
};
ui_button El4 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 4,
    .countPressMax            = 100,
};
ui_button El5 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 5,
    .countPressMax            = 100,
};
ui_button El6 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 6,
    .countPressMax            = 100,
};
ui_button El7 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 7,
    .countPressMax            = 100,
};
ui_button El8 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 8,
    .countPressMax            = 100,
};
ui_button El9 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 9,
    .countPressMax            = 100,
};
ui_button El10 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 10,
    .countPressMax            = 100,
};
ui_button El11 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 11,
    .countPressMax            = 100,
};
ui_button El12 =
{
    {
    .delay                    = { 0, 0 },
    .gain                     = 32767,
    },
    .type                     = UI_BUTTONS_TYPE_NORMAL,
    .label                    = 12,
    .countPressMax            = 100,
};

ui_button* buttonDefinition[12] = {&El1, &El2, &El3, &El4, &El5, &El6, &El7, &El8, &El9,
    &El10, &El11, &El12};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Private functions
 ******************************************************************************/

/*******************************************************************************
 * Public functions
 ******************************************************************************/

uint16_t ui_buttons_init( ui_button*** pButtons )
{
    uint16_t nButtons = sizeof( buttonDefinition ) / sizeof( *buttonDefinition );
    *pButtons = buttonDefinition;

    for( uint16_t c1 = 0; c1 < nButtons; c1++ )
    {
        buttonDefinition[c1]->countPress = 0;
        buttonDefinition[c1]->flagCountReached = 0;
    }

    return nButtons;
}
