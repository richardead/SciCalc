#include <Arduino.h>
#include "IO.h"
#include "buzzer.h"

typedef unsigned long UL;

hw_timer_t *sampleTimer = NULL;

uint8_t *gSamples = nullptr;
volatile uint32_t gSampleCount = 0;
volatile uint32_t gSampleIndex = 0;

void IRAM_ATTR stopPlaying() {
    timerAlarmDisable(sampleTimer);
    ledcWrite(BUZZER_LEDC_CH, 0);
}

void IRAM_ATTR onSampleTimer() {
  if(gSampleIndex < gSampleCount) {
    ledcWrite(BUZZER_LEDC_CH, gSamples[gSampleIndex++]);
  } else {
    stopPlaying();
  }
}

void playSample(uint8_t *samples, int sampleCount, int sampleRate) {
  gSamples = samples;
  gSampleCount = sampleCount;
  gSampleIndex = 0;

  ledcSetup(BUZZER_LEDC_CH, 2UL * (unsigned long)sampleRate, 8);
  ledcAttachPin(BUZZER_IO, BUZZER_LEDC_CH);

  if(!sampleTimer) {
    sampleTimer = timerBegin(BUZZER_TIMER_CH, 2, true);
    timerAttachInterrupt(sampleTimer, &onSampleTimer, true);
  }

  timerAlarmWrite(sampleTimer, 40000000UL / sampleRate, true); // 80Mhz timer clock
  timerAlarmEnable(sampleTimer);
}


// void playSample(uint8_t * samples, int sampleCount, int sampleRate) {
//     ledcSetup(0, 2UL*(UL)sampleRate, 8);
//     ledcAttachPin(BUZZER_IO, 0);
//     int sampleTime = 1000000 / sampleRate;

//     UL start = micros();

//     for (UL i = 0; i < (UL)sampleCount; i++) {
//         ledcWrite(0, (uint8_t)(samples[i]));
//         delayMicroseconds(max((i+1UL)*sampleTime-(micros() - start), 0UL));
//     }
// }


// void initBuzzer() {
//     hw_timer_t * timer = NULL;
//     timer = timerBegin(0, 1, true);
//     timerAttachInterrupt(timer, &playSample, true);
//     timerAlarmWrite(timer, 3333, true);
//     timerAlarmEnable(timer);
// }