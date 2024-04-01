#pragma once
#include <Arduino.h>
#include <MIDI.h>


const int CHANNEL_DEFAULT = 1;
const int CHANNEL_MAX = 16;
const int CHANNEL_MIN = 1;

const int VELOCITY_DEFAULT = 101;
const int VELOCITY_MAX = 127;
const int VELOCITY_MIN = 1;
const int SILENCE = 0;

const int BEND_DEFAULT = 0;             // 0x2000
const int BEND_REFRESH_PERIOD = 1;      // [msec]
const double BEND_DURATION = 0.033;     // [sec]
const int BEND_RATE = ((MIDI_PITCHBEND_MAX - MIDI_PITCHBEND_MIN) / 2) / (BEND_DURATION * 1000);        // [step/msec]

const int NOTE_LOW_DEFAULT = 46;
const int NOTE_HIGH_DEFAULT = 79;
const int TRANSPOSE_MAX = 127 - NOTE_HIGH_DEFAULT;
const int TRANSPOSE_MIN = 0 - NOTE_LOW_DEFAULT;


void midiSetup();
void midiParamInit();

void midiNoteOn(byte);
void midiNoteOff(byte);

void midiSendCC(byte, byte);

void midiPitchBend(int);
void setPitchBendDirection(int);
void handlePBGlide();

void setTranspose(int);

void setVelocity(int);

void setChannel(int);
