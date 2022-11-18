#pragma once

/// @brief This represents the state of individual gesture recognizers.
typedef enum state {
    // Every recognizer is initialized to the null state.
    RECOGNIZER_STATE_NULL,
    // Some recognizers transition to the possible state before formally completing or becoming in-progress.
    RECOGNIZER_STATE_POSSIBLE,
    // Recognizers that have meaningful data while in-progress report this state.
    RECOGNIZER_STATE_IN_PROGRESS,
    // Recognizers that receive touch events inconsistent with their gesture will fail.
    RECOGNIZER_STATE_FAILED,
    // Recognizers complete once all touch events associated with a complete gesture are processed.
    RECOGNIZER_STATE_COMPLETED
} state_t;
