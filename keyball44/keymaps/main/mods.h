#ifndef MODS_H
#define MODS_H

#include <stdbool.h>
#include <stdint.h>

#include QMK_KEYBOARD_H
#include "quantum.h"

// void pointing_device_init_user(void) {
//     set_auto_mouse_enable(true);
// };

enum custom_keycodes {
    KC_SCR = SAFE_RANGE,
};


enum click_state {
    NONE,
    SCROLLING
};

enum click_state state;

int16_t my_abs(int16_t num) {
    return num < 0 ? -num : num;
}

const uint16_t ignore_disable_mouse_layer_keys[] = {
    KC_LGUI,
    KC_LCTL,
    KC_LALT,
    KC_LSFT,
    KC_RGUI,
    KC_RCTL,
    KC_RALT,
    KC_RSFT,
};

int16_t scroll_v_mouse_interval_counter;
int16_t scroll_h_mouse_interval_counter;

int16_t scroll_v_threshold = 50;
int16_t scroll_h_threshold = 50;

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
         } return false; // break;

        default: {
            for (int i = 0; i < sizeof(ignore_disable_mouse_layer_keys) / sizeof(ignore_disable_mouse_layer_keys[0]); ++i) {
                if (keycode == ignore_disable_mouse_layer_keys[i]) {
                    return true;
                }
            }
            state = NONE;
        } break;
    }

    return true;
}

const key_override_t aesc = ko_make_basic(MOD_MASK_ALT, KC_SCLN, KC_ESC);
const key_override_t vmut = ko_make_basic(MOD_MASK_ALT, KC_VOLU, KC_MUTE);
const key_override_t btn1 = ko_make_basic(MOD_MASK_GUI, KC_M, KC_BTN1);
const key_override_t btn2 = ko_make_basic(MOD_MASK_GUI, KC_DOT, KC_BTN2);
const key_override_t lang = ko_make_basic(MOD_MASK_SHIFT, KC_INT2, KC_INT4);
const key_override_t vldn = ko_make_basic(MOD_MASK_SHIFT, KC_VOLU, KC_VOLD);
const key_override_t brdn = ko_make_basic(MOD_MASK_SHIFT, KC_BRIU, KC_BRID);

const key_override_t **key_overrides = (const key_override_t *[]){
    &aesc,
    &btn1,
    &btn2,
    &brdn,
    &vldn,
    &vmut,
    &lang,
    NULL
};

#endif // MODS_H
