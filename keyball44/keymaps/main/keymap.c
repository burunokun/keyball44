/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "quantum.h"

#include "mods.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [0] = LAYOUT_universal(
    TD(TD_ALT_TAB), KC_Q  , KC_W   , KC_E     , KC_R     , KC_T     ,            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_BSPC  ,
    KC_ESC        , KC_A  , KC_S   , KC_D     , KC_F     , KC_G     ,            KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , KC_ENT   ,
    OSM(MOD_LSFT) , KC_Z  , KC_X   , KC_C     , KC_V     , KC_B     ,            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , OSM(MOD_RALT) ,
    KC_BRIU  , KC_VOLU  , OSM(MOD_LGUI) , KC_SPACE  , OSM(MOD_LCTL) ,            OSL(2) , OSL(1)  , _______  , _______  , KC_ALGR
  ),

  [1] = LAYOUT_universal(
    KC_TAB   , KC_EXLM  , KC_LBRC  , KC_LCBR  , KC_LPRN  , KC_GRV   ,            KC_EQL   , KC_RPRN  , KC_RCBR  , KC_RBRC  , KC_PLUS  , KC_BSPC  ,
    KC_ESC   , KC_TILD  , KC_CIRC  , KC_HASH  , KC_ASTR  , KC_DLR   ,            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , KC_QUOT  , KC_ENT   ,
    KC_LSFT  , KC_AT    , KC_PERC  , KC_UNDS  , KC_BSLS  , KC_AMPR  ,            KC_HOME  , KC_PGDN  , KC_PGUP  , KC_END   , KC_MINUS , KC_RALT  ,
    KC_INT3  , KC_INT1  , KC_LGUI  , KC_SPACE , KC_LCTL  ,                       KC_RCTL  , G(KC_SPC), _______  , _______  , KC_DEL
  ),

  [2] = LAYOUT_universal(
    KC_TAB   ,A(S(KC_B)), KC_BRK   , KC_F8    , KC_F4    , KC_F10   ,            KC_ASTR  , KC_7     , KC_8     , KC_9     , KC_EQL   , KC_BSPC  ,
    KC_ESC   , G(KC_T)  , CW_TOGG  , KC_F5    , KC_F2    , KC_F11   ,            _______  , KC_4     , KC_5     , KC_6     , KC_0     , KC_ENT   ,
    KC_LSFT  , G(KC_B)  , KC_DOT   , KC_COMM  , KC_F9    , KC_F12   ,            KC_PERC  , KC_1     , KC_2     , KC_3     , KC_MINUS , KC_RALT  ,
    _______  , _______  , KC_LGUI  , KC_SPACE , KC_LCTL  ,                       KC_APP   , KC_RALT  , _______  , _______  , KC_DEL
  ),

  [3] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,            _______  , _______  , _______  , _______  , _______  , _______  ,
    KC_ESC   , _______  , _______  , _______  , _______  , _______  ,            _______  , _______  , KC_SCR   , _______  , _______  , _______  ,
    KC_LSFT  , _______  , _______  , _______  , _______  , _______  ,            KC_BTN4  , KC_BTN1  , KC_BTN2  , KC_BTN5  , _______  , KC_RSFT  ,
    _______  , _______  , KC_LGUI  , _______  , KC_LCTL  ,                       _______  , _______  , _______  , _______  , _______
  ),

};

// clang-format on
layer_state_t layer_state_set_user(layer_state_t state) {
    switch(get_highest_layer(remove_auto_mouse_layer(state, true))) {
        case 1 ... 2:
            state = remove_auto_mouse_layer(state, false);
            set_auto_mouse_enable(false);
            break;
        default:
            set_auto_mouse_enable(true);
            break;
    }
    return state;
}

// #ifdef OLED_ENABLE

// #include "lib/oledkit/oledkit.h"

// void oledkit_render_info_user(void) {
//     keyball_oled_render_keyinfo();
//     keyball_oled_render_ballinfo();
//     keyball_oled_render_layerinfo();
// }
// #endif
