#ifndef MODS_H
#define MODS_H

#include <stdbool.h>
#include <stdint.h>

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "keymap_japanese.h"

// Define names for layers
enum _layers {
    _QWERTY = 0,
    _SYM,
    _FUN,
    _MOV,
};

// Names for Layer/Mod Keys
#define L_SYM OSL(_SYM)
// #define L_FUN OSL(_FUN)
// #define L_SYM LT(_SYM, KC_SPC)
#define L_FUN LT(_FUN, KC_TAB)

// Left and Right ALT
// #define OM_LALT OSM(MOD_LALT)
// #define OM_RALT OSM(MOD_RALT)
#define LALT_K LALT_T(KC_TAB)

// Left and Right GUI
// #define OM_LGUI OSM(MOD_LGUI)
// #define OM_RGUI OSM(MOD_RGUI)
#define LGUI_K LGUI_T(KC_ESC)

// Left and Right CONTROL
#define OM_LCTL OSM(MOD_LCTL)
// #define OM_RCTL OSM(MOD_RCTL)
/*#define LCTL_K LCTL_T(KC_TAB)*/
#define RCTL_K LCTL_T(KC_ENT)

// Left and Right SHIFT
#define OM_LSFT OSM(MOD_LSFT)
#define OM_RSFT OSM(MOD_RSFT)

// Mouse Buttons
#define MS_BTN1 KC_MS_BTN1
#define MS_BTN2 KC_MS_BTN2

// Shortcuts
#define KC_SSHT G(S(KC_S))

// Custom key for scrolling and JIS mode
enum custom_keycodes {
    KC_SCR = SAFE_RANGE,
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

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RCTL_K:
            // Do not select the hold action when another key is pressed.
            return false;
        default:
            // Immediately select the hold action when another key is pressed.
            return true;
    }
}

// Auto mouse
void pointing_device_init_user(void) {
    set_auto_mouse_layer(_MOV);
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

// Media keys
const key_override_t vmut = ko_make_basic(MOD_MASK_ALT,   KC_VOLU, KC_MUTE);
const key_override_t vldn = ko_make_basic(MOD_MASK_SHIFT, KC_VOLU, KC_VOLD);
const key_override_t brdn = ko_make_basic(MOD_MASK_SHIFT, KC_BRIU, KC_BRID);

// To oneshot while in jis_mode
const key_override_t kc1 = ko_make_basic(MOD_MASK_SHIFT, JP_1, JP_AT);
const key_override_t kc2 = ko_make_basic(MOD_MASK_SHIFT, JP_2, JP_LBRC);
const key_override_t kc3 = ko_make_basic(MOD_MASK_SHIFT, JP_3, JP_LCBR);
const key_override_t kc4 = ko_make_basic(MOD_MASK_SHIFT, JP_4, JP_LPRN);
const key_override_t kc5 = ko_make_basic(MOD_MASK_SHIFT, JP_5, JP_PERC);
const key_override_t kc6 = ko_make_basic(MOD_MASK_SHIFT, JP_6, JP_AMPR);
const key_override_t kc7 = ko_make_basic(MOD_MASK_SHIFT, JP_7, JP_RPRN);
const key_override_t kc8 = ko_make_basic(MOD_MASK_SHIFT, JP_8, JP_RCBR);
const key_override_t kc9 = ko_make_basic(MOD_MASK_SHIFT, JP_9, JP_RBRC);
const key_override_t kc0 = ko_make_basic(MOD_MASK_SHIFT, JP_0, JP_DLR);

const key_override_t coln = ko_make_basic(MOD_MASK_SHIFT, JP_SCLN, JP_COLN);
const key_override_t dot  = ko_make_basic(MOD_MASK_SHIFT, JP_MINS, JP_UNDS);
const key_override_t plus = ko_make_basic(MOD_MASK_SHIFT, JP_QUOT, JP_DQUO);
const key_override_t dquo = ko_make_basic(MOD_MASK_SHIFT, JP_EQL, JP_PLUS);
const key_override_t pipe = ko_make_basic(MOD_MASK_SHIFT, JP_BSLS, JP_PIPE);

// Grave key
const key_override_t grv = ko_make_basic(MOD_MASK_SHIFT, JP_TILD, JP_GRV);

const key_override_t *key_overrides[] = {
    &brdn, &vldn, &vmut, &grv,
    &kc1 , &kc2, &kc3, &kc4, &kc5, &kc6, &kc7, &kc8, &kc9, &kc0,
    &coln, &dot, &plus, &dquo, &pipe,
    NULL
};

#endif // MODS_IMPLEMENTATION
