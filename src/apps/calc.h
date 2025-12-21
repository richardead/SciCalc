#ifndef CALC_H
#define CALC_H

#include <Arduino.h>
#include <SPIFFS.h>
#include "buzzer.h"
#include "IO.h"
#include "screen.h"
#include "keyboard.h"
#include <cmath>
extern "C" {
  #include "tinyexpr.h"
}

String textInput = "";
int cursorPosition = 0;
int layer = 0;

void updateCalc() {
    display.fillScreen(ILI9341_BLACK);
    display.setCursor(0, 10);
    display.setTextColor(ILI9341_WHITE);
    display.print(textInput);

    display.drawRect((cursorPosition*12)%312, 10+15*(cursorPosition/26), 2, 15, ILI9341_YELLOW);

}

void calc() {
    updateCalc();
    while(true) {
        if(!SystemEventQueue.empty()) {
            auto top = SystemEventQueue.front();
            if(top.type == false) {
                switch(top.button) {
                    case 17: // exit
                        return;
                        break;

                    case 6: // ce
                        cursorPosition = 0;
                        textInput = "";
                        updateCalc();
                        break;

                    case 5: // backspace
                        textInput = textInput.substring(0, max(0, cursorPosition-1)) + textInput.substring(cursorPosition, textInput.length());
                        cursorPosition = max(0, cursorPosition-1);
                        updateCalc();
                        break;

                    case 11: // right
                        cursorPosition = min(cursorPosition+1, (int)textInput.length());
                        updateCalc();
                        break;

                    case 9: // left
                        cursorPosition = max(0, cursorPosition-1);
                        updateCalc();
                        break;

                    case 10:
                        display.setCursor(10, 215);
                        display.print(String("= ") + String(te_interp(textInput.c_str(), 0)));
                        break;
                    
                    case 0:
                        layer = 0;
                        break;

                    case 1:
                        layer = 1;
                        break;

                    default:
                        switch(layer) {
                            case 0:
                                switch(top.has_modifier()) {
                                    case 0:
                                        textInput = textInput.substring(0, cursorPosition) + l0IndexToText[top.button] + textInput.substring(cursorPosition, textInput.length()); 
                                        cursorPosition += l0IndexToText[top.button].length();
                                        break;
                                    case 1:
                                        textInput = textInput.substring(0, cursorPosition) + shiftedl0IndexToText[top.button] + textInput.substring(cursorPosition, textInput.length()); 
                                        cursorPosition += shiftedl0IndexToText[top.button].length();
                                        break;
                                }
                                break;
                            case 1:
                                switch(top.has_modifier()) {
                                    case 0:
                                        textInput = textInput.substring(0, cursorPosition) + l1IndexToText[top.button] + textInput.substring(cursorPosition, textInput.length()); 
                                        cursorPosition += l1IndexToText[top.button].length();
                                        break;
                                    case 1:
                                        textInput = textInput.substring(0, cursorPosition) + shiftedl1IndexToText[top.button] + textInput.substring(cursorPosition, textInput.length()); 
                                        cursorPosition += shiftedl1IndexToText[top.button].length();
                                        break;
                                }
                                break;
                        }
                        
                        updateCalc();
                        break;
                }
            }

        }
        SystemEventQueue.pop();
        delay(10);
    }
    
}

#endif