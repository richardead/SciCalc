#ifndef IO_H
#define IO_H

#define KEYPAD_ROW_0    41
#define KEYPAD_ROW_1    42
#define KEYPAD_ROW_2     3
#define KEYPAD_ROW_3    46
#define KEYPAD_ROW_4    21
#define KEYPAD_ROW_5    47
#define KEYPAD_ROW_6    48
#define KEYPAD_ROW_7    45
#define KEYPAD_ROW_8    38

#define KEYPAD_COL_5     5
#define KEYPAD_COL_0    17
#define KEYPAD_COL_1    16
#define KEYPAD_COL_2    15
#define KEYPAD_COL_3     7
#define KEYPAD_COL_4     6

#define IR_DIODE         8
#define IR_RECEIVER     18
#define BAT_REFF         4

#define DISPLAY_SCK     12
#define DISPLAY_SDA     11
#define DISPLAY_RST      9
#define DISPLAY_DC      13
#define DISPLAY_CS      10

#define SERIAL_1_TX     40
#define SERIAL_1_RX     39

#define BUZZER_IO        2

void setupIO();

#endif
