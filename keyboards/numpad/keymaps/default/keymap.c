#include "numpad.h"
#include <avr/wdt.h>


#define NUMPAD  0
#define UTILITY 1

enum functionId {
	PROMICRO_PROGRAM
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[NUMPAD] = KEYMAP( /* Base */
  LT(UTILITY, KC_NLCK), KC_PSLS,  KC_PAST, KC_PMNS, \
  KC_P7               , KC_P8  ,  KC_P9  , KC_PPLS, \
  KC_P4               , KC_P5  ,  KC_P6  , KC_NO,   \
  KC_P1               , KC_P2  ,  KC_P3  , KC_PENT, \
  KC_P0               , KC_NO  ,  KC_PDOT, KC_NO    \  
),
[UTILITY] = KEYMAP(
  KC_TRNS, KC_PSLS,  KC_PAST, KC_PMNS, \
  KC_P7  , KC_P8  ,  KC_P9  , KC_PPLS, \
  KC_P4  , KC_P5  ,  KC_P6  , KC_NO  , \
  KC_P1  , KC_P2  ,  KC_P3  , KC_FN0 , \
  KC_P0  , KC_NO  ,  KC_PDOT, KC_NO    \  
)
};

const uint16_t PROGMEM fn_actions[] = {
	[0] = ACTION_FUNCTION(PROMICRO_PROGRAM)
};

void promicro_program(bool program) {
	uint16_t *const bootKeyPtr = (uint16_t *) 0x0800;

	// Value used by Caterina bootloader use to determine whether to run the
	// sketch or the bootloader programmer.
	uint16_t bootKey = program ? 0x7777 : 0;

	*bootKeyPtr = bootKey;

	// setup watchdog timeout
	wdt_enable(WDTO_60MS);

	while(1) {} // wait for watchdog timer to trigger
}

void action_function(keyrecord_t* record, uint8_t id, uint8_t opt) {
	switch (id) {
		case PROMICRO_PROGRAM:	
			promicro_program(true);
			break;
		default:
			break;
	}
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
          if (record->event.pressed) {
            register_code(KC_RSFT);
          } else {
            unregister_code(KC_RSFT);
          }
        break;
      }
    return MACRO_NONE;
};


void matrix_init_user(void) {
}


uint8_t ledStatus;
int q = 0;
void matrix_scan_user(void) {
	uint8_t layer = biton32(layer_state);
    
	switch (layer) {
		case UTILITY:
			if (q == 0) {
				setNumlockLed(true);
			} else if (q == 512) {
				setNumlockLed(false);
			}
			q++;
			if (q == 1024) {
				q = 0;
			}
			break;
		default:
			if (q != 0) {
				led_set_user(ledStatus);
				q = 0;
			}
			break;
	}
}

void led_set_user(uint8_t usb_led) {
	ledStatus = usb_led;
	if (usb_led & (1<<USB_LED_NUM_LOCK)) {
		setNumlockLed(false);
	} else {
		setNumlockLed(true);
	}

}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}
