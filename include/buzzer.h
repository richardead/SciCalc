#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "IO.h"

#define BUZZER_LEDC_CH 0
#define BUZZER_TIMER_CH 1

void IRAM_ATTR stopPlaying();
void playSample(uint8_t *samples, int sampleCount, int sampleRate);

#endif