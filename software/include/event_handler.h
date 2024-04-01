#pragma once
#include <Arduino.h>


#define PS2_BREAK   0x01
#define PS2_EXT     0x02


struct EventData {
    bool break_state = false;       // received 0xF0 byte
    bool extended_state = false;    // received 0xE0 byte
    bool weird_state = false;       // received 0xE1 byte
    byte scancode = 0x0;
};


EventData parseScanCode(void);
void handleEvent(EventData);
