// Copyright 2022 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// clang-format off

#define U_LTESC LT(7, KC_ESC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_top(
            KC_HOME,           KC_MUTE,           KC_MPLY,
            TO(0),    TO(1),   TO(2),   TO(3),   TO(4),
             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
             U_LTESC, KC_TAB,  KC_SPC,  KC_BSPC, KC_ENT
    ),
    [1] = LAYOUT_top(
            KC_HOME,           KC_MUTE,           KC_MPLY,
             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
             KC_TRNS, KC_TAB,  KC_SPC,  KC_BSPC, KC_ENT
    ),
    [2 ... 6] = LAYOUT_top(
            KC_TRNS,           KC_TRNS,           KC_TRNS,
             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [7] = LAYOUT_top(
            RGB_M_P,           RGB_M_B,           RGB_M_R,
             RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI,
             RGB_RMOD,RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD,
             KC_TRNS, RGB_TOG, RGB_M_P, RGB_M_B, RGB_M_R
    ),
};

// clang-format on

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] =        { ENCODER_CCW_CW(KC_PGUP, KC_PGDN), ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [1] =        { ENCODER_CCW_CW(RGB_HUD, RGB_HUI), ENCODER_CCW_CW(RGB_SAD, RGB_SAI), ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [2 ... 7] =  { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) }
};
#endif

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t ledindex = 0;
    switch(get_highest_layer(layer_state|default_layer_state)) {
        case 4:
            ledindex = 18; // Row 0, column 4
            break;
        case 3:
            ledindex = 17; // Row 0, column 3
            break;
        case 2:
            ledindex = 12; // Row 0, column 2
            break;
        case 1:
            ledindex = 11; // Row 0, column 1
            break;
        case 0:
            ledindex = 6; // Row 0, column 0
            break;
        default:
            ledindex = led_max;
    }
    if(ledindex < led_max) {
        HSV hsv = {HSV_GREEN};
        if(hsv.v > RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
            hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
        }
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(ledindex, rgb.r, rgb.g, rgb.b);
    }
    return false;
}

void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_OFF);
}