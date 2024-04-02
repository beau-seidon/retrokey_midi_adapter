#include <Arduino.h>
#include <PS2Keyboard.h>

#include "activity_indicator.h"
#include "event_handler.h"
#include "midi_handler.h"
#include "kbd_debug.h"



const int DataPin = 5;
const int IRQpin = 18;

PS2Keyboard keyboard;
EventData ps2event;


void setup() {
    delay(1000);

    activityIndicatorSetup();
    keyboard.begin(DataPin, IRQpin);
    midiSetup();
    debugSetup();
}


void loop() {
    // push space bar to start
    if (keyboard.available()) {
        ps2event = parseScanCode();

        if (ps2event.scancode) {
            showActivity();
            // keyDebug(ps2event);
            handleEvent(ps2event);
        }
        handlePBGlide();
    }
    clearActivity();
}
