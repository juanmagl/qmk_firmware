#include QMK_KEYBOARD_H

enum layer_names {
    _BASE_LAYER,
    _FUNC_LAYER,
    _NUM_LAYER,
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Tap dance enums
enum {
    FN_CTL,
    CAPS_CTL,
    SOME_OTHER_DANCE
};

td_state_t cur_dance(qk_tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void fn_finished(qk_tap_dance_state_t *state, void *user_data);
void fn_reset(qk_tap_dance_state_t *state, void *user_data);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    //,: Base Layer
    [_BASE_LAYER] = LAYOUT_60_iso(
        KC_ESC,       KC_1,    KC_2,     KC_3,     KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,    KC_EQL,  KC_BSPC,
        KC_TAB,       KC_Q,    KC_W,     KC_E,     KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,    KC_RBRC,
        TD(CAPS_CTL), KC_A,    KC_S,     KC_D,     KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,    KC_NUHS, KC_ENT,
        KC_LSFT,      KC_NUBS, KC_Z,     KC_X,     KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,             KC_RSFT,
        KC_LCTL,      KC_LGUI, KC_LALT,                              KC_SPC,                             KC_RALT, TD(FN_CTL), KC_RCTL, KC_APP
	),

    //,: Function Layer
    [_FUNC_LAYER] = LAYOUT_60_iso(
        KC_GRV,  KC_F1,   KC_F2,    KC_F3,    KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        _______, XXXXXXX, KC_HOME,  KC_UP,    KC_END,  KC_PGUP, KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_PSCR, KC_SLCK, KC_PAUS,
        _______, XXXXXXX, KC_LEFT,  KC_DOWN,  KC_RGHT, KC_PGDN, KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX, KC_INS,  _______,
        _______, XXXXXXX, C(KC_Z),  C(KC_X),  C(KC_C), C(KC_V), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          _______,
        _______, _______, _______,                              _______,                            _______, _______, _______, _______
	),

    //,: Numerical Layer
    [_NUM_LAYER] = LAYOUT_60_iso(
        KC_GRV,  KC_A,    KC_B,     KC_C,     KC_D,    KC_E,    KC_F,    KC_P7,   KC_P8,   KC_P9,   KC_PSLS, XXXXXXX, KC_NLCK, KC_BSPC,
        _______, XXXXXXX, KC_END,   KC_UP,    KC_HOME, KC_PGUP, XXXXXXX, KC_P4,   KC_P5,   KC_P6,   KC_PAST, XXXXXXX, XXXXXXX,
        _______, XXXXXXX, KC_LEFT,  KC_DOWN,  KC_RGHT, KC_PGDN, XXXXXXX, KC_P1,   KC_P2,   KC_P3,   KC_PMNS, KC_PEQL, KC_INS,  KC_PENT,
        _______, XXXXXXX, C(KC_Z),  C(KC_X),  C(KC_C), C(KC_V), XXXXXXX, XXXXXXX, KC_P0,   KC_PCMM, KC_PDOT, KC_PPLS,          _______,
        _______, _______, _______,                              _______,                            _______, _______, _______, _______
	)

};

/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustating to type.
 *
 * For the third point, there does exist the 'TD_DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        // Obviating state->interrupted on first tap to allow for HOLD_ON_OTHER_KEY_PRESS behaviour
        //if (state->interrupted || !state->presed) return TD_SINGLE_TAP;
        if (!state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for the 'fn' tap dance.
static td_tap_t fntap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Create an instance of 'td_tap_t' for the 'caps_lock' tap dance.
static td_tap_t capstap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void fn_finished(qk_tap_dance_state_t *state, void *user_data) {
    fntap_state.state = cur_dance(state);
    switch (fntap_state.state) {
        case TD_SINGLE_TAP:
            // Enable BASE as only layer.
            layer_move(_BASE_LAYER);
            break;
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_SINGLE_TAP:
            // Enable FUNC layer.
            layer_clear();
            layer_on(_FUNC_LAYER);
            break;
        case TD_DOUBLE_HOLD:
        case TD_TRIPLE_TAP:
            layer_clear();
            layer_on(_NUM_LAYER);
            break;
        default:
            break;
    }
}

void fn_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (fntap_state.state) {
        case TD_SINGLE_TAP:
            break;
        case TD_SINGLE_HOLD:
            layer_off(_FUNC_LAYER);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_SINGLE_TAP:
            break;
        case TD_DOUBLE_HOLD:
            layer_off(_NUM_LAYER);
            break;
        case TD_TRIPLE_TAP:
            break;
        default:
            break;
    }
    fntap_state.state = TD_NONE;
}

void caps_finished(qk_tap_dance_state_t *state, void *user_data) {
    capstap_state.state = cur_dance(state);
    switch (capstap_state.state) {
        case TD_SINGLE_TAP:
            if( IS_LAYER_ON(_FUNC_LAYER) || IS_LAYER_ON(_NUM_LAYER)) {
                // Do nothing here
            } else {
                // Send CapsLock.
                register_code(KC_CAPS_LOCK);
            }
            break;
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_SINGLE_TAP:
            // Enable FUNC layer.
            layer_clear();
            layer_on(_FUNC_LAYER);
            break;
        case TD_DOUBLE_HOLD:
        case TD_TRIPLE_TAP:
            layer_clear();
            layer_on(_NUM_LAYER);
            break;
        default:
            break;
    }
}

void caps_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (capstap_state.state) {
        case TD_SINGLE_TAP:
            if( IS_LAYER_ON(_FUNC_LAYER) || IS_LAYER_ON(_NUM_LAYER)) {
                // Return to base layer
                layer_move(_BASE_LAYER);
            } else {
                // Unregister CapsLock.
                unregister_code(KC_CAPS_LOCK);
            }
            break;
        case TD_SINGLE_HOLD:
            layer_off(_FUNC_LAYER);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_SINGLE_TAP:
            break;
        case TD_DOUBLE_HOLD:
            layer_off(_NUM_LAYER);
            break;
        case TD_TRIPLE_TAP:
            break;
        default:
            break;
    }
    capstap_state.state = TD_NONE;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [FN_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fn_finished, fn_reset),
    [CAPS_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, caps_finished, caps_reset)
};
