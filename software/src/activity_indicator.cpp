#include <activity_indicator.h>


const int ACTIVITY_LED_DELAY = 20;  // ms
unsigned long T_act;                // might need to move to main and make extern



void activityIndicatorSetup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    T_act = millis();
}



void showActivity() {
    if (millis() > (T_act + ACTIVITY_LED_DELAY)) {
        digitalWrite(LED_BUILTIN, HIGH);
        T_act = millis();
    };
}



void clearActivity() {
    if (millis() > (T_act + ACTIVITY_LED_DELAY)) {
        digitalWrite(LED_BUILTIN, LOW);
    }
}
