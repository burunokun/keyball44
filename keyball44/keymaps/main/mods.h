#ifndef MODS_H
#define MODS_H

#include <stdbool.h>
#include <stdint.h>

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "lib/ansi_to_jis/translate_ansi_to_jis.h"

// Define names for layers
enum _layers {
    _QWERTY = 0,
    _SYM,
    _FUN,
    _MOV,
};

// Names for layer/mod keys
#define L_SYM OSL(_SYM)
#define L_FUN OSL(_FUN)

// Left and Right ALT
#define OM_LALT OSM(MOD_LALT)
#define OM_RALT OSM(MOD_RALT)

// Left and Right GUI
#define OM_LGUI OSM(MOD_LGUI)
#define OM_RGUI OSM(MOD_RGUI)

// Left and Right CONTROl
#define OM_LCTL OSM(MOD_LCTL)
#define OM_RCTL OSM(MOD_RCTL)

// Left and Right SHIFT
#define OM_LSFT OSM(MOD_LSFT)
#define OM_RSFT OSM(MOD_RSFT)

// Mouse Buttons
#define MS_BTN1 KC_MS_BTN1
#define MS_BTN2 KC_MS_BTN2

// Right ALT and Right CONTROL
#define RC_SCLN RALT_T(KC_SCLN)
// #define RC_SLSH RCTL_T(KC_SLSH)

// Custom key for scrolling and JIS mode
enum custom_keycodes {
    KC_SCR = SAFE_RANGE,
    KC_JIS,
};

// keyboard states
enum click_state {
    NONE,
    SCROLLING
};

enum click_state state;
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
int16_t my_abs(int16_t num);

#endif // MODS_H

#ifdef MODS_IMPLEMENTATION

// Auto mouse
void pointing_device_init_user(void) {
    set_auto_mouse_layer(_MOV);
    set_auto_mouse_enable(true);
};

int16_t my_abs(int16_t num) {
    return num < 0 ? -num : num;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // case RC_SLSH:
        case RC_SCLN:
            // Do not select the hold action when another key is pressed.
            return false;
        default:
            // Immediately select the hold action when another key is pressed.
            return true;
    }
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
        case KC_JIS: {
            if (record->event.pressed) {
                set_jis_mode(!is_jis_mode());
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

    if (is_jis_mode()) {
        // if in JIS mode then convert keys to its JIS equivalent
        return process_record_user_a2j(keycode, record);
    }
    return true;
}

// Media keys
const key_override_t vmut = ko_make_basic(MOD_MASK_ALT,   KC_VOLU, KC_MUTE);
const key_override_t vldn = ko_make_basic(MOD_MASK_SHIFT, KC_VOLU, KC_VOLD);
const key_override_t brdn = ko_make_basic(MOD_MASK_SHIFT, KC_BRIU, KC_BRID);

// To oneshot while in jis_mode
const key_override_t kc1 = ko_make_basic(MOD_MASK_SHIFT, KC_1, KC_EQL);
const key_override_t kc2 = ko_make_basic(MOD_MASK_SHIFT, KC_2, KC_RBRC);
const key_override_t kc3 = ko_make_basic(MOD_MASK_SHIFT, KC_3, S(KC_RBRC));
const key_override_t kc4 = ko_make_basic(MOD_MASK_SHIFT, KC_4, S(KC_8));
// const key_override_t kc5 = ko_make_basic(MOD_MASK_SHIFT, KC_5, KC_LBRC);
const key_override_t kc6 = ko_make_basic(MOD_MASK_SHIFT, KC_6, KC_LBRC);
const key_override_t kc7 = ko_make_basic(MOD_MASK_SHIFT, KC_7, KC_LPRN);
const key_override_t kc8 = ko_make_basic(MOD_MASK_SHIFT, KC_8, S(KC_NUHS));
const key_override_t kc9 = ko_make_basic(MOD_MASK_SHIFT, KC_9, KC_NUHS);
const key_override_t kc0 = ko_make_basic(MOD_MASK_SHIFT, KC_0, KC_DLR);

const key_override_t coln = ko_make_basic(MOD_MASK_SHIFT, KC_SCLN, KC_QUOT);
const key_override_t unds = ko_make_basic(MOD_MASK_SHIFT, KC_MINS, S(KC_INT1));

// Grave key
const key_override_t grv = ko_make_basic(MOD_MASK_SHIFT, KC_TILD, S(KC_LCBR));

const key_override_t **key_overrides = (const key_override_t *[]) {
    &brdn, &vldn, &vmut, &grv,
    &kc1, &kc2, &kc3, &kc4, /*&kc5,*/ &kc6, &kc7, &kc8, &kc9, &kc0,
    &coln, &unds,
    NULL
};

#endif // MODS_IMPLEMENTATION
