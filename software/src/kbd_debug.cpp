#include "kbd_debug.h"



void debugSetup() {
    Serial.begin(9600);
    Serial.println("Keyboard Test:");
}


void hexprint(byte b) {
    Serial.print("0x");
    Serial.print(b < 16 ? "0" : "");
    Serial.print(b, HEX);
    // Serial.print(" ");
}


void keyDebug(EventData evt) {
    Serial.print("ext: ");
    Serial.print(evt.extended_state);
    Serial.print(" break: ");
    Serial.print(evt.break_state);
    Serial.print(" code: ");
    // Serial.print(evt.scancode, DEC);
    hexprint(evt.scancode);
    Serial.println();
}
