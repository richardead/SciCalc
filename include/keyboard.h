#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>
#include "IO.h"
#include "light_containers.h"

#define KeyboardRows 9
#define KeyboardColumns 6

typedef unsigned long long ULL;


struct event{
    bool type; // true if KeyDown    false if KeyUp
    int button; // button id
    int modifier = -1;
    event() : type(0), button(0), modifier(-1){}
    event(bool type, int index) : type(type), button(index), modifier(-1){}
    event(bool type, int index, int modifierIndex) : type(type), button(index), modifier(modifierIndex){}
    int get_x(){ return button%KeyboardColumns;}
    int get_y(){ return button/KeyboardRows;}
    int get_index(){ return button; }
    bool has_modifier(){ return modifier != -1;}
    int get_modifier(){ return modifier; }
};

//ten napis zostal napisany o 21:37 2 kwietnia 2025 :)
extern light_queue<event> SystemEventQueue;

extern String l0IndexToText[54];
extern String shiftedl0IndexToText[54];
extern String l1IndexToText[54];
extern String shiftedl1IndexToText[54];

bool isButtonModifier(int row, int column);
bool isButtonModifier(int index);

bool isButtonActive(int row, int column);
bool isButtonActive(int index);

ULL getKeyboard();

void initKeyboard();

#endif