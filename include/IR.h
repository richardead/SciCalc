#ifndef IR_H
#define IR_h

#include <Arduino.h>

#define IR_LEDC_CH 1

struct IR_packet {
    unsigned long long data = 0ULL;
    unsigned long startT, pauseT, waitT, zeroT, oneT;
    unsigned short bits = 0;
    bool error = true;
};

IR_packet recordIR();

void sendIR(IR_packet msg);

#endif