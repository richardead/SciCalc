#ifndef MANDELBROT_H
#define MANDELBROT_H

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

const int16_t
  bits        = 20,   // Fractional resolution
  pixelWidth  = 320,  // TFT dimensions
  pixelHeight = 240,
  iterations  = 256;  // Fractal iteration limit or 'dwell'
float
  centerReal  = -0.6, // Image center point in complex plane
  centerImag  =  0.0,
  rangeReal   =  3.0, // Image coverage in complex plane
  rangeImag   =  3.0;

#if defined(USE_BUFFER)
  uint16_t buffer[pixelWidth * pixelHeight];
#endif

int64_t       n, a, b, a2, b2, posReal, posImag;
uint32_t      startTime,elapsedTime;


int32_t
  startReal   = (int64_t)((centerReal - rangeReal * 0.5)   * (float)(1 << bits)),
  startImag   = (int64_t)((centerImag + rangeImag * 0.5)   * (float)(1 << bits)),
  incReal     = (int64_t)((rangeReal / (float)pixelWidth)  * (float)(1 << bits)),
  incImag     = (int64_t)((rangeImag / (float)pixelHeight) * (float)(1 << bits));

void mandelbrot() {
  startTime = millis();
  posImag = startImag;
  for (int y = 0; y < pixelHeight; y++) {
    posReal = startReal;
    for (int x = 0; x < pixelWidth; x++) {
      a = posReal;
      b = posImag;
      for (n = iterations; n > 0 ; n--) {
        a2 = (a * a) >> bits;
        b2 = (b * b) >> bits;
        if ((a2 + b2) >= (4 << bits)) 
          break;
        b  = posImag + ((a * b) >> (bits - 1));
        a  = posReal + a2 - b2;
      }
      #if defined(USE_BUFFER)
        buffer[y * pixelWidth + x] = (n * 29)<<8 | (n * 67);
      #else
        display.drawPixel(x, y, (n * 29)<<8 | (n * 67)); // takes 500ms with individual pixel writes
      #endif
      posReal += incReal;
    }
    posImag -= incImag;
  }
  #if defined(USE_BUFFER)
    display.drawRGBBitmap(0, 0, buffer, pixelWidth, pixelHeight); // takes 169 ms
  #endif
  elapsedTime = millis()-startTime;

  rangeReal *= 0.95;
  rangeImag *= 0.95;

  while(SystemEventQueue.empty()) delay(100);
}

#endif