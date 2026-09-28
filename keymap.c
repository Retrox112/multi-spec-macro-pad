#include QMK_KEYBOARD_H

#define JOYSTICK_BTN_PIN GP26 
#define ENCODER_BTN_PIN  GP27

static bool is_scroll_mode = false;
static bool last_joy_btn_state = true;

//keymaps blank, MAP THEM IDIOT
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_direct(
        KC_WBAK,         // SW1 Browser Back
        KC_WFWD,         // SW2 Browser Forward
        KC_WREF,         // SW3 Browser Refresh

        KC_COPY,         // SW4 Ctrl+C
        KC_CUT,          // SW5 Ctrl+X
        KC_PASTE,        // SW6 Ctrl+V

        HYPR(KC_D),      // SW7 Ctrl+Alt+Shift+Win + D
        HYPR(KC_Y),      // SW8 Ctrl+Alt+Shift+Win + Y
        HYPR(KC_T)       // SW9 Ctrl+Alt+Shift+Win + T
    )
};
//encoder vol stuff
if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) }
};
endif
//joystick btn def ig
void matrix_init_user(void) {
   pull-ups
    setPinInputHigh(JOYSTICK_BTN_PIN);
    setPinInputHigh(ENCODER_BTN_PIN);
}

void housekeeping_task_user(void) {
//encoder button mute
    static bool last_enc_btn_state = true;
    bool curr_enc_btn_state = readPin(ENCODER_BTN_PIN);
    
    if (last_enc_btn_state && !curr_enc_btn_state) { 
        tap_code(KC_MUTE);
    }
    last_enc_btn_state = curr_enc_btn_state;
//joystick button toggle
    bool curr_joy_btn_state = readPin(JOYSTICK_BTN_PIN);
    
    if (last_joy_btn_state && !curr_joy_btn_state) { 
        is_scroll_mode = !is_scroll_mode;
    }
    last_joy_btn_state = curr_joy_btn_state;
}
//scroll mode for joystick
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (is_scroll_mode) {
        wheel vectors (h, v)
        mouse_report.h = mouse_report.x;
        mouse_report.v = -mouse_report.y;
        
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}