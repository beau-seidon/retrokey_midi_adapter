#include <event_handler.h>
#include <event_tables.h>
#include <midi_handler.h>
#include <PS2Keyboard.h>


byte note_state[128] = {0};
byte cc_state[128] = {0};

byte cc_num = 0;
byte cc_val = 0;

extern PS2Keyboard keyboard;
extern int transpose;


EventData parseScanCode(void) {
    EventData event;

	static byte state = 0x0;
	byte scan_code;

	while (1) {
		scan_code = keyboard.readScanCode();
		if (!scan_code) {
            event.break_state = false;
            event.extended_state = false;
            event.scancode = 0x0;
            return event;
        }

		if (scan_code == 0xF0) {
			state |= PS2_BREAK;
		} else if (scan_code == 0xE0) {
			state |= PS2_EXT;
		} else {
            event.break_state = (state & PS2_BREAK);
            event.extended_state = (state & PS2_EXT);
            event.scancode = scan_code;

			state &= ~(PS2_BREAK | PS2_EXT);
			return event;
		}
	}
}



void handleEvent(EventData event) {
    int index = (int)event.scancode;
    int ps2_ext = event.extended_state;
    int ps2_break = event.break_state;

    int table;
    if (ps2_break && ps2_ext) {
        table = 3;
    } else if (ps2_break) {
        table = 2; 
    } else if (ps2_ext) {
        table = 1;
    } else {
        table = 0;
    }


    int action = scancode_to_event[table][index][0];
    int data = scancode_to_event[table][index][1];


    switch(action) { 

        case(MIDI_NOTE_ON):
            if (note_state[data] == 0) { 
                note_state[data] = 1;

                midiNoteOn((byte)data + (byte)transpose);

                // Serial.print("MIDI_NOTE_ON, ");
                // Serial.print(data);
                // Serial.println();
            }
            break;  


        case(MIDI_NOTE_OFF):
            if (note_state[data] == 1) { 
                note_state[data] = 0;

                midiNoteOff((byte)data + (byte)transpose);

                // Serial.print("MIDI_NOTE_OFF, ");
                // Serial.print(data);
                // Serial.println();
            }
            break;


        case(ALL_NOTES_OFF):
            cc_num = 0x7B;  // 123
            cc_val = 0x0;
            midiSendCC(cc_num, cc_val);

            for (byte i = 0; i < 128; i++) {
                midiNoteOff((byte)i);
                note_state[i]= 0;
            }

            // Serial.print("ALL_NOTES_OFF, ");
            // Serial.print(data);
            // Serial.println();
            break;


        case(MIDI_CC_TOGGLE):
            cc_num = (data & 0xFF00) >> 8;
            cc_val = (data & 0x7F);

            if (cc_state[cc_num] == 0 && cc_val > 63) {
                cc_state[cc_num] = 1;
                midiSendCC(cc_num, cc_val);

                // Serial.print("MIDI_CC_TOGGLE, ");
                // Serial.print(cc_num);
                // Serial.print(", ");
                // Serial.print(cc_val);
                // Serial.println();                
            }

            if (cc_state[cc_num] == 1 && cc_val < 64) {
                cc_state[cc_num] = 0;
                midiSendCC(cc_num, cc_val);

                // Serial.print("MIDI_CC_TOGGLE, ");
                // Serial.print(cc_num);
                // Serial.print(", ");
                // Serial.print(cc_val);
                // Serial.println();
            }
            break;


        case(MIDI_CC_ADJUST):
            // Serial.print("MIDI_CC_ADJUST, ");
            // Serial.print(data);
            // Serial.println();
            break;


        case(MIDI_PITCHBEND):
            setPitchBendDirection(data);

            // Serial.print("MIDI_PITCHBEND, ");
            // Serial.print(data);
            // Serial.println();
            break;      


        case(TRANSPOSE):
            setTranspose(data);

            // Serial.print("TRANSPOSE, ");
            // Serial.print(data);
            // Serial.println();
            break;     


        case(CHANGE_VELOCITY):
            setVelocity(data);

            // Serial.print("CHANGE_VELOCITY, ");
            // Serial.print(data);
            // Serial.println();
            break;


        case(CHANGE_CHANNEL):
            // Serial.print("CHANGE_CHANNEL, ");
            // Serial.print(data);
            // Serial.println();
            break;


        case(FULL_RESET):
            for (byte i = 0; i < 128; i++) {
                midiNoteOff((byte)i);
                note_state[i] = 0;
                cc_state[i] = 0;
            }
            midiParamInit();

            // Serial.print("FULL_RESET, ");
            // Serial.print(data);
            // Serial.println();
            break;


        case(DO_NOTHING):
            Serial.print("DO_NOTHING, ");
            Serial.print(data);
            Serial.println();
            break;     


        case(UNUSED):
            Serial.print("UNUSED, ");
            Serial.print(data);
            Serial.println();
            break;      


        default:
            Serial.print("OTHER, ");
            Serial.print(data);
            Serial.println();
            break;
    }
}
