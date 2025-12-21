#include <Arduino.h>
#include "IO.h"
#include "light_containers.h"
#include "keyboard.h"

typedef unsigned long long ULL;


light_queue<event> SystemEventQueue;

//ten napis zostal napisany o 21:37 2 kwietnia 2025 :)
volatile int _modifierButton = -1;
ULL _isModifier = 0b0000000000000000000000000000000000000000000000000000000000001000ULL; // teraz jest shift bo on ma numer 3

int _rowl[KeyboardRows] = {KEYPAD_ROW_0, KEYPAD_ROW_1, KEYPAD_ROW_2, KEYPAD_ROW_3, KEYPAD_ROW_4, KEYPAD_ROW_5, KEYPAD_ROW_6, KEYPAD_ROW_7, KEYPAD_ROW_8};
int _cols[KeyboardColumns] = {KEYPAD_COL_0, KEYPAD_COL_1, KEYPAD_COL_2, KEYPAD_COL_3, KEYPAD_COL_4, KEYPAD_COL_5};

volatile ULL _lastKeyboardState = 0;
volatile ULL _realKeyboardState = 0;

bool isButtonModifier(int row, int column) { 
    return (_isModifier >> (column + row*KeyboardColumns))&1ULL; 
}

bool isButtonModifier(int index) { 
    return (_isModifier >> index)&1ULL; 
}


bool isActive(int row, int column) { 
    return (_realKeyboardState >> (column + row*KeyboardColumns))&1ULL; 
}

bool isActive(int index) { 
    return (_realKeyboardState >> index)&1ULL; 
}


ULL getKeyboard() {
    return _realKeyboardState;
}

ULL _ScanKeyboard() {
    ULL state = 0;
    for(int col=0; col<KeyboardColumns; col++) {
        digitalWrite(_cols[col], true);
        for(int row=0; row < KeyboardRows; row++) {
            if(digitalRead(_rowl[row])) state += (1ULL << (col + row*KeyboardColumns));
        }
        digitalWrite(_cols[col], false);
    }
    return state;
}

void IRAM_ATTR ISRscann() {
    ULL state = _ScanKeyboard();
    ULL realState = state & _lastKeyboardState;
    ULL xorState = _realKeyboardState ^ realState;
    _lastKeyboardState = state;
    for(int i = 0; i < KeyboardColumns*KeyboardRows; i++)
        if((xorState >> i)&1ULL){
            if((realState >> i)&1ULL){
                if(isButtonModifier(i)){
                    SystemEventQueue.push(event(1, i));
                    _modifierButton = i;
                }else SystemEventQueue.push(event(1, i, _modifierButton));
            }else{
                if(isButtonModifier(i)){
                    SystemEventQueue.push(event(0, i));
                    _modifierButton = -1;
                }else SystemEventQueue.push(event(0, i, _modifierButton));
            }
        }
    _realKeyboardState = realState;
}

void initKeyboard() {
    hw_timer_t * timer = NULL;
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &ISRscann, true);
    timerAlarmWrite(timer, 10000, true);
    timerAlarmEnable(timer);
}


String l0IndexToText[54] = {
    "layer0", "layer1", "layer2", "shift", "up", "backspace", 
    "ce", "", "", "left", "enter", "right", 
    "", "", "", "", "down", "exit", 
    "", "", "", "", "", "col", 
    "sin()", "cos()", "tan()", "ctg()", "e", "pi", 
    "^", "7", "8", "9", "+", "-", 
    "sqrt()", "4", "5", "6", "*", "=", 
    "log10()", "1", "2", "3", "(", ")", 
    "lg()", "ans", "0", "%", "ans", "ok"
};

String shiftedl0IndexToText[54] = {
    "layer0", "layer1", "layer2", "shift", "up", "backspace", 
    "ce", "ans", "", "left", "enter", "right", 
    "", "", "", "", "down", "exit", 
    "", "", "", "", "", "col", 
    "sin()", "cos()", "tan()", "ctg()", "e", "pi", 
    "^", "7", "8", "9", "+", "-", 
    "sqrt()", "4", "5", "6", "*", "=", 
    "log10()", "1", "2", "3", "(", ")", 
    "lg()", "", "0", "", "%", "ok"
};

String l1IndexToText[54] = {
    "layer0", "layer1", "layer2", "shift", "up", "backspace", 
    "ce", "!", "@", "left", "enter", "right", 
    "#", "$", "&", " ", "down", "exit", 
    "a", "b", "c", "d", "e", "f", 
    "g", "h", "i", "j", "k", "l", 
    "m", "n", "o", "p", "r", "s", 
    "t", "u", "v", "w", "x", "y", 
    "z", "nigger", ".", ",", "/", ";", 
    "()", "[]", "{}", "<>", "\\", "`"
};
    
String shiftedl1IndexToText[54] = {
    "layer0", "layer1", "layer2", "shift", "up", "backspace", 
    "ce", "!", "@", "left", "enter", "right", 
    "#", "$", "&", " ", "down", "exit", 
    "A", "B", "C", "D", "E", "F", 
    "G", "H", "I", "J", "K", "L", 
    "M", "N", "O", "P", "R", "S", 
    "T", "U", "V", "W", "X", "Y", 
    "Z", "NIGGER", ".", ",", "?",  
    "()", "[]", "{}", "<>", "|", "~"
};
