#include "tap.h"

#include "gestureparams.h"
#include "repeat_tap.h"

tap_t tap_d[MAX_TOUCHES];
void (*on_tap)(const tap_t*) = 0;

void init_tap(void) {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        tap_d[i].state = RECOGNIZER_STATE_NULL;
        tap_d[i].t     = 0;
        tap_d[i].x     = 0;
        tap_d[i].y     = 0;
    }
    on_tap = 0;
}

void recognize_tap(const touch_event_t* event) {
    (void) event;
    int repeat_tap_idx = get_updated_repeat_tap();
    if (repeat_tap_idx == -1) {
        return;
    }
    const repeat_tap_t* check = get_repeat_tap() + repeat_tap_idx;

    tap_d[repeat_tap_idx].state = check->state;
    tap_d[repeat_tap_idx].x     = check->x;
    tap_d[repeat_tap_idx].y     = check->y;
    tap_d[repeat_tap_idx].t     = check->t;

    if (on_tap) {
        on_tap(tap_d + repeat_tap_idx);
    }
}

const tap_t* get_tap(void) {
    return tap_d;
}

int set_on_tap(void (*listener)(const tap_t*)) {
    if (on_tap) {
        on_tap = listener;
        return 0;
    } else {
        on_tap = listener;
        return 1;
    }
}
