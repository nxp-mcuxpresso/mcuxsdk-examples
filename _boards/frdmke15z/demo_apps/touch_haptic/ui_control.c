/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ui_buttons.h"
#include "haptic.h"
#include "haptic_pwm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUTTON_ID_HAPTICS_ON_OFF (  0 )
#define BUTTON_ID_AUDIO_ON_OFF   (  1 )
#define BUTTON_ID_CLEAR          (  2 )
#define BUTTON_ID_DOWN           (  9 )
#define BUTTON_ID_UP             ( 11 )

/*******************************************************************************
 * Types
 ******************************************************************************/
typedef enum _ui_control_audio_en
{
    UI_CONTROL_AUDIO_OFF  = 0u,
    UI_CONTROL_AUDIO_ON   = 1u,
    UI_CONTROL_AUDIO_BEEP = 2u,
} ui_control_audio_en;

typedef enum _ui_control_haptic_en
{
    UI_CONTROL_HAPTIC_OFF  = 0u,
    UI_CONTROL_HAPTIC_ON   = 1u,
} ui_control_haptic_en;

typedef struct _ui_control_state_t
{
    int16_t limitFlag;
    uint16_t nButtons;
    ui_control_audio_en audioMode;
    ui_control_haptic_en hapticMode;
    uint16_t promptState;
    ui_button** buttons;
} ui_control_state_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static ui_control_state_t uiState = { 0 };
static ui_control_state_t* pS = &uiState;

uint16_t autorepeatTouchCounter = 0;    /*!< Autorepeat press counter */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Private functions
 ******************************************************************************/
void ui_haptic_trigger( int32_t triggerId, haptic_effect_t* pEffect, int32_t doubleClickDelay )
{
    /* Function to send the command for a haptic click */
    /* Click sound is independent of button, but delay and gain is specific to button */

    /* BEEP mode overwrite */
    if( pS->audioMode == UI_CONTROL_AUDIO_BEEP )
    {
        switch( triggerId )
        {
            case HAPTIC_TRIGGER_PRESS:
                triggerId = HAPTIC_TRIGGER_BEEP;
                break;

            case HAPTIC_TRIGGER_RELEASE:
                triggerId = HAPTIC_TRIGGER_LAST;
                break;
            
            case HAPTIC_TRIGGER_AUTOREPEAT:
                triggerId = HAPTIC_TRIGGER_AUTOREPEAT;
                break;                    

            default:
                triggerId = HAPTIC_TRIGGER_LAST;
                break;
        }
    }
    
    if( doubleClickDelay > 0 )
    {
        haptic_trigger_double_click((haptic_trigger_en)triggerId, pEffect, doubleClickDelay );
    }
    else
    {
        haptic_trigger_click((haptic_trigger_en)triggerId, pEffect );
    }
}

void ui_haptic_on_off( void )
{
    /* Toggle haptics off/on */
    pS->hapticMode =(ui_control_haptic_en)!pS->hapticMode ;
    haptic_enable_haptic( pS->hapticMode );
    switch( pS->hapticMode )
    {
        case UI_CONTROL_HAPTIC_OFF:
            break;

        case UI_CONTROL_HAPTIC_ON:
            break;

        default:
            break;
    }
}

void ui_audio_on_off( void )
{
    /* Toggle audio off/on/beep */
    pS->audioMode = (ui_control_audio_en)((pS->audioMode + 1 ) % 3);
    haptic_enable_audio( pS->audioMode );
    switch( pS->audioMode )
    {
        case UI_CONTROL_AUDIO_OFF:
            break;

        case UI_CONTROL_AUDIO_ON:
            break;

        case UI_CONTROL_AUDIO_BEEP:
            break;
        default:
            break;
    }
}

void ui_keypad_touch( uint32_t index )
{
    /* Specific actions for press */
    switch( index )
    {
        default:
            ui_haptic_trigger( HAPTIC_TRIGGER_PRESS, &pS->buttons[index]->hapticEffect, 0 );
            break;
    }
}

void ui_keypad_release( uint32_t index )
{
    /* Reset delay counter */
    autorepeatTouchCounter = 0;
}

void ui_keypad_autorepeat( uint32_t index )
{
    if(autorepeatTouchCounter++ > pS->buttons[index]->autorepeatTouchVal)
    {
        autorepeatTouchCounter = 0;
        
        switch (index)
        {
        case BUTTON_ID_HAPTICS_ON_OFF:
            ui_haptic_on_off();
            switch (pS->hapticMode)
            {
                case UI_CONTROL_HAPTIC_ON:
                    haptic_trigger_double_click( HAPTIC_TRIGGER_PRESS,  &pS->buttons[index]->hapticEffect, 2000);
                    break;
                    
                default:
                    break;
            }
            break;
            
        case BUTTON_ID_AUDIO_ON_OFF:
            ui_audio_on_off();
            switch (pS->audioMode)
            {
                case UI_CONTROL_AUDIO_ON:
                    haptic_trigger_double_click( HAPTIC_TRIGGER_PRESS,  &pS->buttons[index]->hapticEffect, 2000);
                    break;
                    
                case UI_CONTROL_AUDIO_BEEP:
                    haptic_trigger_double_click( HAPTIC_TRIGGER_BEEP,  &pS->buttons[index]->hapticEffect, 2500);
                    break;
                    
                default:
                    break;
            }
            
        default:
            break;
        }
        
    }
    else
    {
        switch (index) 
        {
            case BUTTON_ID_HAPTICS_ON_OFF:
                break;
            
            case BUTTON_ID_AUDIO_ON_OFF:
                break;
                
            default:
                break;
        }
    }
}

void ui_keypad_event( ui_buttons_keypad_event_en event, uint32_t index )
{
    /* Specific key actions */
    switch (event)
    {
        case UI_BUTTONS_KEYPAD_TOUCH:
            ui_keypad_touch( index );
            break;

        case UI_BUTTONS_KEYPAD_RELEASE:
            ui_keypad_release( index );
            break;

        case UI_BUTTONS_KEYPAD_AUTOREPEAT:
            ui_keypad_autorepeat( index );
            break;

        default:
            break;
    }
}

void ui_slider_touch( uint32_t index )
{
    /* Specific actions for press */
    switch( index )
    {
        default:
            ui_haptic_trigger( HAPTIC_TRIGGER_PRESS, &pS->buttons[index]->hapticEffect, 0 );
            break;
    }
}

void ui_slider_release( uint32_t index )
{
    /* No release feedback in BEEP mode */
    if( pS->audioMode != UI_CONTROL_AUDIO_BEEP )
    {
        ui_haptic_trigger( HAPTIC_TRIGGER_RELEASE, &pS->buttons[index]->hapticEffect, 0 );
    }
}

void ui_slider_event( ui_buttons_slider_event_en event, uint32_t index )
{
    /* Specific key actions */
    switch (event)
    {
        case UI_BUTTONS_SLIDER_TOUCH:
            ui_slider_touch( index );
            break;

        case UI_BUTTONS_SLIDER_RELEASE:
            break;

        default:
            break;
    }
}

void ui_rotary_touch( uint32_t index )
{
    /* Specific actions for press */
    switch( index )
    {
        default:
            ui_haptic_trigger( HAPTIC_TRIGGER_PRESS, &pS->buttons[index]->hapticEffect, 0 );
            break;
    }
}

void ui_rotary_release( uint32_t index )
{
    /* No release feedback in BEEP mode */
    if( pS->audioMode != UI_CONTROL_AUDIO_BEEP )
    {
        ui_haptic_trigger( HAPTIC_TRIGGER_RELEASE, &pS->buttons[index]->hapticEffect, 0 );
    }
}

void ui_rotary_event( ui_buttons_rotary_event_en event, uint32_t index )
{
    /* Specific key actions */
    switch (event)
    {
        case UI_BUTTONS_ROTARY_TOUCH:
            ui_rotary_touch( index );
            break;

        case UI_BUTTONS_ROTARY_RELEASE:
            break;

        default:
            break;
    }
}

/*******************************************************************************
 * Public functions
 ******************************************************************************/

void ui_control_keypad_event( ui_buttons_keypad_event_en inputEvent, uint32_t index, uint16_t nScans )
{
    ui_keypad_event( inputEvent, index );
}

void ui_control_slider_event( ui_buttons_slider_event_en event, uint32_t position, uint16_t nScans )
{
    /* Calculate haptic effect gain from slider position
       Slider's range is 160 but actual position returned by callback is in range between 40 to 160
       Newly calculated linear gain for specified button is in range between 0 to 32760 (default setting is 32767)*/
    pS->buttons[7]->hapticEffect.gain = 273 * position - 10920;
    
    ui_slider_event(event, 7);
}

void ui_control_rotary_event( ui_buttons_rotary_event_en event, uint32_t position, uint16_t nScans )
{
    ui_rotary_event(event, 0);
}

void ui_control_init( void )
{
    /* Init UI */
    pS->nButtons = ui_buttons_init( &pS->buttons );
    pS->audioMode  = UI_CONTROL_AUDIO_ON;
    pS->hapticMode = UI_CONTROL_HAPTIC_ON;

    /* Init haptics */
    haptic_init();
    /* Play startup tune */
    haptic_trigger_click( HAPTIC_TRIGGER_INTRO, &pS->buttons[0]->hapticEffect );
}

void ui_control_main( void )
{
    /* Haptic process */
    haptic_main();
}
