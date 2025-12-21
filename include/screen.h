#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <SPI.h>
#include "IO.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <driver/spi_master.h>
#include "driver/spi_common.h"

SPIClass *spi = new SPIClass(0);
Adafruit_ILI9341 display = Adafruit_ILI9341(spi, DISPLAY_DC, DISPLAY_CS, DISPLAY_RST);

void initScreen() {
    spi->begin();
    spi->setFrequency(80000000);
    display.begin(80000000);
    display.fillScreen(ILI9341_BLACK);
    display.setRotation(2);
    display.setTextColor(ILI9341_WHITE);
    display.setTextSize(2);
}

void displayText(const char* text, int x, int y, uint16_t color) {
    display.setCursor(x, y);
    display.print(text);
}

void displayRectangle(int x, int y, int w, int h, uint16_t color) {
     display.drawRect(x, y, w, h, color);
}

#endif