/////////////////////////////////////////////////////////////////////////////
// Copyright 2022 @Yowkees
// Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
/////////////////////////////////////////////////////////////////////////////
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
/////////////////////////////////////////////////////////////////////////////
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
/////////////////////////////////////////////////////////////////////////////
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "keymap_japanese.h"

#define MODS_IMPLEMENTATION
#include "mods.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT_universal(
    JP_TILD  , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,  KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_BSPC  ,
    LALT_K   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,  KC_H     , KC_J     , KC_K     , KC_L     , JP_SCLN  , RALT_K   ,
    OM_LSFT  , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,  KC_N     , KC_M     , KC_COMM  , KC_DOT   , RCTL_K   , OM_RSFT  ,
               _______  , _______  , KC_LGUI  , KC_SPC   , LCTL_K   ,  L_FUN    , L_SYM    , _______  , _______  , KC_DEL
   ),

  [_SYM] = LAYOUT_universal(
    _______  , KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,  KC_6     , KC_7     , KC_8     , KC_9     , KC_0     , _______  ,
//  _______  , JP_AT    , JP_LBRC  , JP_LCBR  , JP_LPRN  , JP_LABK  ,  JP_RABK  , JP_RPRN  , JP_RCBR  , JP_RBRC  , JP_DLR   , _______  ,
    _______  , JP_AMPR  , JP_MINS  , JP_QUOT  , JP_ASTR  , JP_EQL   ,  KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , _______  , _______  ,
    _______  , JP_BSLS  , JP_UNDS  , JP_EXLM  , JP_HASH  , JP_PERC  ,  KC_HOME  , KC_PGDN  , KC_PGUP  , KC_END   , _______  , _______  ,
               _______  , _______  , _______  , _______  , _______  ,  _______  , KC_SPC   , _______  , _______  , _______
  ),

  [_FUN] = LAYOUT_universal(
/*
    _______  , KC_F1    , KC_F2    , KC_F3    , KC_F4    , _______  ,  KC_SCRL  , KC_7     , KC_8     , KC_9     , _______  , _______  ,
    _______  , KC_F5    , KC_F6    , KC_F7    , KC_F8    , KC_VOLU  ,  KC_INS   , KC_4     , KC_5     , KC_6     , KC_0     , _______  ,
    _______  , KC_F9    , KC_F10   , KC_F11   , KC_F12   , KC_BRIU  ,  KC_BRK   , KC_1     , KC_2     , KC_3     , _______  , _______  ,
               _______  , _______  , _______  , _______  , _______  ,  _______  , KC_SPC   , _______  , _______  , _______
*/
    _______  , KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,  KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , _______  ,
    _______  , _______  , KC_SSHT  , KC_BRK   , _______  , KC_VOLU  ,  KC_F11   , KC_APP   , _______  , _______  , _______  , _______  ,
    _______  , KC_SCRL  , _______  , _______  , KC_INS   , KC_BRIU  ,  KC_F12   , KC_GRV   , _______  , _______  , _______  , _______  ,
               _______  , _______  , _______  , _______  , _______  ,  _______  , KC_SPC   , _______  , _______  , _______
  ),

  [_MOV] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,  _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,  _______  , _______  , KC_SCR   , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,  _______  , KC_BTN1  , KC_BTN2  , _______  , _______  , _______  ,
               _______  , _______  , _______  , _______  , _______  ,  _______  , _______  , _______  , _______  , _______
  ),
};

// clang-format on
layer_state_t layer_state_set_user(layer_state_t state) {
    switch(get_highest_layer(remove_auto_mouse_layer(state, true))) {
        case _SYM ... _FUN:
            state = remove_auto_mouse_layer(state, false);
            set_auto_mouse_enable(false);
            break;
        default:
            set_auto_mouse_enable(true);
            break;
    }
    return state;
}

// unfortunately OLED is broken
// #ifdef OLED_ENABLE
// #include "lib/oledkit/oledkit.h"
// void oledkit_render_info_user(void) {
//     keyball_oled_render_keyinfo();
//     keyball_oled_render_ballinfo();
//     keyball_oled_render_layerinfo();
// }
// #endif
