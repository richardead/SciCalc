#ifndef BADAPPLE_H
#define BADAPPLE_H

#include <Arduino.h>
#include <SPIFFS.h>
#include "buzzer.h"
#include "IO.h"
#include "screen.h"
#include "keyboard.h"
#include <driver/spi_master.h>

#define WAV_HEADER_LENGTH 44UL

void badApple() {
  display.fillScreen(0x18CF); // reverse of ILI9341_PINK
  display.setCursor(69, 69);
  display.setTextColor(ILI9341_WHITE);
  display.print("LOADING..");
  // display.fillCircle(169, 169, 69, ILI9341_RED);

  File file = SPIFFS.open("/badapple.bin");
  // File audio = SPIFFS.open("/BadApple.wav", "r");

  if(!file) {
    Serial.println("Failed to open file");
    return;
  }

  // if (!audio) {
  //   Serial.println("Failed to open file");
  //   return;
  // }

  // if (audio.size() <= WAV_HEADER_LENGTH) {
  //   Serial.println("File too small");
  //   return;
  // }

  // audio.seek(WAV_HEADER_LENGTH); // skip header
  // size_t size = audio.size() - WAV_HEADER_LENGTH;

  // uint8_t *soundBuffer = (uint8_t*)ps_malloc(size);
  // if (!soundBuffer) {
  //   Serial.println("Allocation failed");
  //   return;
  // }

  // audio.read(soundBuffer, size);
  // audio.close();

  
  uint16_t *frameBuffer = (uint16_t*)malloc(320*240*sizeof(uint16_t));
  for(int i=0; i<240*320; i++) frameBuffer[i] = ILI9341_BLACK;

  display.setRotation(1);
  // playSample(soundBuffer, size, 24000); // play bad apple audio

  uint8_t frameLengthBuf[4];
  uint32_t frameLength;

  uint32_t startTime = micros();
  uint32_t frames = 0;
  uint32_t FPS = 30;

  while(file.available()) {
    file.read(frameLengthBuf, 4);

    frameLength = (
      (uint32_t)frameLengthBuf[0] << 24 |
      (uint32_t)frameLengthBuf[1] << 16 |
      (uint32_t)frameLengthBuf[2] << 8  |
      (uint32_t)frameLengthBuf[3]
    );

    uint8_t stripes[frameLength];
    file.read(stripes, frameLength);

    uint32_t count = 0;
    uint8_t rowCount = 0;
    bool state = false;

    for(uint32_t i=0; i < frameLength; i++) {
      for(uint8_t j=0; j < stripes[i]; j++) frameBuffer[count++] = state?ILI9341_WHITE:ILI9341_BLACK;
      rowCount += stripes[i];
      state = !state;

      if(rowCount >= 240) {
        state = false;
        rowCount = 0;
      }
    }
    uint32_t st = micros();
    // display.drawRGBBitmap(0, 0, frameBuffer, 240, 320);
    display.startWrite();
    display.setAddrWindow(0, 0, 240, 320);
    display.writePixels(frameBuffer, 320*240); // about 1% faster than display.drawRGBBitmap()
    display.endWrite();

    // Serial.print("Frame took ");
    // Serial.print(micros()-st);
    // Serial.println("us");
    frames++;
     
    uint32_t expectedTime = frames * (1000000UL / FPS);
    uint32_t realTime = micros() - startTime;

    if(expectedTime > realTime) delayMicroseconds(expectedTime - realTime);
  
    // free(stripes);
    if(!SystemEventQueue.empty()) break;
  }


  // stopPlaying();
  file.close();
  free(frameBuffer);
  // free(soundBuffer);
  display.setRotation(2);
}
#endif