#ifndef MODS_H
#define MODS_H

#include <stdbool.h>
#include <stdint.h>

#include QMK_KEYBOARD_H
#include "quantum.h"

enum custom_keycodes {
    KC_SCR = SAFE_RANGE,
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

void pointing_device_init_user(void) {
    set_auto_mouse_enable(true);
};

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
    switch (keycode) {
        case KC_SCR: {
             if (record->event.pressed) {
                 state = SCROLLING;
             } else {
                 state = NONE;
             }
         } return false;
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

// Escape
// const key_override_t aesc = ko_make_basic(MOD_MASK_ALT,   KC_SCLN, KC_ESC);

// Volume
const key_override_t vmut = ko_make_basic(MOD_MASK_ALT,   KC_VOLU, KC_MUTE);
const key_override_t vldn = ko_make_basic(MOD_MASK_SHIFT, KC_VOLU, KC_VOLD);
const key_override_t brdn = ko_make_basic(MOD_MASK_SHIFT, KC_BRIU, KC_BRID);

// Make Tab Switching more intuitive
const key_override_t pgup = ko_make_basic(MOD_MASK_CTRL,  KC_PGDN, C(KC_PGUP));
const key_override_t pgdn = ko_make_basic(MOD_MASK_CTRL,  KC_PGUP, C(KC_PGDN));

const key_override_t **key_overrides = (const key_override_t *[]) {
    // &aesc,
    &brdn, &vldn, &vmut,
    &pgup, &pgdn,
    NULL
};

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

#endif // MODS_H
