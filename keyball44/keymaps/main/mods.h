#ifndef MODS_H
#define MODS_H

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "lib/ansi_to_jis/translate_ansi_to_jis.h"

#include <stdbool.h>
#include <stdint.h>

// Define names for layers
enum _layers {
    _QWERTY = 0,
    _SYM,
    _FUN,
    _MOV,
};

// Define names for layer/mod keys
#define L_SYM OSL(_SYM)
#define L_FUN OSL(_FUN)

#define OM_LALT OSM(MOD_LALT)
#define OM_LGUI OSM(MOD_LGUI)

#define OM_LCTL OSM(MOD_LCTL)

#define OM_LSFT OSM(MOD_LSFT)
#define OM_RSFT OSM(MOD_RSFT)

#define RC_SCLN RCTL_T(KC_SCLN)

#define MS_BTN1 KC_MS_BTN1
#define MS_BTN2 KC_MS_BTN2

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

// Number Keys
const key_override_t shk3 = ko_make_basic(MOD_MASK_SHIFT, KC_3, KC_LCBR);
const key_override_t shk4 = ko_make_basic(MOD_MASK_SHIFT, KC_4, KC_LPRN);
const key_override_t shk5 = ko_make_basic(MOD_MASK_SHIFT, KC_5, KC_LBRC);
const key_override_t shk6 = ko_make_basic(MOD_MASK_SHIFT, KC_6, KC_RBRC);
const key_override_t shk7 = ko_make_basic(MOD_MASK_SHIFT, KC_7, KC_RPRN);
const key_override_t shk8 = ko_make_basic(MOD_MASK_SHIFT, KC_8, KC_RCBR);
const key_override_t shk9 = ko_make_basic(MOD_MASK_SHIFT, KC_9, KC_CIRC);
const key_override_t shk0 = ko_make_basic(MOD_MASK_SHIFT, KC_0, KC_DLR);

// Grave key
const key_override_t tgrv = ko_make_basic(MOD_MASK_SHIFT, KC_TILD, KC_GRV);
const key_override_t cgrv = ko_make_basic(MOD_BIT(KC_RCTL), KC_ESC, KC_GRV);

// Fn Keys
const key_override_t nfn1  = ko_make_basic(MOD_BIT(KC_RCTL), KC_1, KC_F1 );
const key_override_t nfn2  = ko_make_basic(MOD_BIT(KC_RCTL), KC_2, KC_F2 );
const key_override_t nfn3  = ko_make_basic(MOD_BIT(KC_RCTL), KC_3, KC_F3 );
const key_override_t nfn4  = ko_make_basic(MOD_BIT(KC_RCTL), KC_4, KC_F4 );
const key_override_t nfn5  = ko_make_basic(MOD_BIT(KC_RCTL), KC_5, KC_F5 );
const key_override_t nfn6  = ko_make_basic(MOD_BIT(KC_RCTL), KC_6, KC_F6 );
const key_override_t nfn7  = ko_make_basic(MOD_BIT(KC_RCTL), KC_7, KC_F7 );
const key_override_t nfn8  = ko_make_basic(MOD_BIT(KC_RCTL), KC_8, KC_F8 );
const key_override_t nfn9  = ko_make_basic(MOD_BIT(KC_RCTL), KC_9, KC_F9 );
const key_override_t nfn10 = ko_make_basic(MOD_BIT(KC_RCTL), KC_0, KC_F10);
const key_override_t nfn11 = ko_make_basic(MOD_BIT(KC_RCTL), KC_END, KC_F11);

// Home, End, PageUp, PageDown Keys
const key_override_t home = ko_make_basic(MOD_BIT(KC_RCTL), KC_LEFT, KC_HOME);
const key_override_t end  = ko_make_basic(MOD_BIT(KC_RCTL), KC_RGHT, KC_END );
const key_override_t pgup = ko_make_basic(MOD_BIT(KC_RCTL), KC_UP  , KC_PGUP);
const key_override_t pgdn = ko_make_basic(MOD_BIT(KC_RCTL), KC_DOWN, KC_PGDN);

const key_override_t **key_overrides = (const key_override_t *[]) {
    &brdn, &vldn, &vmut,
    &shk3, &shk4, &shk5, &shk6, &shk7, &shk8, &shk9, &shk0,
    &nfn1, &nfn2, &nfn3, &nfn4, &nfn5, &nfn6, &nfn7, &nfn8, &nfn9, &nfn10, &nfn11,
    &tgrv, &cgrv,
    &home, &end, &pgup, &pgdn,
    NULL
};

#endif // MODS_H
