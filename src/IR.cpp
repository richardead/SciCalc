#include <Arduino.h>
#include <vector>
#include "IR.h"
#include "IO.h"

const size_t changesSize = 256;
volatile size_t changesIdx = 0;
volatile unsigned long changes[changesSize];

void IRAM_ATTR ISRchange() {
    if(changesIdx < changesSize) {
        changes[changesIdx++] = micros();
    }
}

IR_packet recordIR() {
    IR_packet parsed;

    changesIdx = 0;
    while(!digitalRead(IR_RECEIVER)) delay(1);

    attachInterrupt(digitalPinToInterrupt(IR_RECEIVER), &ISRchange, CHANGE);

    unsigned long start = micros();
    while(true) {
        size_t lastChangesIdx = changesIdx;

        if(lastChangesIdx > 0) {
            if(micros()-changes[lastChangesIdx-1] > 100000UL) break;
        } else {
            if(micros()-start > 5000000UL) break;
        }
    }

    detachInterrupt(digitalPinToInterrupt(IR_RECEIVER));

    Serial.println(changesIdx - 1);

    if(changesIdx < 6 || changesIdx&1) return parsed;

    parsed.startT = changes[1] - changes[0];
    parsed.pauseT = changes[2] - changes[1];
    parsed.bits = (changesIdx-4)/2; // assume perfect transmission

    unsigned long long waitSum = 0, zeroSum = 0, oneSum = 0;
    unsigned short zeros=0, ones=0;
    std::vector<unsigned long> delays;

    for(int i=2; i<changesIdx-2; i+=2) {
        waitSum += changes[i+1] - changes[i];
        delays.push_back(changes[i+2] -   changes[i+1]);
    }

    parsed.waitT = waitSum / parsed.bits;

    unsigned long minn = ULONG_MAX, maxx = 0;
    for(int i=0; i<delays.size(); i++) {
        minn = min(minn, delays[i]);
        maxx = max(maxx, delays[i]);
    }

    unsigned long mid = (minn + maxx) / 2;

    for(int i=delays.size()-1; i>=0; i--) {
        parsed.data <<= 1;
        parsed.data |= (delays[i] > mid);

        if(delays[i] > mid) {
            ones++;
            oneSum += delays[i];
        } else {
            zeros++;
            zeroSum +=  delays[i];
        }
    }

    parsed.oneT = oneSum / ones;
    parsed.zeroT = zeroSum / zeros;
    parsed.error = false;

    return parsed;
}

void sendIR(IR_packet vid) {
    if(vid.error) return;

    ledcSetup(IR_LEDC_CH, 38000, 8);
    ledcAttachPin(IR_DIODE, IR_LEDC_CH);

    ledcWrite(IR_LEDC_CH, 128);
    delayMicroseconds(vid.startT);
    ledcWrite(IR_LEDC_CH, 0);

    delayMicroseconds(vid.pauseT);

    for(int i=0; i<vid.bits; i++) {
        ledcWrite(IR_LEDC_CH, 128);
        delayMicroseconds(vid.waitT);
        ledcWrite(IR_LEDC_CH, 0);
        delayMicroseconds(((vid.data>>i)&1)?vid.oneT:vid.zeroT);
    }

    ledcWrite(IR_LEDC_CH, 128);
    delayMicroseconds(vid.waitT);
    ledcWrite(IR_LEDC_CH, 0);

    ledcDetachPin(IR_DIODE);
}