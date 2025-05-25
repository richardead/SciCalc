#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "IO.h"

typedef unsigned long UL;

void playSample(char * samples, int sampleCount, int sampleRate) {
    ledcSetup(0, 2UL*(UL)sampleRate, 8);
    ledcAttachPin(BUZZER_IO, 0);
    int sampleTime = 1000000 / sampleRate;

    UL start = micros();

    for (UL i = 0; i < (UL)sampleCount; i++) {
        ledcWrite(0, (uint8_t)(samples[i]/2));
        delayMicroseconds(max((i+1UL)*sampleTime-(micros() - start), 0UL));
    }
}

#endif