#include <midi_handler.h>


byte midi_channel;
byte velocity;

int bend;
int last_bend;
int bend_direction;
unsigned long T_pb;

int transpose;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, SerialMIDI);



void midiSetup() {
    SerialMIDI.begin();
    midiParamInit();
}



void midiParamInit() {
    midi_channel = CHANNEL_DEFAULT;
    velocity = VELOCITY_DEFAULT;
    bend = 0;
    last_bend = bend;
    bend_direction = 0;
    T_pb = millis();
    transpose = 0;
}



void midiNoteOn(byte note) {
    SerialMIDI.sendNoteOn(note, velocity, midi_channel);
}



void midiNoteOff(byte note) {
    SerialMIDI.sendNoteOff(note, SILENCE, midi_channel);
}



void midiSendCC(byte cc_num, byte cc_val) {
    SerialMIDI.sendControlChange(cc_num, cc_val, midi_channel);
}



void midiPitchBend(int bend) {
    SerialMIDI.sendPitchBend(bend, midi_channel);
}



void setPitchBendDirection(int new_direction) {
    if (bend_direction == 0) {
        bend_direction = new_direction;
    } else if (new_direction == 0) {
        bend_direction = 0;
    }
}



void handlePBGlide() {
    if (bend_direction != 0) {
        if (MIDI_PITCHBEND_MIN < bend && bend < MIDI_PITCHBEND_MAX) {
            if (millis() > T_pb + BEND_REFRESH_PERIOD) {
                bend += (bend_direction * BEND_RATE * BEND_REFRESH_PERIOD);
                T_pb = millis();
            }
        }
        if (bend > MIDI_PITCHBEND_MAX) bend = MIDI_PITCHBEND_MAX;
        if (bend < MIDI_PITCHBEND_MIN) bend = MIDI_PITCHBEND_MIN;

    } else bend = 0;
    if (bend != last_bend) {
        midiPitchBend(bend);
        last_bend = bend;
    }
}



void setTranspose(int adjust) {
    transpose += adjust;
    if (transpose > TRANSPOSE_MAX) transpose = TRANSPOSE_MAX;
    if (transpose < TRANSPOSE_MIN) transpose = TRANSPOSE_MIN;
}



void setVelocity(int adjust) {
    velocity += adjust;
    if (velocity > VELOCITY_MAX) velocity = VELOCITY_MAX;     
    if (velocity < VELOCITY_MIN) velocity = VELOCITY_MIN;
}



void setChannel(int adjust) {
    midi_channel += adjust;
    if (midi_channel > CHANNEL_MAX) midi_channel = CHANNEL_MAX;     
    if (midi_channel < CHANNEL_MIN) midi_channel = CHANNEL_MIN;
}
