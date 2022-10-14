#pragma once

/// @brief
typedef enum state {
    RECOGNIZER_STATE_START,
    RECOGNIZER_STATE_POSSIBLE,
    RECOGNIZER_STATE_IN_PROGRESS,
    RECOGNIZER_STATE_FAILED,
    RECOGNIZER_STATE_COMPLETED
    // maybe more?
} state_t;
