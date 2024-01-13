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
    KC_SCR = SAFE_RANGE
};

enum click_state {
    NONE,
    SCROLLING
};

enum click_state state;

int16_t my_abs(int16_t num) {
    return num < 0 ? -num : num;
}

int16_t scroll_v_mouse_interval_counter;
int16_t scroll_h_mouse_interval_counter;

int16_t scroll_v_threshold = 50;
int16_t scroll_h_threshold = 50;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    int16_t current_x = mouse_report.x;
    int16_t current_y = mouse_report.y;
    int16_t current_h = 0;
    int16_t current_v = 0;

    if (current_x != 1000 || current_y != 1000) {
        set_auto_mouse_enable(true);
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
    } else {
        set_auto_mouse_enable(false);
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
         } break;

        default: {
            state = NONE;
        } break;
    }

    return true;
}

const key_override_t aesc = ko_make_basic(MOD_MASK_ALT, KC_SCLN, KC_ESC);

const key_override_t mu = ko_make_basic(MOD_MASK_CG, KC_K, KC_WH_U);
const key_override_t mr = ko_make_basic(MOD_MASK_CG, KC_L, KC_WH_R);
const key_override_t md = ko_make_basic(MOD_MASK_CG, KC_J, KC_WH_D);
const key_override_t ml = ko_make_basic(MOD_MASK_CG, KC_H, KC_WH_L);

const key_override_t n1 = ko_make_basic(MOD_MASK_SHIFT, KC_EXLM, KC_1);
const key_override_t n2 = ko_make_basic(MOD_MASK_SHIFT, KC_LBRC, KC_2);
const key_override_t n3 = ko_make_basic(MOD_MASK_SHIFT, KC_LCBR, KC_3);
const key_override_t n4 = ko_make_basic(MOD_MASK_SHIFT, KC_LPRN, KC_4);
const key_override_t n5 = ko_make_basic(MOD_MASK_SHIFT, KC_PLUS, KC_5);
const key_override_t n6 = ko_make_basic(MOD_MASK_SHIFT, KC_EQL , KC_6);
const key_override_t n7 = ko_make_basic(MOD_MASK_SHIFT, KC_RPRN, KC_7);
const key_override_t n8 = ko_make_basic(MOD_MASK_SHIFT, KC_RCBR, KC_8);
const key_override_t n9 = ko_make_basic(MOD_MASK_SHIFT, KC_RBRC, KC_9);
const key_override_t n0 = ko_make_basic(MOD_MASK_SHIFT, KC_DQUO, KC_0);

const key_override_t brdn = ko_make_basic(MOD_MASK_SHIFT, KC_BRIU, KC_BRID);
const key_override_t vldn = ko_make_basic(MOD_MASK_SHIFT, KC_VOLU, KC_VOLD);

const key_override_t **key_overrides = (const key_override_t *[]){
    &aesc,
    &brdn,
    &vldn,
    &n1,
    &n2,
    &n3,
    &n4,
    &n5,
    &n6,
    &n7,
    &n8,
    &n9,
    &n0,
    &mu,
    &mr,
    &md,
    &ml,
    NULL
};

#endif // MODS_H
