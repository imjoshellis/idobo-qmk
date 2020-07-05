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

// Change tapping term for left space to be very short
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(2, KC_SPC):
            return 80;
        default:
            return TAPPING_TERM;
    }
}

// Tap Dance declarations
enum {
    TD_VS_TERM,
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for CMD+J, twice for CMD+SHFT+J
    [TD_VS_TERM] = ACTION_TAP_DANCE_DOUBLE(G(KC_J), G(S(KC_J))),
};

// Definitions for cleaner matrix 

// Tap / Hold
#define LC_TAB  LCTL_T(KC_TAB)   // TAB  || CTRL 
#define LT_Z    LT(1, KC_Z)      // Z    || L1 
#define LT_SLSH LT(2, KC_SLSH)   // SLSH || L2
#define LCA_ESC LCA_T(KC_ESC)    // ESC  || L2 
#define LT_SPC  LT(2, KC_SPC)    // SPC  || L2 
#define SFT_TAB LSFT_T(KC_TAB)   // TAB  || SFT 
#define SG_ENT  SGUI_T(KC_ENT)   // SGUI || ENT 

// Window Controls
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_5x15(
    //  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    Z_OUT,   Z_RESET, Z_IN,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, 
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    _______, OSL(3),  _______, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS, 
        LC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    _______, _______, _______, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, 
        KC_LSFT, LT_Z,    KC_X,    KC_C,    KC_V,    KC_B,    _______, _______, _______, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT, 
        MO(3),   LCA_ESC, KC_LCTL, KC_LALT, KC_LGUI, LT_SPC,  SFT_TAB, _______, SG_ENT,  KC_SPC,  CMD_TAB, OSL(3),  N_PD,    N_ND,    MO(12)
        ),
    [1] = LAYOUT_ortho_5x15(
    //  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, KC_BRID, KC_BRIU, _______, RGB_TOG, BL_TOGG, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, _______, _______, _______, N_PW,    N_NT,    N_PT,    N_NW,    N_PD,    _______, 
        _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, N_ND,    _______, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, VS_TERM, VS_WIN1, VS_WIN2, VS_EXP,  VS_GIT,  _______, 
        _______, _______, _______, _______, _______, KC_ENT,  _______, _______, _______, _______, _______, _______, _______, _______, _______
        ),
    [2] = LAYOUT_ortho_5x15(
    //  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______, KC_MUTE, KC_VOLD, KC_VOLU, KC_PERC, KC_AMPR, _______, _______, _______, KC_AT,   KC_LPRN, KC_RPRN, ARW,     _______, ARW_FN, 
        _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_EXLM, KC_CIRC, _______, _______, _______, KC_DLR,  KC_LBRC, KC_RBRC, KC_MINS, KC_EQL,  KC_GRV, 
        _______, _______, _______, _______, _______, KC_ASTR, _______, _______, _______, KC_HASH, KC_UNDS, KC_PLUS, KC_GT,   _______, _______, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_ENT,  _______, _______, _______, _______, _______
        ),
    [3] = LAYOUT_ortho_5x15(
    //  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   _______, W_LTRD,  W_CTRD,  W_CTRD,  _______, KC_NLCK, KC_PSLS, KC_PAST, KC_PMNS, _______, 
        _______, KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______, W_L2TRD, W_MAX,   W_R2TRD, _______, KC_P7,   KC_P8,   KC_P9,   KC_PPLS, _______, 
        _______, KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, W_UL,    W_U,     W_UR,    _______, KC_P4,   KC_P5,   KC_P6,   KC_PCMM, _______, 
        _______, _______, _______, _______, _______, _______, W_L,     W_C,     W_R,     _______, KC_P1,   KC_P2,   KC_P3,   KC_PEQL, _______, 
        _______, _______, _______, _______, _______, _______, W_DL,    W_D,     W_DR,    _______, KC_P0,   KC_P0,   KC_PDOT, KC_PENT, _______
        ),
    [12] = LAYOUT_ortho_5x15(
    //  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______, _______, _______, _______, _______, _______, _______, RESET,   _______, _______, _______, _______, _______, _______, _______, 
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