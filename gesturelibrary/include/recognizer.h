#pragma once

/// @brief
typedef enum state {
    RECOGNIZER_STATE_NULL,  // used to be start
    RECOGNIZER_STATE_POSSIBLE,
    RECOGNIZER_STATE_IN_PROGRESS,
    RECOGNIZER_STATE_FAILED,
    RECOGNIZER_STATE_COMPLETED
    // maybe more?
} state_t;
