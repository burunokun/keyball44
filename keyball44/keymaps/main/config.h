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

#pragma once

// no RGB available in my keyboard
// #ifdef RGBLIGHT_ENABLE
// #    define RGBLIGHT_EFFECT_BREATHING
// #    define RGBLIGHT_EFFECT_RAINBOW_MOOD
// #    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
// #    define RGBLIGHT_EFFECT_SNAKE
// #    define RGBLIGHT_EFFECT_KNIGHT
// #    define RGBLIGHT_EFFECT_CHRISTMAS
// #    define RGBLIGHT_EFFECT_STATIC_GRADIENT
// #    define RGBLIGHT_EFFECT_RGB_TEST
// #    define RGBLIGHT_EFFECT_ALTERNATING
// #    define RGBLIGHT_EFFECT_TWINKLE
// #endif

#define TAP_CODE_DELAY 5

#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define TAPPING_TERM 200

#define ONESHOT_TIMEOUT 500

#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_TIME 500

#define DUMMY_MOD_NEUTRALIZER_KEYCODE KC_F24
#define MODS_TO_NEUTRALIZE {    \
            MOD_BIT(KC_LALT)  , \
            MOD_BIT(KC_RALT)  , \
            MOD_BIT(KC_LGUI)  , \
            MOD_BIT(KC_RGUI)  , \
}
