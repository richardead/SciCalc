#ifndef DZIURAWSAMOLOCIE_H
#define DZIURAWSAMOLOCIE_H

#include <Arduino.h>
#include "screen.h"
#include "keyboard.h"
#include "IR.h"


void dziuraWSamolocie() {
    IR_packet recordedVideo;
    recordedVideo.error = true;
    
    while(true) {
        if(!SystemEventQueue.empty()) {
            auto top = SystemEventQueue.front();
            SystemEventQueue.pop();
            
            if(top.type == false) {
                switch(top.button) {
                    case 17: // exit
                        return;
                        
                    case 9: // record
                        recordedVideo = recordIR();
                        
                        // Tylko wynik po nagraniu
                        display.fillScreen(ILI9341_BLACK);
                        display.setCursor(10, 10);
                        if(!recordedVideo.error) {
                            display.print("Nagrano");
                        } else {
                            display.print("Zdrajca metalu");
                        }
                        delay(2000);
                        break;
                        
                    case 11: // play
                        if(!recordedVideo.error) {
                            sendIR(recordedVideo);
                            
                            // Tylko wynik po odtworzeniu
                            display.fillScreen(ILI9341_BLACK);
                            display.setCursor(10, 10);
                            display.print("Odtworzono");
                            delay(2000);
                        }
                        break;
                }
            }
        }
        delay(10);
    }
}

#endif