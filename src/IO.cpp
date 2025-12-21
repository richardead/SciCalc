#include <Arduino.h>
#include "IO.h"

void setupIO() {
    pinMode(KEYPAD_ROW_0, INPUT_PULLDOWN);
    pinMode(KEYPAD_ROW_1, INPUT_PULLDOWN);
    pinMode(KEYPAD_ROW_2, INPUT_PULLDOWN);
    pinMode(KEYPAD_ROW_3, INPUT_PULLDOWN);
    pinMode(KEYPAD_ROW_4, INPUT_PULLDOWN);
    pinMode(KEYPAD_ROW_5, INPUT_PULLDOWN);
    pinMode(KEYPAD_ROW_6, INPUT_PULLDOWN);
    pinMode(KEYPAD_ROW_7, INPUT_PULLDOWN);
    pinMode(KEYPAD_ROW_8, INPUT_PULLDOWN);

    pinMode(KEYPAD_COL_0, OUTPUT);
    pinMode(KEYPAD_COL_1, OUTPUT);
    pinMode(KEYPAD_COL_2, OUTPUT);
    pinMode(KEYPAD_COL_3, OUTPUT);
    pinMode(KEYPAD_COL_4, OUTPUT);
    pinMode(KEYPAD_COL_5, OUTPUT);

    pinMode(IR_DIODE,    OUTPUT);
    pinMode(IR_RECEIVER, INPUT );
    pinMode(BAT_REFF,    INPUT );
    
    pinMode(DISPLAY_SCK, OUTPUT);
    pinMode(DISPLAY_SDA, OUTPUT);
    pinMode(DISPLAY_RST, OUTPUT);
    pinMode(DISPLAY_DC,  OUTPUT);
    pinMode(DISPLAY_CS,  OUTPUT);
    
    
    pinMode(SERIAL_1_TX, OUTPUT);
    pinMode(SERIAL_1_RX, INPUT );
}
