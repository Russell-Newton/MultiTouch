#include "double.h"

#include "gestureparams.h"
#include "repeat_tap.h"

double_tap_t double_tap_d[MAX_TOUCHES];
void (*on_double_tap)(const double_tap_t*) = 0;

void init_double_tap(void) {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        double_tap_d[i].state = RECOGNIZER_STATE_NULL;
        double_tap_d[i].t     = 0;
        double_tap_d[i].x     = 0;
        double_tap_d[i].y     = 0;
    }
    on_double_tap = 0;
}

void recognize_double_tap(const touch_event_t* event) {
    (void)event;
    int repeat_tap_idx = get_updated_repeat_tap();
    if (repeat_tap_idx == -1) {
        return;
    }
    const repeat_tap_t* check = get_repeat_tap() + repeat_tap_idx;

    if (check->count % 2 == 0) {
        int state_changed = 0;
        if (double_tap_d[repeat_tap_idx].state != check->state) {
            double_tap_d[repeat_tap_idx].state = check->state;
            state_changed                      = 1;
        }

        double_tap_d[repeat_tap_idx].x = check->x;
        double_tap_d[repeat_tap_idx].y = check->y;
        double_tap_d[repeat_tap_idx].t = check->t;

        if (state_changed && on_double_tap) {
            on_double_tap(double_tap_d + repeat_tap_idx);
        }
    }
}

const double_tap_t* get_double_tap(void) {
    return double_tap_d;
}

int set_on_double_tap(void (*listener)(const double_tap_t*)) {
    if (on_double_tap) {
        on_double_tap = listener;
        return 0;
    } else {
        on_double_tap = listener;
        return 1;
    }
}
