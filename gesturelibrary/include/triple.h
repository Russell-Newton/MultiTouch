#pragma once   

#include "gesturelib.h"
#include "recognizer.h"

/**
 * @brief 
 * 
 */
typedef struct tap_data {
    state_t state;
    uint32_t last_x1;
    uint32_t last_y1;
    uint32_t last_x2;
    uint32_t last_y2;
    uint32_t last_x3;
    uint32_t last_y3;
    uint32_t last_time;
} tap_data_t;

/**
 * @brief 
 * 
 */
void recognize_triple_tap(touch_event_t);

/**
 * @brief 
 * 
 */
void recognize_triple_swipe(touch_event_t* event);

/**
 * @brief 
 * 
 */
void recognize_triple_hold(touch_event_t* event);

/**
 * @brief 
 * 
 */
void recognize_triple_drag(touch_event_t* event);

/**
 * @brief 
 * 
 */
void recognize_triple_rotate(touch_event_t* event);