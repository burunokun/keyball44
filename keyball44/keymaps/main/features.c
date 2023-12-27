#include QMK_KEYBOARD_H
#include "quantum.h"

void pointing_device_init_user(void) {
    set_auto_mouse_enable(true);
};

const key_override_t aesc = ko_make_basic(MOD_MASK_ALT, KC_SCLN, KC_ESC);

const key_override_t mu = ko_make_basic(MOD_MASK_CG, KC_K, KC_WH_U);
const key_override_t mr = ko_make_basic(MOD_MASK_CG, KC_L, KC_WH_R);
const key_override_t md = ko_make_basic(MOD_MASK_CG, KC_J, KC_WH_D);
const key_override_t ml = ko_make_basic(MOD_MASK_CG, KC_H, KC_WH_L);

const key_override_t brdn = ko_make_basic(MOD_MASK_SHIFT, KC_BRIU, KC_BRID);
const key_override_t vldn = ko_make_basic(MOD_MASK_SHIFT, KC_VOLU, KC_VOLD);

const key_override_t **key_overrides = (const key_override_t *[]){
    &aesc,
    &brdn,
    &vldn,
    &mu,
    &mr,
    &md,
    &ml,
    NULL
};
