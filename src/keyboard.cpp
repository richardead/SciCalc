#include <Arduino.h>
#include "IO.h"

typedef unsigned long long ULL;


const int rows = 9;
const int columns = 6;

int rowl[rows] = {KEYPAD_ROW_0, KEYPAD_ROW_1, KEYPAD_ROW_2, KEYPAD_ROW_3, KEYPAD_ROW_4, KEYPAD_ROW_5, KEYPAD_ROW_6, KEYPAD_ROW_7, KEYPAD_ROW_8};
int cols[columns] = {KEYPAD_COL_0, KEYPAD_COL_1, KEYPAD_COL_2, KEYPAD_COL_3, KEYPAD_COL_4, KEYPAD_COL_5};



ULL scan() {
    ULL state = 0;
    for(int col=0; col<columns; col++) {
        digitalWrite(cols[col], true);
        for(int row=0; row < rows; row++) {
            if(digitalRead(rowl[row])) {
                state += (1 << (col + row*columns));
            }
        }
    }
    return state;
}

volatile ULL lastKeyboardState = 0;


void IRAM_ATTR ISRscan() {
    ULL state = scan();
    ULL realState = state & lastKeyboardState;
    lastKeyboardState = state;
}

void init() {
    hw_timer_t * timer = NULL;

}