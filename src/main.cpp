#include <Arduino.h>
#include <SPIFFS.h>
#include "buzzer.h"
#include "IO.h"
#include "screen.h"
#include "keyboard.h"
#include "apps/menu.h"


void setup() {
  Serial.begin(115200);

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  if(!psramFound()) {
    Serial.println("PSRAM is NOT available!");
    return;
  }
    
  setupIO();
  initScreen();
  initKeyboard();
  menu();


  
}

// String inputString = "";   // a String to hold incoming data
// bool inputComplete = false;

void loop() {
  // Serial.println("hello, world");
  // delay(500);
  // while (Serial.available()) {
  //   char inChar = (char)Serial.read();
  //   if (inChar == '\n') {
  //     inputComplete = true;
  //     break;
  //   } else {
  //     inputString += inChar;
  //   }
  // }

  // if (inputComplete) {
  //   Serial.println(te_interp(inputString.c_str(), 0));

  //   inputString = "";
  //   inputComplete = false;
  // }

}






