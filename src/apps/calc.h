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

void displayGraph(double top, double bottom, double left, double right) {
    display.fillScreen(ILI9341_BLACK);

    double Ydiff = top - bottom, Xdiff = right - left;

    if(top > 0 && bottom < 0) {
        double pos = top / Ydiff;
        int y = pos * 240;
        display.drawLine(0, y, 320, y, ILI9341_WHITE);
    }

    if(left < 0 && right > 0) {
        double pos = right / Xdiff;
        int x = pos * 320;
        display.drawLine(x, 0, x, 240, ILI9341_WHITE);
    }

    double x, step = (right - left) / 320.0;
    te_variable vars[] = {{"x", &x}};
    te_expr *expr = te_compile(textInput.c_str(), vars, 1, 0);

    for(x = (left + step/2); x < (right - step/2); x += step) {
        double y = te_eval(expr);
        double X = (x - left) / Xdiff, Y = (y - bottom) / Ydiff;
        int realX = (1.00-X) * 320, realY = (1.00-Y) * 240;
        display.drawPixel(realX, realY, ILI9341_CYAN);
    }
    
    te_free(expr);
}

void graph(double top, double bottom, double left, double right) {
    while(true) {
        double scale = (top-bottom) / 240;
        if(!SystemEventQueue.empty()) {
            auto front = SystemEventQueue.front();
            if(front.type == false) {
                switch (front.button) {
                    case 17: // exit
                        return;
                        break;
                    case 4: // up
                        top += scale * 24;
                        bottom += scale * 24;
                        break;
                    case 9: // left
                        left += scale * 32;
                        right += scale * 32;
                        break;
                    case 11: // right
                        left -= scale * 32;
                        right -= scale * 32;
                        break;
                    case 16: // down
                        top -= scale * 24;
                        bottom -= scale * 24;
                        break;
                    case 34: // zoom in
                        top /= 10;
                        bottom /= 10; 
                        left /= 10;
                        right /= 10;
                        break;
                    case 35: // zoom out
                        top *= 10;
                        bottom *= 10; 
                        left *= 10;
                        right *= 10;
                        break;
                }
                
                displayGraph(top, bottom, left, right);
            }
        }
        SystemEventQueue.pop();
        delay(30);
    }
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

                    case 10: // calculate
                        display.setCursor(10, 215);
                        display.print(String("= ") + String(te_interp(textInput.c_str(), 0)));
                        break;
                    
                    case 0:
                        layer = 0;
                        break;

                    case 1:
                        layer = 1;
                        break;
                    
                    case 7:
                        graph(1, -1, -1, 1);
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