/* Copyright 2020 imjoshellis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include <print.h>

bool     is_cmd_tab_active = false;
uint16_t cmd_tab_timer     = 0;

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes { ARW = SAFE_RANGE, ARW_FN, CMD_TAB };

// Define macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ARW:
            if (record->event.pressed) {
                SEND_STRING("=>");
            } else {
            }
            break;
        case ARW_FN:
            if (record->event.pressed) {
                SEND_STRING("() => {}");
            } else {
            }
            break;
        case CMD_TAB:
            if (record->event.pressed) {
                if (!is_cmd_tab_active) {
                    is_cmd_tab_active = true;
                    register_code(KC_LGUI);
                }
                cmd_tab_timer = timer_read();
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            break;
    }
    return true;
};

// ***** TAP DANCE *****
typedef struct {
    bool    is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
};

// Tap Dance declarations
enum {
    TD_VS_TERM,
    TD_LTHUMB,
    TD_RTHUMB
};

uint8_t cur_dance(qk_tap_dance_state_t *state);

void    lthumb_finished(qk_tap_dance_state_t *state, void *user_data);
void    lthumb_reset(qk_tap_dance_state_t *state, void *user_data);

// ***** Definitions *****

// Tap / Hold
#define LC_TAB  LCTL_T(KC_TAB)   // TAB  || CTRL 
#define LCA_ESC LCA_T(KC_ESC)    // ESC  || L2 
#define LT_SPC  TD(TD_LTHUMB)    // SPC  || TD LTHUMB 
#define RT_SPC  LT(1, KC_SPC)    // SPC  || TD LTHUMB 
#define SFT_TAB LSFT_T(KC_TAB)   // TAB  || SFT 
#define SG_BSPC  SGUI_T(KC_BSPC)   // SGUI || ENT 

// Window Controls
#define FLLSCRN C(G(KC_F))     // FULLSCREEN
#define W_UL    C(A(KC_U))     // UP-LEFT
#define W_U     C(A(KC_UP))    // UP
#define W_UR    C(A(KC_I))     // UP-RIGHT
#define W_L     C(A(KC_LEFT))  // LEFT
#define W_C     C(A(KC_C))     // CENTER
#define W_R     C(A(KC_RGHT))  // RIGHT
#define W_DL    C(A(KC_J))     // DOWN-LEFT
#define W_D     C(A(KC_DOWN))  // DOWN
#define W_DR    C(A(KC_K))     // DOWN-RIGHT
#define W_LTRD  C(A(KC_D))     // LEFT   1/3
#define W_CTRD  C(A(KC_F))     // CENTER 1/3
#define W_RTRD  C(A(KC_G))     // RIGHT  1/3
#define W_L2TRD C(A(KC_E))     // LEFT   2/3
#define W_R2TRD C(A(KC_T))     // RIGHT  2/3
#define W_MAX   C(A(KC_ENT))   // MAXIMIZE

// Space Navigation
#define N_ND    C(KC_RGHT)     // Next Desktop
#define N_PD    C(KC_LEFT)     // Prev Desktop
#define N_NW    G(KC_GRV)      // Next Window
#define N_PW    G(S(KC_GRV))   // Prev Window
#define N_NT    C(KC_TAB)      // Next Tab
#define N_PT    C(S(KC_TAB))   // Prev Tab

// Zoom Controls
#define Z_IN    G(KC_EQL)      // Zoom In
#define Z_OUT   G(KC_MINS)     // Zoom Out
#define Z_RESET G(KC_0)        // Zoom Reset

// VS Code Controls
#define VS_TERM TD(TD_VS_TERM) // Focus/Hide Terminal
#define VS_WIN1 G(KC_1)        // Focus First Panel
#define VS_WIN2 G(KC_2)        // Focus Second Panel
#define VS_EXP  G(S(KC_E))     // Focus Explorer
#define VS_GIT  C(S(KC_G))     // Focus Source Control

// Discord Controls
#define DIS_NCH A(S(KC_DOWN))  // Discord next unread channel

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_5x15(
    //  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    Z_OUT,   Z_RESET, Z_IN,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, 
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    FLLSCRN, OSL(3),  DIS_NCH, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS, 
        LC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_MUTE, KC_VOLD, KC_VOLU, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, 
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_MPRV, KC_MPLY, KC_MNXT, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT, 
        MO(3),   LCA_ESC, KC_LCTL, KC_LALT, KC_LGUI, LT_SPC,  SFT_TAB, _______, SG_BSPC,  RT_SPC,  CMD_TAB, OSL(3),  N_PD,    N_ND,    MO(12)
        ),
    [1] = LAYOUT_ortho_5x15(
    //  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    _______, _______, _______, _______, N_PW,    N_NT,    N_PT,    N_NW,    N_PD,    _______, 
        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, N_ND,    _______, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, VS_TERM, VS_WIN1, VS_WIN2, VS_EXP,  VS_GIT,  _______, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
        ),
    [2] = LAYOUT_ortho_5x15(
    //  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, _______, _______, _______, _______, KC_HASH, KC_LCBR, KC_RCBR, ARW,     _______, ARW_FN, 
        KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, _______, _______, _______, KC_CIRC, KC_LBRC, KC_RBRC, KC_MINS, KC_EQL,  KC_GRV, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DLR,  KC_UNDS, KC_PLUS, _______, _______, _______, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_ENT,  _______, _______, _______, _______, _______
        ),
    [3] = LAYOUT_ortho_5x15(
    //  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, W_LTRD,  W_CTRD,  W_CTRD,  _______, KC_NLCK, KC_PSLS, KC_PAST, KC_PMNS, _______, 
        _______, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, W_L2TRD, W_MAX,   W_R2TRD, _______, KC_P7,   KC_P8,   KC_P9,   KC_PPLS, _______, 
        _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______, W_UL,    W_U,     W_UR,    _______, KC_P4,   KC_P5,   KC_P6,   KC_PCMM, _______, 
        _______, _______, _______, _______, _______, _______, W_L,     W_C,     W_R,     _______, KC_P1,   KC_P2,   KC_P3,   KC_PEQL, _______, 
        _______, _______, _______, _______, _______, _______, W_DL,    W_D,     W_DR,    _______, KC_P0,   KC_P0,   KC_PDOT, KC_PENT, _______
        ),
    [12] = LAYOUT_ortho_5x15(
    //  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______, _______, _______, _______, _______, _______, DEBUG,   RESET,   _______, _______, _______, _______, _______, _______, _______, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
        ),
};

void matrix_init_user(void) {}

void matrix_scan_user(void) {
    if (is_cmd_tab_active) {
        if (timer_elapsed(cmd_tab_timer) > 400) {
            unregister_code(KC_LGUI);
            is_cmd_tab_active = false;
        }
    }
}

void led_set_user(uint8_t usb_led) {}

uint8_t int_tap_cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->pressed)
            return SINGLE_HOLD;
        else return SINGLE_TAP;
    } else if (state->count == 2) {
        if (state->pressed)
            return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    } else
        return 8;
}

uint8_t int_hold_cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed)
            return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (!state->pressed)
            return DOUBLE_TAP;
        else return DOUBLE_HOLD;
    } else
        return 8;
}

static tap lthumb_tap_state = {.is_press_action = true, .state = 0};

void lthumb_finished(qk_tap_dance_state_t *state, void *user_data) {
    lthumb_tap_state.state = int_hold_cur_dance(state);
    switch (lthumb_tap_state.state) {
        case SINGLE_TAP:
            dprint("lt single tap finished\n");
            register_code(KC_SPC);
            break;
        case SINGLE_HOLD:
            dprint("lt single hold finished\n"); 
            layer_on(2);  
            break;
        case DOUBLE_TAP:
            dprint("lt double tap finished\n");
            register_code(KC_ESC);
            break;
        case DOUBLE_HOLD:
            dprint("lt double hold finished\n");
            layer_on(1);  
            break;
    }
}

void lthumb_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (lthumb_tap_state.state) {
        case SINGLE_TAP:
            dprint("lt single tap reset\n");
            unregister_code(KC_SPC);
            break;
        case SINGLE_HOLD:
            dprint("lt single hold reset\n");
            layer_off(2);  
            break;
        case DOUBLE_TAP:
            dprint("lt double tap reset\n");
            unregister_code(KC_ESC);
            break;
        case DOUBLE_HOLD:
            dprint("lt double hold reset\n");
            layer_off(1);  
    }
    lthumb_tap_state.state = 0;
}

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for CMD+J, twice for CMD+SHFT+J
    [TD_VS_TERM] = ACTION_TAP_DANCE_DOUBLE(G(KC_J), G(S(KC_J))),
    // Left and right thumbs
    [TD_LTHUMB] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, lthumb_finished, lthumb_reset, 200),
};