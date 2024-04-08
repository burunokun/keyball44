#ifndef MODS_H
#define MODS_H

#include <stdbool.h>
#include <stdint.h>

#include QMK_KEYBOARD_H
#include "quantum.h"

enum custom_keycodes {
    KC_SCR = SAFE_RANGE,
};

enum {
    TD_ALT_TAB,
};


enum click_state {
    NONE,
    SCROLLING
};

enum click_state state;

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;



const uint16_t ignore_disable_mouse_layer_keys[] = {
    KC_LGUI,
    KC_LCTL,
    KC_LALT,
    KC_LSFT,
};

int16_t scroll_v_mouse_interval_counter;
int16_t scroll_h_mouse_interval_counter;

int16_t scroll_v_threshold = 50;
int16_t scroll_h_threshold = 50;

// void pointing_device_init_user(void) {
//     set_auto_mouse_enable(true);
// };


int16_t my_abs(int16_t num) {
    return num < 0 ? -num : num;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    int16_t current_x = mouse_report.x;
    int16_t current_y = mouse_report.y;
    int16_t current_h = 0;
    int16_t current_v = 0;

    switch (state) {
        case SCROLLING: {
            int8_t rep_v = 0;
            int8_t rep_h = 0;

            if (my_abs(current_y) * 2 > my_abs(current_x)) {
                scroll_v_mouse_interval_counter += current_y;
                while (my_abs(scroll_v_mouse_interval_counter) > scroll_v_threshold) {
                    if (scroll_v_mouse_interval_counter < 0) {
                        scroll_v_mouse_interval_counter += scroll_v_threshold;
                        rep_v -= scroll_v_threshold;
                    } else {
                        scroll_v_mouse_interval_counter -= scroll_v_threshold;
                        rep_v += scroll_v_threshold;
                    }
                }
            } else {
                scroll_h_mouse_interval_counter += current_x;
                while (my_abs(scroll_h_mouse_interval_counter) > scroll_h_threshold) {
                    if (scroll_h_mouse_interval_counter < 0) {
                        scroll_h_mouse_interval_counter += scroll_h_threshold;
                        rep_h -= scroll_h_threshold;
                    } else {
                        scroll_h_mouse_interval_counter -= scroll_h_threshold;
                        rep_h += scroll_h_threshold;
                    }
                }
            }

            current_h = rep_h / scroll_h_threshold;
            current_v = -rep_v / scroll_v_threshold;
            current_x = 0;
            current_y = 0;
        } break;

        case NONE: {} break;

        default: {
            state = NONE;
        } break;
    }

    mouse_report.x = current_x;
    mouse_report.y = current_y;
    mouse_report.h = current_h;
    mouse_report.v = current_v;
    return mouse_report;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;
    switch (keycode) {
        case KC_SCR: {
             if (record->event.pressed) {
                 state = SCROLLING;
             } else {
                 state = NONE;
             }
         } return false;

        case TD(TD_ALT_TAB):
            action = &tap_dance_actions[TD_INDEX(TD_ALT_TAB)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            } return true;

        default: {
            size_t len = sizeof(ignore_disable_mouse_layer_keys) / sizeof(ignore_disable_mouse_layer_keys[0]);
            for (size_t i = 0; i < len; ++i) {
                if (keycode == ignore_disable_mouse_layer_keys[i]) {
                    return true;
                }
            }
            state = NONE;
        } break;
    }

    return true;
}

const key_override_t esc  = ko_make_basic(MOD_MASK_ALT,   KC_SCLN, KC_ESC);

const key_override_t vmut = ko_make_basic(MOD_MASK_ALT,   KC_VOLU, KC_MUTE);
const key_override_t ctab = ko_make_basic(MOD_MASK_ALT,   KC_TAB, A(C(KC_TAB)));

const key_override_t pgup = ko_make_basic(MOD_MASK_CTRL,  KC_PGDN, C(KC_PGUP));
const key_override_t pgdn = ko_make_basic(MOD_MASK_CTRL,  KC_PGUP, C(KC_PGDN));

const key_override_t vldn = ko_make_basic(MOD_MASK_SHIFT, KC_VOLU, KC_VOLD);
const key_override_t brdn = ko_make_basic(MOD_MASK_SHIFT, KC_BRIU, KC_BRID);

// const key_override_t kc1 = ko_make_basic(MOD_MASK_SHIFT, KC_LPRN, KC_F1);
// const key_override_t kc2 = ko_make_basic(MOD_MASK_SHIFT, KC_ASTR, KC_F2);
// const key_override_t kc3 = ko_make_basic(MOD_MASK_SHIFT, KC_AT  , KC_F3);
// const key_override_t kc4 = ko_make_basic(MOD_MASK_SHIFT, KC_LCBR, KC_F4);
// const key_override_t kc5 = ko_make_basic(MOD_MASK_SHIFT, KC_HASH, KC_F5);
// const key_override_t kc6 = ko_make_basic(MOD_MASK_SHIFT, KC_AMPR, KC_F6);
// const key_override_t kc7 = ko_make_basic(MOD_MASK_SHIFT, KC_LBRC, KC_F7);
// const key_override_t kc8 = ko_make_basic(MOD_MASK_SHIFT, KC_CIRC, KC_F8);
// const key_override_t kc9 = ko_make_basic(MOD_MASK_SHIFT, KC_PERC, KC_F9);
// const key_override_t kc10 = ko_make_basic(MOD_MASK_SHIFT, KC_PLUS, KC_F10);
// const key_override_t kc11 = ko_make_basic(MOD_MASK_SHIFT, KC_DLR , KC_F11);
// const key_override_t kc12 = ko_make_basic(MOD_MASK_SHIFT, KC_EXLM, KC_F12);

const key_override_t **key_overrides = (const key_override_t *[]) {
    &esc, &brdn, &vldn, &vmut, &ctab, &pgup, &pgdn,
    // &kc1, &kc2, &kc3,
    // &kc4, &kc5, &kc6,
    // &kc7, &kc8,&kc9,
    // &kc10, &kc11, &kc12,
    NULL
};

/*
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_SPC:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}
*/

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

tap_dance_action_t tap_dance_actions[] = {
    [TD_ALT_TAB] = ACTION_TAP_DANCE_TAP_HOLD(KC_TAB, A(C(KC_TAB))),
};

#endif // MODS_H
