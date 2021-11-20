//You must select MIDI from the "Tools > USB Type" menu
//   http://www.pjrc.com/teensy/td_midi.html

const int QUANTIZATION = 50; //Analog potentiometer quantization. Lower means more values but less give
const int OPEN_VAL = 20;   // Value to "snap" to for open line. Needed to accomodate for hardware issues
const int MIDI_OFFSET = 55;
const int controller_A0 = 16; //MIDI Controller channel
const int channel = 1;        //MIDI channel number to send messages on
  
int prev_A0 = 0;              //Store previous value, to detect changes
int quantized_A0 = 0;        //Measured value, quantized

elapsedMillis msec = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (msec >= 100) { //Only check at 50Hz
    msec = 0;
    int val_A0 = analogRead(A0);
    if (val_A0 < 10 || val_A0 > 1000) { 
      quantized_A0 = OPEN_VAL; 
    } else { 
      quantized_A0 = val_A0 / QUANTIZATION;
    }
    quantized_A0 += MIDI_OFFSET;
    if (quantized_A0 != prev_A0) {
      usbMIDI.sendNoteOn(prev_A0, 0, channel);
      if (quantized_A0 != OPEN_VAL + MIDI_OFFSET) {
        usbMIDI.sendNoteOn(quantized_A0, 127, channel);
      }
      prev_A0 = quantized_A0;
    }
  }
  // MIDI Controllers should discard incoming MIDI messages.
  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}
