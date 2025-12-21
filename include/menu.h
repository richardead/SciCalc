#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <SPIFFS.h>
#include "buzzer.h"
#include "IO.h"
#include "screen.h"
#include "keyboard.h"
#include <vector>

class Menu {
private:
    bool infinite;

    int rowsOnScreen = 0;
    int menuItemCount = 0;

    int currentSelection = 0;
    int highestSelectionIndex = 0;
    int prevHighestSelectionIndex = 0;

    std::vector<String> menuItems;
    std::vector<void (*)()> appFunctions;

public:
    Menu(bool infinite) : infinite(infinite) {}

    void addOption(String name, void (*appFunc)()) {
        menuItems.push_back(name);
        appFunctions.push_back(appFunc);

        menuItemCount++;
        rowsOnScreen = min(menuItemCount, 9);
    }

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

    void run() {
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

                if(top.type == false && top.button == 10) {
                    SystemEventQueue.pop();

                    appFunctions[currentSelection]();
                    
                    display.fillScreen(ILI9341_BLACK);
                    if(infinite) {
                        updateMenu();
                    } else {
                        break;
                    }
                }

                // Serial.print(top.type?"KeyDown ":"KeyUp ");
                // Serial.println(top.button);

                SystemEventQueue.pop();
            }
            
            delay(10);
        }
    }
};

#endif