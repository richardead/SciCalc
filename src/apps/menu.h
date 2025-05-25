#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <SPIFFS.h>
#include "buzzer.h"
#include "IO.h"
#include "screen.h"
#include "keyboard.h"
#include "apps/calc.h"

int currentSelection = 0;
int highestSelectionIndex = 0;
int prevHighestSelectionIndex = 0;
const int rowsOnScreen = 9;
const char* menuItems[] = {"Calculator", "Notes", "Images", "Games", "Skibidi", "Sowinska simulator", "Franek simulator", "womp womp", "Linux (real)", "DOOM (real)"};
const int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);

void updateMenu() {
    // scrolling up
    if(currentSelection < highestSelectionIndex) {
        highestSelectionIndex = currentSelection;
    }
    // scrolling down
    if(currentSelection > highestSelectionIndex + rowsOnScreen - 1) {
        highestSelectionIndex++;
    }

    display.setTextSize(2);
    display.fillScreen(ILI9341_BLACK);
    for(int i = 0; i < rowsOnScreen; i++) {
        display.setCursor(10, 15 + i * 25);

        if(i+highestSelectionIndex == currentSelection) {
            display.setTextColor(ILI9341_YELLOW);
        } else {
            display.setTextColor(ILI9341_WHITE);
        }
        
        display.print(menuItems[i+highestSelectionIndex]);
    }

    prevHighestSelectionIndex = highestSelectionIndex;
}

void menu() {
    Serial.begin(115200);
    updateMenu();

    while(true) {
        if(!SystemEventQueue.empty()) {
            auto top = SystemEventQueue.front();

            if(top.type == false && top.button == 16) {
                currentSelection = min(menuItemCount-1, currentSelection+1);
                updateMenu();
            }

            if(top.type == false && top.button == 4) {
                currentSelection = max(0, currentSelection-1);
                updateMenu();
            }

            if (top.type == false && top.button == 10) {
                SystemEventQueue.pop();
                switch(currentSelection) {
                case 0:
                    calc();
                    break;
                
                default:
                    break;
                }
                
                display.fillScreen(ILI9341_BLACK);
                updateMenu();
            }

            // Serial.print(top.type?"KeyDown ":"KeyUp ");
            // Serial.println(top.button);

            SystemEventQueue.pop();
        }
        
        delay(10);
    }
    
}

#endif